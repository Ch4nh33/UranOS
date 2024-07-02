#include "apilib.h"

void HariMain(void)
{
	int i, timer;
	timer = api_alloctimer();
	api_inittimer(timer, 128);
	for (i = 20000000; i >= 20000; i -= i / 100) {
		/* 20 KHz~20Hz : �ΰ����� �鸮�� �Ҹ��� ���� */
		/* i�� 1%�� �ٿ����� */
		api_beep(i);
		api_settimer(timer, 1);		/* 0.01�� */
		if (api_getkey(1) != 128) {
			break;
		}
	}
	api_beep(0);
	api_end();
}
