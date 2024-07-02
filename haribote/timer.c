/* Ÿ�̸� ���� */

#include "bootpack.h"

#define PIT_CTRL	0x0043
#define PIT_CNT0	0x0040

struct TIMERCTL timerctl;

#define TIMER_FLAGS_ALLOC		1	/* Ȯ���� ���� */
#define TIMER_FLAGS_USING		2	/* Ÿ�̸� �۵��� */

void init_pit(void)
{
	int i;
	struct TIMER *t;
	io_out8(PIT_CTRL, 0x34);
	io_out8(PIT_CNT0, 0x9c);
	io_out8(PIT_CNT0, 0x2e);
	timerctl.count = 0;
	for (i = 0; i < MAX_TIMER; i++) {
		timerctl.timers0[i].flags = 0; /* �̻�� */
	}
	t = timer_alloc(); /* 1�� �޾� �´� */
	t->timeout = 0xffffffff;
	t->flags = TIMER_FLAGS_USING;
	t->next = 0; /* ���� �� */
	timerctl.t0 = t; /* ������ sentinel �ۿ� ���� ������ �����̱⵵ �ϴ� */
	timerctl.next = 0xffffffff; /* sentinel �ۿ� ���� ������ sentinel �ð� */
	return;
}

struct TIMER *timer_alloc(void)
{
	int i;
	for (i = 0; i < MAX_TIMER; i++) {
		if (timerctl.timers0[i].flags == 0) {
			timerctl.timers0[i].flags = TIMER_FLAGS_ALLOC;
			timerctl.timers0[i].flags2 = 0;
			return &timerctl.timers0[i];
		}
	}
	return 0; /* �߰ߵ��� �ʾҴ� */
}

void timer_free(struct TIMER *timer)
{
	timer->flags = 0; /* �̻�� */
	return;
}

void timer_init(struct TIMER *timer, struct FIFO32 *fifo, int data)
{
	timer->fifo = fifo;
	timer->data = data;
	return;
}

void timer_settime(struct TIMER *timer, unsigned int timeout)
{
	int e;
	struct TIMER *t, *s;
	timer->timeout = timeout + timerctl.count;
	timer->flags = TIMER_FLAGS_USING;
	e = io_load_eflags();
	io_cli();
	t = timerctl.t0;
	if (timer->timeout <= t->timeout) {
		/* ���ο� �� �� �ִ� ��� */
		timerctl.t0 = timer;
		timer->next = t; /* ������ t */
		timerctl.next = timer->timeout;
		io_store_eflags(e);
		return;
	}
	/* ��� ���� ���� ã�´� */
	for (;;) {
		s = t;
		t = t->next;
		if (timer->timeout <= t->timeout) {
			/* s�� t�� ���̿� �� �� �ִ� ��� */
			s->next = timer; /* s�� ������ timer */
			timer->next = t; /* timer�� ������ t */
			io_store_eflags(e);
			return;
		}
	}
}

void inthandler20(int *esp)
{
	struct TIMER *timer;
	char ts = 0;
	io_out8(PIC0_OCW2, 0x60);	/* IRQ-00 ���� �ϷḦ PIC�� ���� */
	timerctl.count++;
	if (timerctl.next > timerctl.count) {
		return;
	}
	timer = timerctl.t0; /* �켱 ������ ������ timer�� ���� */
	for (;;) {
		/* timers�� Ÿ�̸Ӵ� ��� �������� ���̹Ƿ�, flags�� Ȯ������ �ʴ´� */
		if (timer->timeout > timerctl.count) {
			break;
		}
		/* Ÿ�� �ƿ� */
		timer->flags = TIMER_FLAGS_ALLOC;
		if (timer != task_timer) {
			fifo32_put(timer->fifo, timer->data);
		} else {
			ts = 1; /* task_timer�� Ÿ�� �ƿ� �ߴ� */
		}
		timer = timer->next; /* ���� Ÿ�̸��� ������ timer�� ���� */
	}
	timerctl.t0 = timer;
	timerctl.next = timer->timeout;
	if (ts != 0) {
		task_switch();
	}
	return;
}

int timer_cancel(struct TIMER *timer)
{
	int e;
	struct TIMER *t;
	e = io_load_eflags();
	io_cli();	/* �����߿� Ÿ�̸� ���°� ��ȭ���� �ʰ� �ϱ� ���� */
	if (timer->flags == TIMER_FLAGS_USING) {	/* ��� ó���� �ʿ��Ѱ�?  */
		if (timer == timerctl.t0) {
			/* ���ο��� ����� ��� ó�� */
			t = timer->next;
			timerctl.t0 = t;
			timerctl.next = t->timeout;
		} else {
			/* ���� �̿��� ����� ��� ó�� */
			/* timer�� 1�� ���� ã�´� */
			t = timerctl.t0;
			for (;;) {
				if (t->next == timer) {
					break;
				}
				t = t->next;
			}
			t->next = timer->next; /* ��timer �������� ������, ��timer �������� ����Ű���� �Ѵ� */
		}
		timer->flags = TIMER_FLAGS_ALLOC;
		io_store_eflags(e);
		return 1;	/* ĵ�� ó�� ���� */
	}
	io_store_eflags(e);
	return 0; /* ĵ�� ó���� ���ʿ��ߴ� */
}

void timer_cancelall(struct FIFO32 *fifo)
{
	int e, i;
	struct TIMER *t;
	e = io_load_eflags();
	io_cli();	/* �����߿� Ÿ�̸� ���°� ��ȭ���� �ʰ� �ϱ� ���� */
	for (i = 0; i < MAX_TIMER; i++) {
		t = &timerctl.timers0[i];
		if (t->flags != 0 && t->flags2 != 0 && t->fifo == fifo) {
			timer_cancel(t);
			timer_free(t);
		}
	}
	io_store_eflags(e);
	return;
}
