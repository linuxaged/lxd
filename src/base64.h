#pragma once

#include "defines.h"

namespace lxd {
	DLL_PUBLIC size_t Base64_EncodeSizeInBytes( size_t dataSizeInBytes );
	DLL_PUBLIC size_t Base64_DecodeSizeInBytes( const char * base64, const size_t base64SizeInBytes );
	DLL_PUBLIC size_t Base64_Encode( char * base64, const unsigned char * data, const size_t dataSizeInBytes );
	DLL_PUBLIC size_t Base64_Decode( unsigned char * data, const char * base64, const size_t base64SizeInBytes, const size_t maxDecodeSizeInBytes );
}