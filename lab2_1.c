#include <windows.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <stdio.h>

#pragma comment(lib, "setupapi.lib")

int main() {
    HDEVINFO deviceInfoSet;
    SP_DEVINFO_DATA deviceInfoData;
    DWORD i;

    // Obține toate dispozitivele prezente în sistem
    deviceInfoSet = SetupDiGetClassDevs(
        NULL, 0, 0, DIGCF_ALLCLASSES | DIGCF_PRESENT);

    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        printf("Eroare la SetupDiGetClassDevs\n");
        return 1;
    }

    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    for (i = 0; SetupDiEnumDeviceInfo(deviceInfoSet, i, &deviceInfoData); i++) {
        CHAR deviceInstanceId[MAX_DEVICE_ID_LEN];
        if (CM_Get_Device_IDA(deviceInfoData.DevInst, deviceInstanceId, MAX_DEVICE_ID_LEN, 0) == CR_SUCCESS) {
            if (strstr(deviceInstanceId, "USB")) {
                // Dispozitiv USB detectat
                CHAR desc[1024];
                DWORD size = 0;
                if (SetupDiGetDeviceRegistryPropertyA(deviceInfoSet, &deviceInfoData,
                    SPDRP_DEVICEDESC, NULL, (PBYTE)desc, sizeof(desc), &size)) {
                    printf("USB Device: %s\n", desc);
                }
            }
        }
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
    return 0;
}
