#pragma once

#include "defines.h"
#include <string>

namespace lxd {
    class DLL_PUBLIC HttpRequestSync {
    public:
        HttpRequestSync(const wchar_t* host, const wchar_t* path, std::string& result, const std::string& post = {});
        ~HttpRequestSync();
    private:
        unsigned long dwSize = 0;
        unsigned long dwDownloaded = 0;
        bool  bResults = false;
        void* hSession = nullptr;
        void* hConnect = nullptr;
        void* hRequest = nullptr;
    };
}