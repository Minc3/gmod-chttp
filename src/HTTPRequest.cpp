#include <curl/curl.h>

#include "HTTPRequest.h"
#include "threading.h"

HTTPRequest::HTTPRequest() {
	curl_version_info_data *info = curl_version_info(CURLVERSION_NOW);
	this->headers["User-Agent"] = (std::string) "curl/" + info->version + " gmod-chttp/1.4.0-rc1";
	this->headers["Content-Type"] = "text/plain; charset=utf-8";
}

std::string HTTPRequest::buildQueryString() {
	std::string params;

	for (auto const& e : this->parameters) {
		if (!params.empty())
			params += "&";

		params += e.first + "=" + e.second;
	}

	return params;
}

std::string HTTPRequest::buildURL() {
	std::string fullurl = this->url;

	// Do we need a query string?
	if (!this->method.isLikePost() && !this->parameters.empty()) {
		fullurl += "?" + this->buildQueryString();
	}

	return fullurl;
}

void curlAddHeaders(CURL *curl, HTTPRequest *request) {
	struct curl_slist *headers = nullptr;

	// Add all the headers from the request struct
	for (auto const& e : request->headers)
		headers = curl_slist_append(headers, (e.first + ": " + e.second).c_str());

	// Add the header list to the curl struct
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
}

void curlSetMethod(CURL *curl, HTTPMethod method) {
	if (method.isLikePost())
		curl_easy_setopt(curl, CURLOPT_POST, 1L);

	// METHOD_GET and METHOD_POST are not listed here,
	// since they don't require any specific setup
	switch (method) {
		case HTTPMethod::M_HEAD:
			curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
			break;
		case HTTPMethod::M_PUT:
		case HTTPMethod::M_DELETE:
		case HTTPMethod::M_PATCH:
		case HTTPMethod::M_OPTIONS:
		case HTTPMethod::M_INVALID:
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.toString().c_str());
			break;
		default:
			break;
	}
}

// Write callback for appending to an std::string
size_t curl_string_append(char *contents, size_t size, size_t nmemb, std::string *userp) {
	userp->append(contents, size * nmemb);
	return size * nmemb;
}

// Write callback for appending to an std::map (will split header at the first colon)
size_t curl_headermap_append(char *contents, size_t size, size_t nmemb, std::map<std::string, std::string> *userp) {
	std::string header(contents, size * nmemb);

	std::size_t found = header.find_first_of(':');

	if (found != std::string::npos) {
		(*userp)[header.substr(0, found)] = header.substr(found + 2, header.length() - found - 4);
	}

	return size * nmemb;
}

bool HTTPRequest::run() {
	CURL *curl = curl_easy_init();

	if (!curl) {
		getResultQueue().push(new FailedQueueData(this->success, this->failed, "Failed to init curl struct!"));
		return false;
	}

	CURLcode cres;
	bool ret = true;
	auto *response = new SuccessQueueData(this->success, this->failed);
	std::string postbody;
	const char* redirect;

	curlSetMethod(curl, this->method);

	if (this->method.isLikePost()) {
		// Do we have a request body?
		if (!this->body.empty()) {
			postbody = this->body;
		} else {
			postbody = this->buildQueryString();
		}

		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postbody.size());
		curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, postbody.c_str());
	}

	// Set up saving the response body
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response->body);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_string_append);

	// Set up saving the headers
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response->headers);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, curl_headermap_append);

	curlAddHeaders(curl, this);

	curl_easy_setopt(curl, CURLOPT_URL, this->buildURL().c_str());

resend:
	cres = curl_easy_perform(curl);

	if (cres != CURLE_OK) {
		getResultQueue().push(new FailedQueueData(this->success, this->failed, curl_easy_strerror(cres)));
		ret = false;
		goto cleanup;
	}

	curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &redirect);
	if (redirect) {
		// Clear out saved headers and body
		response->headers.clear();
		response->body.clear();

		// Set the new URL and clear the temp variable
		curl_easy_setopt(curl, CURLOPT_URL, redirect);
		redirect = "";

		goto resend;
	}

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response->code);

	getResultQueue().push(response);

cleanup:
	// Clear out the SuccessQueueData if we don't need it
	if (!ret) {
		delete response;
	}

	curl_easy_cleanup(curl);

	return ret;
}