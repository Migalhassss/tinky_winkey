
#include "service.h"


int delete_service(void)
{
	SC_HANDLE SCManager;
	SC_HANDLE ScService;

	SCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (SCManager == NULL)
	{
		printf("OpenSCManager failed (%lu)\n", GetLastError());
		return 1;
	}

	ScService = OpenService(SCManager, "tinky", DELETE);
	if (ScService == NULL)
	{
		printf("OpenService failed (%lu)\n", GetLastError());
		CloseServiceHandle(SCManager);
		return 1;
	}

	if (!DeleteService(ScService))
	{
		printf("DeleteService failed (%lu)\n", GetLastError());
		CloseServiceHandle(ScService);
		CloseServiceHandle(SCManager);
		return 1;
	}

	printf("Service (tinky) deleted successfully\n");
	CloseServiceHandle(ScService);
	CloseServiceHandle(SCManager);
	return 0;
}