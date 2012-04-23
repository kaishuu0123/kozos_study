#include "defines.h"
#include "serial.h"

#define SERIAL_SCI_NUM 3

/* SCI の定義 */
#define H8_3069F_SCI0 ((volatile struct h8_3069f_sci *)0xffffb0)
#define H8_3069F_SCI1 ((volatile struct h8_3069f_sci *)0xffffb8)
#define H8_3069F_SCI2 ((volatile struct h8_3069f_sci *)0xffffc0)

/* SCIの各種レジスタの定義 */
struct h8_3069f_sci {
	volatile uint8 smr;
	volatile uint8 brr;
	volatile uint8 scr;
	volatile uint8 tdr;
	volatile uint8 ssr;
	volatile uint8 rdr;
	volatile uint8 scmr;
};

/* SMRの各ビットの定義 */
#define H8_3069F_SCI_SMR_CKS_PER1	(0 << 0)
#define H8_3069F_SCI_SMR_CKS_PER4	(1 << 0)
#define H8_3069F_SCI_SMR_CKS_PER16	(2 << 0)
#define H8_3069F_SCI_SMR_CKS_PER64	(3 << 0)
#define H8_3069F_SCI_SMR_CKS_MP		(1 << 2)
#define H8_3069F_SCI_SMR_CKS_STOP	(2 << 3)
#define H8_3069F_SCI_SMR_CKS_OE		(1 << 4)
#define H8_3069F_SCI_SMR_CKS_PE		(1 << 5)
#define H8_3069F_SCI_SMR_CKS_CHR	(1 << 6)
#define H8_3069F_SCI_SMR_CKS_CA		(1 << 7)

/* SCRの各ビットの定義 */
#define  H8_3069F_SCI_SCR_CKE0	(1 << 0)
#define  H8_3069F_SCI_SCR_CKE1	(1 << 1)
#define  H8_3069F_SCI_SCR_TEIE	(1 << 2)
#define  H8_3069F_SCI_SCR_MPIE	(1 << 3)
#define  H8_3069F_SCI_SCR_RE	(1 << 4) /* 受信有効 */
#define  H8_3069F_SCI_SCR_TE	(1 << 5) /* 送信有効 */
#define  H8_3069F_SCI_SCR_RIE	(1 << 6) /* 受信割り込み有効 */
#define  H8_3069F_SCI_SCR_TIE	(1 << 6) /* 送信割り込み有効 */

/* SSRの各ビット定義 */
#define H8_3069F_SCI_SSR_MPBT	(1 << 0)
#define H8_3069F_SCI_SSR_MPB	(1 << 1)
#define H8_3069F_SCI_SSR_TEND	(1 << 2)
#define H8_3069F_SCI_SSR_PER	(1 << 3)
#define H8_3069F_SCI_SSR_FERERS	(1 << 4)
#define H8_3069F_SCI_SSR_ORER	(1 << 5)
#define H8_3069F_SCI_SSR_RDRF	(1 << 6) /* 受信完了 */
#define H8_3069F_SCI_SSR_TDRE	(1 << 7) /* 送信完了 */

static struct {
	volatile struct h8_3069f_sci *sci;
} regs[SERIAL_SCI_NUM] = {
	{ H8_3069F_SCI0 },
	{ H8_3069F_SCI1 },
	{ H8_3069F_SCI2 },
};

/* デバイス初期化 */
int
serial_init(int index)
{
	volatile struct h8_3069f_sci *sci = regs[index].sci;

	sci->scr = 0;
	sci->smr = 0;
	sci->brr = 64; /* 20MHz のクロックから9600bpsを生成 */
	/* 送受信を有効にする */
	sci->scr = H8_3069F_SCI_SCR_RE | H8_3069F_SCI_SCR_TE; 
	sci->ssr =0;

	return 0;
}

/* SSR TDRE フラグをチェックして送信可能かどうかのチェック */
int
serial_is_send_enable(int index)
{
	volatile struct h8_3069f_sci *sci = regs[index].sci;
	return (sci->ssr & H8_3069F_SCI_SSR_TDRE);
}

int
serial_send_byte(int index, unsigned char c)
{
	volatile struct h8_3069f_sci *sci = regs[index].sci;

	/* 送信可能になるまで待つ */
	while (!serial_is_send_enable(index))
	    ;
	sci->tdr = c;
	sci->ssr &= ~H8_3069F_SCI_SSR_TDRE; /* 送信開始 */

	return 0;
}

int
serial_is_recv_enable(int index)
{
	volatile struct h8_3069f_sci *sci = regs[index].sci;

	return (sci->ssr & H8_3069F_SCI_SSR_RDRF);
}

unsigned char
serial_recv_byte(int index)
{
	volatile struct h8_3069f_sci *sci = regs[index].sci;
	unsigned char c;

	while (!serial_is_recv_enable(index))
	    ;
	c = sci->rdr;
	sci->ssr &= ~H8_3069F_SCI_SSR_RDRF;

	return c;
}

int
serial_intr_is_send_enable(int index)
{
	volatile struct h8_3069f_sci *sci = regs[index].sci;
	return (sci->scr & H8_3069F_SCI_SCR_TIE) ? 1 : 0;
}

void
serial_intr_send_enable(int index)
{
	volatile struct h8_3069f_sci *sci = regs[index].sci;
	sci->scr |= H8_3069F_SCI_SCR_TIE;
}

void
serial_intr_send_disable(int index)
{
	volatile struct h8_3069f_sci *sci = regs[index].sci;
	sci->scr &= ~H8_3069F_SCI_SCR_TIE;
}

int
serial_intr_is_recv_enable(int index)
{
	volatile struct h8_3069f_sci *sci = regs[index].sci;
	return (sci->scr & H8_3069F_SCI_SCR_RIE) ? 1 : 0;
}

void
serial_intr_recv_enable(int index)
{
	volatile struct h8_3069f_sci *sci = regs[index].sci;
	sci->scr |= H8_3069F_SCI_SCR_RIE;
}

void
serial_intr_recv_disable(int index)
{
	volatile struct h8_3069f_sci *sci = regs[index].sci;
	sci->scr &= ~H8_3069F_SCI_SCR_RIE;
}