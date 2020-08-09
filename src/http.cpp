#include <string>
#include "http.h"

std::string buildParameters(HTTPRequest *request) {
	std::string params;

	for (auto const& e : request->parameters) {
		if (!params.empty())
			params += "&";

		params += e.first + "=" + e.second;
	}

	return params;
}

std::string buildUrl(HTTPRequest *request) {
	std::string url = request->url;

	// Do we need a query string?
	if (!request->method.isLikePost() && !request->parameters.empty()) {
		url += "?" + buildParameters(request);
	}

	return url;
}
