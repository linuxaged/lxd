#include "http.h"
#include <Windows.h>
#include <winhttp.h>
#include "debug.h"

namespace lxd {
	HttpRequestSync::HttpRequestSync(const wchar_t* host, const wchar_t* path, std::string& result, const std::string& post) {
        // Specify an HTTP server.
        HINTERNET hSession = WinHttpOpen(L"lxd with WinHTTP Sync /1.0",
                                         WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                         WINHTTP_NO_PROXY_NAME,
                                         WINHTTP_NO_PROXY_BYPASS,
                                         0);

        // Connect
        if(hSession)
            hConnect = WinHttpConnect(hSession, host,
                                      INTERNET_DEFAULT_HTTPS_PORT, 0);

        // Create an HTTP request handle.
        if(hConnect)
            hRequest = WinHttpOpenRequest(hConnect,
                                          post.empty() ? L"GET" : L"POST",
                                          path,
                                          NULL, WINHTTP_NO_REFERER,
                                          WINHTTP_DEFAULT_ACCEPT_TYPES,
                                          WINHTTP_FLAG_SECURE);

        // Send a request.
        if(hRequest)
            bResults = WinHttpSendRequest(hRequest,
                                          WINHTTP_NO_ADDITIONAL_HEADERS,
                                          0,
                                          post.empty() ? WINHTTP_NO_REQUEST_DATA : const_cast<char*>(post.c_str()),
                                          post.empty() ? 0 : post.size(),
                                          post.empty() ? 0 : post.size(),
                                          NULL);

        // End the request.
        if(bResults)
            bResults = WinHttpReceiveResponse(hRequest, NULL);

        // Keep checking for data until there is nothing left.
        if(bResults) {
            do {
                // Check for available data.
                dwSize = 0;
                if(!WinHttpQueryDataAvailable(hRequest, &dwSize))
                    lxd::print("Error {} in WinHttpQueryDataAvailable.\n",
                               GetLastError());

                // Allocate space for the buffer.
                auto pszOutBuffer = new char[dwSize + 1];
                if(!pszOutBuffer) {
                    lxd::print("Out of memory\n");
                    dwSize = 0;
                } else {
                    // Read the data.
                    ZeroMemory(pszOutBuffer, dwSize + 1);
                    if(!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
                                        dwSize, &dwDownloaded)) {
                        lxd::print("Error {} in WinHttpReadData.\n", GetLastError());
                    } else {
                        result += pszOutBuffer;
                    }
                    // Free the memory allocated to the buffer.
                    delete[] pszOutBuffer;
                }
            } while(dwSize > 0);
        }


        // Report any errors.
        if(!bResults)
            lxd::print("Error {} has occurred.\n", GetLastError());
	}

    HttpRequestSync::~HttpRequestSync() {
        // Close any open handles.
        if(hRequest) WinHttpCloseHandle(hRequest);
        if(hConnect) WinHttpCloseHandle(hConnect);
        if(hSession) WinHttpCloseHandle(hSession);
    }
}