#include <stdio.h>
#include "http_request.hpp"

#define REQUEST "GET /index HTTP/1.1\r\nHost: 192.168.0.199:80\r\nConnection: keep-alive\r\nAccept: */*\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.22 (KHTML, like Gecko)     Chrome/25.0.1364.97 Safari/537.22\r\nAccept-Encoding: gzip,deflate,sdch\r\nAccept-Language: en-US,en;q=0.8\r\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3\r\n\r\nthis is the message body\r\n"

// PARSE_HTTP_REQUEST -- request struct?		Status:
// 1) parse request line						done
// 		a) Set method type -- enum				done
// 		b) Set path								done
//		c) Set version							done
// 2) parse headers --> map
// 3) parse body --> ??


int main(void) {
	std::string inputString = REQUEST;
	bool		bodyLine = false;
	bool		firstLine = true;
	size_t		startPos = 0;
	HTTPRequest request;

	while (startPos != std::string::npos)
	{
		size_t	foundPos = inputString.find("\r\n", startPos);
		if (foundPos != std::string::npos)
		{
			size_t length = foundPos - startPos;
			std::string substring = inputString.substr(startPos, length);
			if (substring == "")
			{
				bodyLine = true;
				startPos = foundPos + 2;
				continue ;
			}
			if (firstLine)
			{
				request.setMethodType(substring);
				request.setPath(substring);
				request.setVersion(substring);
				firstLine = false;
			}
			else if (bodyLine)
			{
				request.setBody(substring);
				break ;
			}
			else
			{
				request.setHeader(substring);
			}
			startPos = foundPos + 2;
		}
	}

	std::cout << "\nRequest line:\nMethod: " << request.getMethodType() << "\n";
	std::cout << "Path: " << request.getPath() << "\n";
	std::cout << "Version: " << request.getVersion() << "\n";

	std::cout << "\nThe value of the key \"Host\":\n" << request.getValue("Host") << "\n";

	std::cout << "\nBody:\n" << request.getBody() << "\n";

	return 0;
}

// CREATE_HTTP_RESPONSE -- response struct?
// 1) create response line (version, status_code, reason_phrase)
//      a)
// 2) create headers --> map
// 3) fetch html if available
// 4) create string with all the above content in the correct order --> string

