#include "service.h"


int stop_service(void)
{
	SC_HANDLE SCManager;
	SC_HANDLE ScService;

	SCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (SCManager == NULL)
	{
		printf("OpenSCManager failed (%lu)\n", GetLastError());
		return 1;
	}

	ScService = OpenService(SCManager, "tinky", SERVICE_STOP);
	if (ScService == NULL)
	{
		printf("OpenService failed (%lu)\n", GetLastError());
		CloseServiceHandle(SCManager);
		return 1;
	}

	SERVICE_STATUS ServiceStatus;
	if (!ControlService(ScService, SERVICE_CONTROL_STOP, &ServiceStatus))
	{
		printf("ControlService failed (%lu)\n", GetLastError());
		CloseServiceHandle(ScService);
		CloseServiceHandle(SCManager);
		return 1;
	}

	printf("Service (tinky) stopped successfully\n");
	CloseServiceHandle(ScService);
	CloseServiceHandle(SCManager);
	return 0;
}