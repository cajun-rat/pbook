#include <windows.h>

void randombytes(unsigned char *x,unsigned long long xlen) {
	HCRYPTPROV cryptprovider;
	int ok;
	cryptprovider = 0;

	ok = 0;
	for(;;) {
		ok = CryptAcquireContextW(&cryptprovider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT);
		if (ok) break;
		Sleep(1);
	}

	for (;;) {
		ok = CryptGenRandom(cryptprovider, xlen, x);
		if (ok) break;
		Sleep(1);
	}
	ok = CryptReleaseContext(cryptprovider, 0);
}

