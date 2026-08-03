#include "service.h"

int start_service(void)
{
	SC_HANDLE SCManager;
	SC_HANDLE ScService;

	SCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (SCManager == NULL)
	{
		printf("OpenSCManager failed (%lu)\n", GetLastError());
		return 1;
	}

	ScService = OpenService(SCManager, "tinky", SERVICE_START);
	if (ScService == NULL)
	{
		printf("OpenService failed (%lu)\n", GetLastError());
		CloseServiceHandle(SCManager);
		return 1;
	}

	if (!StartService(ScService, 0, NULL))
	{
		printf("StartService failed (%lu)\n", GetLastError());
		CloseServiceHandle(ScService);
		CloseServiceHandle(SCManager);
		return 1;
	}

	printf("Service (tinky) started successfully\n");
	CloseServiceHandle(ScService);
	CloseServiceHandle(SCManager);
	return 0;
}