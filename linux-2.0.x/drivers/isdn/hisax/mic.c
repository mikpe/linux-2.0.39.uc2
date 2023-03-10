/* $Id: mic.c,v 1.1.1.1 1999-11-22 03:47:20 christ Exp $

 * mic.c  low level stuff for mic cards
 *
 * Copyright (C) 1997 
 *
 * Author  Stephan von Krawczynski <skraw@ithnet.com>
 *
 *
 * $Log: mic.c,v $
 * Revision 1.1.1.1  1999-11-22 03:47:20  christ
 * Importing new-wave v1.0.4
 *
 * Revision 1.1.2.5  1998/04/08 21:58:43  keil
 * New init code
 *
 * Revision 1.1.2.4  1998/02/17 15:39:20  keil
 * fix reset problem
 *
 * Revision 1.1.2.3  1998/01/27 22:37:25  keil
 * fast io
 *
 * Revision 1.1.2.2  1997/11/15 18:50:54  keil
 * new common init function
 *
 * Revision 1.1.2.1  1997/10/17 22:10:54  keil
 * new files on 2.0
 *
 *
 */

#define __NO_VERSION__
#include "hisax.h"
#include "isac.h"
#include "hscx.h"
#include "isdnl1.h"

extern const char *CardType[];

const char *mic_revision = "$Revision: 1.1.1.1 $";

#define byteout(addr,val) outb(val,addr)
#define bytein(addr) inb(addr)

#define MIC_ISAC	2
#define MIC_HSCX	1
#define MIC_ADR		7

/* CARD_ADR (Write) */
#define MIC_RESET      0x3	/* same as DOS driver */

static inline u_char
readreg(unsigned int ale, unsigned int adr, u_char off)
{
	register u_char ret;
	long flags;

	save_flags(flags);
	cli();
	byteout(ale, off);
	ret = bytein(adr);
	restore_flags(flags);

	return (ret);
}

static inline void
readfifo(unsigned int ale, unsigned int adr, u_char off, u_char * data, int size)
{
	/* fifo read without cli because it's allready done  */

	byteout(ale, off);
	insb(adr, data, size);
}


static inline void
writereg(unsigned int ale, unsigned int adr, u_char off, u_char data)
{
	long flags;

	save_flags(flags);
	cli();
	byteout(ale, off);
	byteout(adr, data);
	restore_flags(flags);
}

static inline void
writefifo(unsigned int ale, unsigned int adr, u_char off, u_char * data, int size)
{
	/* fifo write without cli because it's allready done  */
	byteout(ale, off);
	outsb(adr, data, size);
}

/* Interface functions */

static u_char
ReadISAC(struct IsdnCardState *cs, u_char offset)
{
	return (readreg(cs->hw.mic.adr, cs->hw.mic.isac, offset));
}

static void
WriteISAC(struct IsdnCardState *cs, u_char offset, u_char value)
{
	writereg(cs->hw.mic.adr, cs->hw.mic.isac, offset, value);
}

static void
ReadISACfifo(struct IsdnCardState *cs, u_char * data, int size)
{
	readfifo(cs->hw.mic.adr, cs->hw.mic.isac, 0, data, size);
}

static void
WriteISACfifo(struct IsdnCardState *cs, u_char * data, int size)
{
	writefifo(cs->hw.mic.adr, cs->hw.mic.isac, 0, data, size);
}

static u_char
ReadHSCX(struct IsdnCardState *cs, int hscx, u_char offset)
{
	return (readreg(cs->hw.mic.adr,
			cs->hw.mic.hscx, offset + (hscx ? 0x40 : 0)));
}

static void
WriteHSCX(struct IsdnCardState *cs, int hscx, u_char offset, u_char value)
{
	writereg(cs->hw.mic.adr,
		 cs->hw.mic.hscx, offset + (hscx ? 0x40 : 0), value);
}

/*
 * fast interrupt HSCX stuff goes here
 */

#define READHSCX(cs, nr, reg) readreg(cs->hw.mic.adr, \
		cs->hw.mic.hscx, reg + (nr ? 0x40 : 0))
#define WRITEHSCX(cs, nr, reg, data) writereg(cs->hw.mic.adr, \
		cs->hw.mic.hscx, reg + (nr ? 0x40 : 0), data)

#define READHSCXFIFO(cs, nr, ptr, cnt) readfifo(cs->hw.mic.adr, \
		cs->hw.mic.hscx, (nr ? 0x40 : 0), ptr, cnt)

#define WRITEHSCXFIFO(cs, nr, ptr, cnt) writefifo(cs->hw.mic.adr, \
		cs->hw.mic.hscx, (nr ? 0x40 : 0), ptr, cnt)

#include "hscx_irq.c"

