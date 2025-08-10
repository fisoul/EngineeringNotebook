// CRT
#include <stdio.h>
#include <stdbool.h>
// Platform
#include <Windows.h>
// Project
#include "task_scheduler.h"
#include "example.h"

static int cycle_time_ms[] = { 1000, 2000, 10000 }; // Default cycle times for tasks

static DWORD WINAPI task_scheduler_thread_func(LPVOID para)
{
	return (DWORD)task_scheduler_run((PTASK_SCHEDULER)para);
}

int main(int argc, const char* argv[])
{
	int cpuInfo[4] = { 0 };
	char brand[0x40] = { 0 };

	// 获取 CPU 品牌字符串
	__cpuid(cpuInfo, 0x80000000);
	int nExIds = cpuInfo[0];
	if (nExIds >= 0x80000004) {
		__cpuid((int*)(cpuInfo), 0x80000002);
		memcpy(brand, cpuInfo, sizeof(cpuInfo));
		__cpuid((int*)(cpuInfo), 0x80000003);
		memcpy(brand + 16, cpuInfo, sizeof(cpuInfo));
		__cpuid((int*)(cpuInfo), 0x80000004);
		memcpy(brand + 32, cpuInfo, sizeof(cpuInfo));
		printf("CPU 型号: %s\n", brand);
	}

	HKEY hKey;
	DWORD data = 0;
	DWORD dataSize = sizeof(data);
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
		0, KEY_READ, &hKey) == ERROR_SUCCESS) {
		if (RegQueryValueExA(hKey, "~MHz", NULL, NULL, (LPBYTE)&data, &dataSize) == ERROR_SUCCESS) {
			printf("CPU 主频: %u MHz\n", data);
		}
		RegCloseKey(hKey);
	}

	LARGE_INTEGER freq, start, end;
	QueryPerformanceFrequency(&freq);      // 获取计时器频率

	for (int i = 0; i < 10; i++)
	{
		QueryPerformanceCounter(&start);       // 记录开始时间

		// 模拟一些代码执行
		// 这里是你要计时的代码块
		int count = 1000000;
		for (volatile int i = 0; i < count; i++)
		{

		}

		QueryPerformanceCounter(&end);         // 记录结束时间

		double elapsed_ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
		printf("测试%d: 循环%d次代码执行时间: %.3f ms\n\n", i, count, elapsed_ms);
	}
	


	for (int i = 0; i < argc; i++)
	{
		printf("argv[%d] = %s\n\n", i, argv[i]);
	}
	// Initialize the task scheduler with the default cycle times
	TASK_SCHEDULER* scheduler = task_scheduler_init(sizeof(cycle_time_ms) / sizeof(cycle_time_ms[0]), cycle_time_ms);
	// get task id=1 from the scheduler
	TASK* task1 = task_scheduler_get_task(1);
	TASK* task2 = task_scheduler_get_task(2);
	TASK* task3 = task_scheduler_get_task(3);

	// Create a example program
	PROGRAM program1 = {
		.id = 1,
		.name = "Example Program 1",
		.init = app_example1_init,
		.run = app_example1_run,
	};

	PROGRAM program2 = {
		.id = 1,
		.name = "Example Program 2",
		.init = app_example2_init,
		.run = app_example2_run,
	};
	
	PROGRAM program3 = {
		.id = 1,
		.name = "Example Program 3",
		.init = app_example3_init,
		.run = app_example3_run,
	};

	// add the example program to the task
	task_add_program(task1, &program1);
	task_add_program(task2, &program2);
	task_add_program(task3, &program3);
	
	task_scheduler_info(scheduler); // Print task scheduler information

	// Run the task scheduler
	HANDLE hThread = CreateThread(
		NULL, // Default security attributes
		0, // Default stack size
		(LPTHREAD_START_ROUTINE)task_scheduler_thread_func, // Thread function
		(LPVOID)scheduler, // Parameter to thread function
		0, // Default creation flags
		NULL); // Receive thread identifier

	if (hThread == NULL) {
		fprintf(stderr, "创建调度器线程失败: %d\n", GetLastError());
		return 1;
	}

	// 主线程控制台输入
	printf("输入 q 退出调度器...\n");
	char cmd[16];
	while (scanf_s("%15s", cmd, (unsigned)_countof(cmd))) {
		if (cmd[0] == 'q' || cmd[0] == 'Q') {
			scheduler->quit = 1; // 通知调度器线程退出
			break;
		}
	}

	// 等待调度器线程结束
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	return 0;
}


