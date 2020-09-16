#include "crypt.h"
#include "debug.h"
#include <windows.h>
#include <bcrypt.h>

#define NT_SUCCESS(Status)          (((NTSTATUS)(Status)) >= 0)
#define STATUS_UNSUCCESSFUL         ((NTSTATUS)0xC0000001L)

namespace lxd {
	std::vector<unsigned char> hash(HashAlgorithm algorithm, const char* message, size_t msgSize, const char* key, size_t keySize) {
        BCRYPT_ALG_HANDLE       hAlg = NULL;
        BCRYPT_HASH_HANDLE      hHash = NULL;
        NTSTATUS                status = STATUS_UNSUCCESSFUL;
        DWORD                   cbData = 0;
        DWORD                   cbHash = 0;
        DWORD                   cbHashObject = 0;
        PBYTE                   pbHashObject = NULL;
        PBYTE                   pbHash = NULL;

        //open an algorithm handle
        if(!NT_SUCCESS(status = BCryptOpenAlgorithmProvider(
            &hAlg,
            BCRYPT_SHA256_ALGORITHM,
            NULL,
            HMAC == algorithm ? BCRYPT_ALG_HANDLE_HMAC_FLAG : 0))) {
            lxd::print(L"**** Error {0:x} returned by BCryptOpenAlgorithmProvider\n", status);
            goto Cleanup;
        }

        //calculate the size of the buffer to hold the hash object
        if(!NT_SUCCESS(status = BCryptGetProperty(
            hAlg,
            BCRYPT_OBJECT_LENGTH,
            (PBYTE)&cbHashObject,
            sizeof(DWORD),
            &cbData,
            0))) {
            lxd::print(L"**** Error {0:x} returned by BCryptGetProperty\n", status);
            goto Cleanup;
        }

        //allocate the hash object on the heap
        pbHashObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHashObject);
        if(NULL == pbHashObject) {
            lxd::print(L"**** memory allocation failed\n");
            goto Cleanup;
        }

        //calculate the length of the hash
        if(!NT_SUCCESS(status = BCryptGetProperty(
            hAlg,
            BCRYPT_HASH_LENGTH,
            (PBYTE)&cbHash,
            sizeof(DWORD),
            &cbData,
            0))) {
            lxd::print(L"**** Error {0:x} returned by BCryptGetProperty\n", status);
            goto Cleanup;
        }

        //allocate the hash buffer on the heap
        pbHash = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHash);
        if(NULL == pbHash) {
            lxd::print(L"**** memory allocation failed\n");
            goto Cleanup;
        }

        //create a hash
        if(!NT_SUCCESS(status = BCryptCreateHash(
            hAlg,
            &hHash,
            pbHashObject,
            cbHashObject,
            (PBYTE)key,
            keySize,
            0))) {
            lxd::print(L"**** Error {0:x} returned by BCryptCreateHash\n", status);
            goto Cleanup;
        }

        //hash some data
        if(!NT_SUCCESS(status = BCryptHashData(
            hHash,
            (PBYTE)message,
            msgSize,
            0))) {
            lxd::print(L"**** Error {0:x} returned by BCryptHashData\n", status);
            goto Cleanup;
        }

        //close the hash
        if(!NT_SUCCESS(status = BCryptFinishHash(
            hHash,
            pbHash,
            cbHash,
            0))) {
            lxd::print(L"**** Error {0:x} returned by BCryptFinishHash\n", status);
            goto Cleanup;
        }

Cleanup:

        if(hAlg) {
            BCryptCloseAlgorithmProvider(hAlg, 0);
        }

        if(hHash) {
            BCryptDestroyHash(hHash);
        }

        if(pbHashObject) {
            HeapFree(GetProcessHeap(), 0, pbHashObject);
        }

        std::vector<unsigned char> result;
        if(pbHash) {
            result.insert(result.end(), pbHash, pbHash + cbHash);
            HeapFree(GetProcessHeap(), 0, pbHash);
        }
		return result;
	}
}