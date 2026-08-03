
#include "service.h"



int install_service(void)
{
	SC_HANDLE SCManager;
	SC_HANDLE ScService;

	char service_path[MAX_PATH];

	GetModuleFileName(NULL, service_path, sizeof(service_path));

	SCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (SCManager == NULL)
	{
		printf("OpenSCManager failed (%lu)\n", GetLastError());
		return 1;
	}

	ScService = CreateService(
		SCManager, // service token
		"tinky", // service name
		"tinky", // display name
		SERVICE_ALL_ACCESS, // access
		SERVICE_WIN32_OWN_PROCESS, // service type
		SERVICE_DEMAND_START, // start type
		SERVICE_ERROR_NORMAL, // error control type
		service_path, // service path
		NULL, // no load ordering group
		NULL, // no tag identifier
		NULL, // no dependencies
		NULL, // LocalSystem account
		NULL // no password
	);

	if (ScService == NULL)
	{
		printf("CreateService failed (%lu)\n", GetLastError());
		CloseServiceHandle(SCManager);
		return 1;
	}

	printf("Service (tinky) installed successfully\n");
	CloseServiceHandle(ScService);
	CloseServiceHandle(SCManager);
	return 0;
}