static void
mic_interrupt(int intno, void *dev_id, struct pt_regs *regs)
{
	struct IsdnCardState *cs = dev_id;
	u_char val, stat = 0;

	if (!cs) {
		printk(KERN_WARNING "mic: Spurious interrupt!\n");
		return;
	}
	val = readreg(cs->hw.mic.adr, cs->hw.mic.hscx, HSCX_ISTA + 0x40);
      Start_HSCX:
	if (val) {
		hscx_int_main(cs, val);
		stat |= 1;
	}
	val = readreg(cs->hw.mic.adr, cs->hw.mic.isac, ISAC_ISTA);
      Start_ISAC:
	if (val) {
		isac_interrupt(cs, val);
		stat |= 2;
	}
	val = readreg(cs->hw.mic.adr, cs->hw.mic.hscx, HSCX_ISTA + 0x40);
	if (val) {
		if (cs->debug & L1_DEB_HSCX)
			debugl1(cs, "HSCX IntStat after IntRoutine");
		goto Start_HSCX;
	}
	val = readreg(cs->hw.mic.adr, cs->hw.mic.isac, ISAC_ISTA);
	if (val) {
		if (cs->debug & L1_DEB_ISAC)
			debugl1(cs, "ISAC IntStat after IntRoutine");
		goto Start_ISAC;
	}
	if (stat & 1) {
		writereg(cs->hw.mic.adr, cs->hw.mic.hscx, HSCX_MASK, 0xFF);
		writereg(cs->hw.mic.adr, cs->hw.mic.hscx, HSCX_MASK + 0x40, 0xFF);
		writereg(cs->hw.mic.adr, cs->hw.mic.hscx, HSCX_MASK, 0x0);
		writereg(cs->hw.mic.adr, cs->hw.mic.hscx, HSCX_MASK + 0x40, 0x0);
	}
	if (stat & 2) {
		writereg(cs->hw.mic.adr, cs->hw.mic.isac, ISAC_MASK, 0xFF);
		writereg(cs->hw.mic.adr, cs->hw.mic.isac, ISAC_MASK, 0x0);
	}
}

void
release_io_mic(struct IsdnCardState *cs)
{
	int bytecnt = 8;

	if (cs->hw.mic.cfg_reg)
		release_region(cs->hw.mic.cfg_reg, bytecnt);
}

static int
mic_card_msg(struct IsdnCardState *cs, int mt, void *arg)
{
	switch (mt) {
		case CARD_RESET:
			return(0);
		case CARD_RELEASE:
			release_io_mic(cs);
			return(0);
		case CARD_SETIRQ:
			return(request_irq(cs->irq, &mic_interrupt,
					I4L_IRQ_FLAG, "HiSax", cs));
		case CARD_INIT:
			inithscx(cs); /* /RTSA := ISAC RST */
			inithscxisac(cs, 3);
			return(0);
		case CARD_TEST:
			return(0);
	}
	return(0);
}

__initfunc(int
setup_mic(struct IsdnCard *card))
{
	int bytecnt;
	struct IsdnCardState *cs = card->cs;
	char tmp[64];

	strcpy(tmp, mic_revision);
	printk(KERN_INFO "HiSax: mic driver Rev. %s\n", HiSax_getrev(tmp));
	if (cs->typ != ISDN_CTYPE_MIC)
		return (0);

	bytecnt = 8;
	cs->hw.mic.cfg_reg = card->para[1];
	cs->irq = card->para[0];
	cs->hw.mic.adr = cs->hw.mic.cfg_reg + MIC_ADR;
	cs->hw.mic.isac = cs->hw.mic.cfg_reg + MIC_ISAC;
	cs->hw.mic.hscx = cs->hw.mic.cfg_reg + MIC_HSCX;

	if (check_region((cs->hw.mic.cfg_reg), bytecnt)) {
		printk(KERN_WARNING
		       "HiSax: %s config port %x-%x already in use\n",
		       CardType[card->typ],
		       cs->hw.mic.cfg_reg,
		       cs->hw.mic.cfg_reg + bytecnt);
		return (0);
	} else {
		request_region(cs->hw.mic.cfg_reg, bytecnt, "mic isdn");
	}

	printk(KERN_INFO
	       "mic: defined at 0x%x IRQ %d\n",
	       cs->hw.mic.cfg_reg,
	       cs->irq);
	cs->readisac = &ReadISAC;
	cs->writeisac = &WriteISAC;
	cs->readisacfifo = &ReadISACfifo;
	cs->writeisacfifo = &WriteISACfifo;
	cs->BC_Read_Reg = &ReadHSCX;
	cs->BC_Write_Reg = &WriteHSCX;
	cs->BC_Send_Data = &hscx_fill_fifo;
	cs->cardmsg = &mic_card_msg;
	ISACVersion(cs, "mic:");
	if (HscxVersion(cs, "mic:")) {
		printk(KERN_WARNING
		    "mic: wrong HSCX versions check IO address\n");
		release_io_mic(cs);
		return (0);
	}
	return (1);
}
