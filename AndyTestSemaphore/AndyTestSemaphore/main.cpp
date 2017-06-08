#include <process.h>

#include "Poco\Semaphore.h"
#include "Poco\Mutex.h"

using Poco::Semaphore;
using Poco::FastMutex;

unsigned int CALLBACK Fun(void* lpVoid);

const int THREAD_NUM = 10;
int g_num = 0;

Semaphore semp(1);	// 5������5���źſ���ͬʱwait
FastMutex mt;

int main()
{
	HANDLE hand[THREAD_NUM];
	for (int i = 0; i != THREAD_NUM; ++i)
	{
		hand[i] = (HANDLE)_beginthreadex(NULL, 0, Fun, &i, 0, NULL);
		semp.wait();
	}
	WaitForMultipleObjects(THREAD_NUM, hand, TRUE, INFINITE);

	for (int i = 0; i != THREAD_NUM; ++i)
		CloseHandle(hand[i]);

	return 0;
}

unsigned int CALLBACK Fun( void* lpVoid )
{
	int i = *(int*)lpVoid;
	semp.set();

	Sleep(50);

	Poco::ScopedLock<FastMutex> lock(mt);
	g_num++;
	printf("�̱߳��Ϊ%d  ȫ����ԴֵΪ%d\n", i, g_num);

	return 0;
}