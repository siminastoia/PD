#include <windows.h>
#include <stdio.h>

int main() {
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    printf("Dimensiunea unei pagini: %u bytes\n", si.dwPageSize);

    int N;
    printf("Introdu numarul de pagini de alocat: ");
    scanf("%d", &N);

    if (N <= 0) {
        printf("Numarul trebuie sa fie pozitiv.\n");
        return 1;
    }

    SIZE_T sizeToAllocate = N * si.dwPageSize;
    LPVOID lpMemory = VirtualAlloc(
        NULL,
        sizeToAllocate,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    if (lpMemory == NULL) {
        printf("Alocarea a esuat. Cod eroare: %lu\n", GetLastError());
        return 1;
    } else {
        printf("Memorie alocata cu succes la adresa %p\n", lpMemory);
    }

    VirtualFree(lpMemory, 0, MEM_RELEASE);
    return 0;
}
