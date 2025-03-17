//EX1
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int _tmain(void) {
    HKEY hServicesKey;
    LONG lResult;
    DWORD dwIndex = 0;
    TCHAR szSubKeyName[256];
    DWORD dwSubKeyNameSize;

    // Deschidem cheia HKLM\SYSTEM\CurrentControlSet\Services
    lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                           _T("SYSTEM\\CurrentControlSet\\Services"),
                           0,
                           KEY_READ,
                           &hServicesKey);

    if (lResult != ERROR_SUCCESS) {
        _tprintf(_T("Nu s-a putut deschide cheia Services! Eroare: %ld\n"), lResult);
        return 1;
    }

    // Enumeram toate subcheile
    while (TRUE) {
        dwSubKeyNameSize = sizeof(szSubKeyName) / sizeof(TCHAR);
        lResult = RegEnumKeyEx(hServicesKey,
                               dwIndex,
                               szSubKeyName,
                               &dwSubKeyNameSize,
                               NULL,
                               NULL,
                               NULL,
                               NULL);
        
        if (lResult == ERROR_NO_MORE_ITEMS) {
            break; // Nu mai sunt subchei
        } else if (lResult != ERROR_SUCCESS) {
            _tprintf(_T("Eroare la enumerarea subcheilor: %ld\n"), lResult);
            break;
        }

        // Deschidem fiecare subcheie
        HKEY hSubKey;
        lResult = RegOpenKeyEx(hServicesKey, szSubKeyName, 0, KEY_READ, &hSubKey);
        if (lResult == ERROR_SUCCESS) {
            // Cautam valoarea ImagePath
            TCHAR szImagePath[512];
            DWORD dwSize = sizeof(szImagePath);
            DWORD dwType;

            lResult = RegQueryValueEx(hSubKey,
                                      _T("ImagePath"),
                                      NULL,
                                      &dwType,
                                      (LPBYTE)szImagePath,
                                      &dwSize);
            if (lResult == ERROR_SUCCESS && (dwType == REG_EXPAND_SZ || dwType == REG_SZ)) {
                _tprintf(_T("Subcheie: %s\n"), szSubKeyName);
                _tprintf(_T("ImagePath: %s\n\n"), szImagePath);
            }

            // Inchidem subcheia
            RegCloseKey(hSubKey);
        }

        dwIndex++; // Urmatoarea subcheie
    }

    // Inchidem cheia principala
    RegCloseKey(hServicesKey);

    return 0;
}
