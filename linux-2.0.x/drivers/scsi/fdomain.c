/* fdomain.c -- Future Domain TMC-16x0 SCSI driver
 * Created: Sun May  3 18:53:19 1992 by faith@cs.unc.edu
 * Revised: Sat Nov  2 09:27:47 1996 by root@cs.unc.edu
 * Author: Rickard E. Faith, faith@cs.unc.edu
 * Copyright 1992, 1993, 1994, 1995, 1996 Rickard E. Faith
 *
 * $Id: fdomain.c,v 1.1.1.1 1999-11-22 03:47:21 christ Exp $

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.

 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.

 **************************************************************************

 SUMMARY:

 Future Domain BIOS versions supported for autodetect:
    2.0, 3.0, 3.2, 3.4 (1.0), 3.5 (2.0), 3.6, 3.61
 Chips are supported:
    TMC-1800, TMC-18C50, TMC-18C30, TMC-36C70
 Boards supported:
    Future Domain TMC-1650, TMC-1660, TMC-1670, TMC-1680, TMC-1610M/MER/MEX
    Future Domain TMC-3260 (PCI)
    Quantum ISA-200S, ISA-250MG
    Adaptec AHA-2920 (PCI)
    IBM ?
 LILO command-line options:
    fdomain=<PORT_BASE>,<IRQ>[,<ADAPTER_ID>]



 DESCRIPTION:
 
 This is the Linux low-level SCSI driver for Future Domain TMC-1660/1680
 TMC-1650/1670, and TMC-3260 SCSI host adapters.  The 1650 and 1670 have a
 25-pin external connector, whereas the 1660 and 1680 have a SCSI-2 50-pin
 high-density external connector.  The 1670 and 1680 have floppy disk
 controllers built in.  The TMC-3260 is a PCI bus card.

 Future Domain's older boards are based on the TMC-1800 chip, and this
 driver was originally written for a TMC-1680 board with the TMC-1800 chip.
 More recently, boards are being produced with the TMC-18C50 and TMC-18C30
 chips.  The latest and greatest board may not work with this driver.  If
 you have to patch this driver so that it will recognize your board's BIOS
 signature, then the driver may fail to function after the board is
 detected.

 Please note that the drive ordering that Future Domain implemented in BIOS
 versions 3.4 and 3.5 is the opposite of the order (currently) used by the
 rest of the SCSI industry.  If you have BIOS version 3.4 or 3.5, and have
 more then one drive, then the drive ordering will be the reverse of that
 which you see under DOS.  For example, under DOS SCSI ID 0 will be D: and
 SCSI ID 1 will be C: (the boot device).  Under Linux, SCSI ID 0 will be
 /dev/sda and SCSI ID 1 will be /dev/sdb.  The Linux ordering is consistent
 with that provided by all the other SCSI drivers for Linux.  If you want
 this changed, you will probably have to patch the higher level SCSI code.
 If you do so, please send me patches that are protected by #ifdefs.

 If you have a TMC-8xx or TMC-9xx board, then this is not the driver for
 your board.  Please refer to the Seagate driver for more information and
 possible support.


 
 HISTORY:

 Linux       Driver      Driver
 Version     Version     Date         Support/Notes

             0.0          3 May 1992  V2.0 BIOS; 1800 chip
 0.97        1.9         28 Jul 1992
 0.98.6      3.1         27 Nov 1992
 0.99        3.2          9 Dec 1992

 0.99.3      3.3         10 Jan 1993  V3.0 BIOS
 0.99.5      3.5         18 Feb 1993
 0.99.10     3.6         15 May 1993  V3.2 BIOS; 18C50 chip
 0.99.11     3.17         3 Jul 1993  (now under RCS)
 0.99.12     3.18        13 Aug 1993
 0.99.14     5.6         31 Oct 1993  (reselection code removed)

 0.99.15     5.9         23 Jan 1994  V3.4 BIOS (preliminary)
 1.0.8/1.1.1 5.15         1 Apr 1994  V3.4 BIOS; 18C30 chip (preliminary)
 1.0.9/1.1.3 5.16         7 Apr 1994  V3.4 BIOS; 18C30 chip
 1.1.38      5.18        30 Jul 1994  36C70 chip (PCI version of 18C30)
 1.1.62      5.20         2 Nov 1994  V3.5 BIOS
 1.1.73      5.22         7 Dec 1994  Quantum ISA-200S board; V2.0 BIOS

 1.1.82      5.26        14 Jan 1995  V3.5 BIOS; TMC-1610M/MER/MEX board
 1.2.10      5.28         5 Jun 1995  Quantum ISA-250MG board; V2.0, V2.01 BIOS
 1.3.4       5.31        23 Jun 1995  PCI BIOS-32 detection (preliminary)
 1.3.7       5.33         4 Jul 1995  PCI BIOS-32 detection
 1.3.28      5.36        17 Sep 1995  V3.61 BIOS; LILO command-line support
 1.3.34      5.39        12 Oct 1995  V3.60 BIOS; /proc
 1.3.72      5.39         8 Feb 1996  Adaptec AHA-2920 board
 1.3.85      5.41         4 Apr 1996
 2.0.12      5.44         8 Aug 1996  Use ID 7 for all PCI cards

 

 REFERENCES USED:

 "TMC-1800 SCSI Chip Specification (FDC-1800T)", Future Domain Corporation,
 1990.

 "Technical Reference Manual: 18C50 SCSI Host Adapter Chip", Future Domain
 Corporation, January 1992.

 "LXT SCSI Products: Specifications and OEM Technical Manual (Revision
 B/September 1991)", Maxtor Corporation, 1991.

 "7213S product Manual (Revision P3)", Maxtor Corporation, 1992.

 "Draft Proposed American National Standard: Small Computer System
 Interface - 2 (SCSI-2)", Global Engineering Documents. (X3T9.2/86-109,
 revision 10h, October 17, 1991)

 Private communications, Drew Eckhardt (drew@cs.colorado.edu) and Eric
 Youngdale (ericy@cais.com), 1992.

 Private communication, Tuong Le (Future Domain Engineering department),
 1994. (Disk geometry computations for Future Domain BIOS version 3.4, and
 TMC-18C30 detection.)

 Hogan, Thom. The Programmer's PC Sourcebook. Microsoft Press, 1988. Page
 60 (2.39: Disk Partition Table Layout).

 "18C30 Technical Reference Manual", Future Domain Corporation, 1993, page
 6-1.


 
 NOTES ON REFERENCES:

 The Maxtor manuals were free.  Maxtor telephone technical support is
 great!

 The Future Domain manuals were $25 and $35.  They document the chip, not
 the TMC-16x0 boards, so some information I had to guess at.  In 1992,
 Future Domain sold DOS BIOS source for $250 and the UN*X driver source was
 $750, but these required a non-disclosure agreement, so even if I could
 have afforded them, they would *not* have been useful for writing this
 publically distributable driver.  Future Domain technical support has
 provided some information on the phone and have sent a few useful FAXs.
 They have been much more helpful since they started to recognize that the
 word "Linux" refers to an operating system :-).

 

 ALPHA TESTERS:

 There are many other alpha testers that come and go as the driver
 develops.  The people listed here were most helpful in times of greatest
 need (mostly early on -- I've probably left out a few worthy people in
 more recent times):

 Todd Carrico (todd@wutc.wustl.edu), Dan Poirier (poirier@cs.unc.edu ), Ken
 Corey (kenc@sol.acs.unt.edu), C. de Bruin (bruin@bruin@sterbbs.nl), Sakari
 Aaltonen (sakaria@vipunen.hit.fi), John Rice (rice@xanth.cs.odu.edu), Brad
 Yearwood (brad@optilink.com), and Ray Toy (toy@soho.crd.ge.com).

 Special thanks to Tien-Wan Yang (twyang@cs.uh.edu), who graciously lent me
 his 18C50-based card for debugging.  He is the sole reason that this
 driver works with the 18C50 chip.

 Thanks to Dave Newman (dnewman@crl.com) for providing initial patches for
 the version 3.4 BIOS.

 Thanks to James T. McKinley (mckinley@msupa.pa.msu.edu) for providing
 patches that support the TMC-3260, a PCI bus card with the 36C70 chip.
 The 36C70 chip appears to be "completely compatible" with the 18C30 chip.

 Thanks to Eric Kasten (tigger@petroglyph.cl.msu.edu) for providing the
 patch for the version 3.5 BIOS.

 Thanks for Stephen Henson (shenson@nyx10.cs.du.edu) for providing the
 patch for the Quantum ISA-200S SCSI adapter.
 
 Thanks to Adam Bowen for the signature to the 1610M/MER/MEX scsi cards, to
 Martin Andrews (andrewm@ccfadm.eeg.ccf.org) for the signature to some
 random TMC-1680 repackaged by IBM; and to Mintak Ng (mintak@panix.com) for
 the version 3.61 BIOS signature.

 Thanks for Mark Singer (elf@netcom.com) and Richard Simpson
 (rsimpson@ewrcsdra.demon.co.uk) for more Quantum signatures and detective
 work on the Quantum RAM layout.

 Special thanks to James T. McKinley (mckinley@msupa.pa.msu.edu) for
 providing patches for proper PCI BIOS32-mediated detection of the TMC-3260
 card (a PCI bus card with the 36C70 chip).  Please send James PCI-related
 bug reports.

 Thanks to Tom Cavin (tec@usa1.com) for preliminary command-line option
 patches.
 
 All of the alpha testers deserve much thanks.



 NOTES ON USER DEFINABLE OPTIONS:

 DEBUG: This turns on the printing of various debug information.

 ENABLE_PARITY: This turns on SCSI parity checking.  With the current
 driver, all attached devices must support SCSI parity.  If none of your
 devices support parity, then you can probably get the driver to work by
 turning this option off.  I have no way of testing this, however, and it
 would appear that no one ever uses this option.

 FIFO_COUNT: The host adapter has an 8K cache (host adapters based on the
 18C30 chip have a 2k cache).  When this many 512 byte blocks are filled by
 the SCSI device, an interrupt will be raised.  Therefore, this could be as
 low as 0, or as high as 16.  Note, however, that values which are too high
 or too low seem to prevent any interrupts from occurring, and thereby lock
 up the machine.  I have found that 2 is a good number, but throughput may
 be increased by changing this value to values which are close to 2.
 Please let me know if you try any different values.

 DO_DETECT: This activates some old scan code which was needed before the
 high level drivers got fixed.  If you are having trouble with the driver,
 turning this on should not hurt, and might help.  Please let me know if
 this is the case, since this code will be removed from future drivers.

 RESELECTION: This is no longer an option, since I gave up trying to
 implement it in version 4.x of this driver.  It did not improve
 performance at all and made the driver unstable (because I never found one
 of the two race conditions which were introduced by the multiple
 outstanding command code).  The instability seems a very high price to pay
 just so that you don't have to wait for the tape to rewind.  If you want
 this feature implemented, send me patches.  I'll be happy to send a copy
 of my (broken) driver to anyone who would like to see a copy.

 **************************************************************************/

#ifdef PCMCIA
#define MODULE
#endif

#ifdef MODULE
#include <linux/module.h>
#endif

#ifdef PCMCIA
#undef MODULE
#endif

#include <linux/sched.h>
#include <asm/io.h>
#include <linux/blk.h>
#include "scsi.h"
#include "hosts.h"
#include "fdomain.h"
#include <asm/system.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/ioport.h>
#include <linux/proc_fs.h>
#include <linux/bios32.h>
#include <linux/pci.h>
#include <linux/stat.h>

#include <linux/config.h>	/* for CONFIG_PCI */

struct proc_dir_entry proc_scsi_fdomain = {
    PROC_SCSI_FDOMAIN, 7, "fdomain",
    S_IFDIR | S_IRUGO | S_IXUGO, 2
};
  
#define VERSION          "$Revision: 1.1.1.1 $"

/* START OF USER DEFINABLE OPTIONS */

#define DEBUG            1	/* Enable debugging output */
#define ENABLE_PARITY    1	/* Enable SCSI Parity */
#define FIFO_COUNT       2	/* Number of 512 byte blocks before INTR */
#define DO_DETECT        0	/* Do device detection here (see scsi.c) */

/* END OF USER DEFINABLE OPTIONS */

#if DEBUG
#define EVERY_ACCESS     0	/* Write a line on every scsi access */
#define ERRORS_ONLY      1	/* Only write a line if there is an error */
#define DEBUG_DETECT     0	/* Debug fdomain_16x0_detect() */
#define DEBUG_MESSAGES   1	/* Debug MESSAGE IN phase */
#define DEBUG_ABORT      1	/* Debug abort() routine */
#define DEBUG_RESET      1	/* Debug reset() routine */
#define DEBUG_RACE       1      /* Debug interrupt-driven race condition */
#else
#define EVERY_ACCESS     0	/* LEAVE THESE ALONE--CHANGE THE ONES ABOVE */
#define ERRORS_ONLY      0
#define DEBUG_DETECT     0
#define DEBUG_MESSAGES   0
#define DEBUG_ABORT      0
#define DEBUG_RESET      0
#define DEBUG_RACE       0
#endif

/* Errors are reported on the line, so we don't need to report them again */
#if EVERY_ACCESS
#undef ERRORS_ONLY
#define ERRORS_ONLY      0
#endif

#if ENABLE_PARITY
#define PARITY_MASK      0x08
#else
#define PARITY_MASK      0x00
#endif

enum chip_type {
   unknown          = 0x00,
   tmc1800          = 0x01,
   tmc18c50         = 0x02,
   tmc18c30         = 0x03,
};

enum {
   in_arbitration   = 0x02,
   in_selection     = 0x04,
   in_other         = 0x08,
   disconnect       = 0x10,
   aborted          = 0x20,
   sent_ident       = 0x40,
};

enum in_port_type {
   Read_SCSI_Data   =  0,
   SCSI_Status      =  1,
   TMC_Status       =  2,
   FIFO_Status      =  3,	/* tmc18c50/tmc18c30 only */
   Interrupt_Cond   =  4,	/* tmc18c50/tmc18c30 only */
   LSB_ID_Code      =  5,
   MSB_ID_Code      =  6,
   Read_Loopback    =  7,
   SCSI_Data_NoACK  =  8,
   Interrupt_Status =  9,
   Configuration1   = 10,
   Configuration2   = 11,	/* tmc18c50/tmc18c30 only */
   Read_FIFO        = 12,
   FIFO_Data_Count  = 14
};

enum out_port_type {
   Write_SCSI_Data  =  0,
   SCSI_Cntl        =  1,
   Interrupt_Cntl   =  2,
   SCSI_Mode_Cntl   =  3,
   TMC_Cntl         =  4,
   Memory_Cntl      =  5,	/* tmc18c50/tmc18c30 only */
   Write_Loopback   =  7,
   IO_Control       = 11,	/* tmc18c30 only */
   Write_FIFO       = 12
};

static int               port_base         = 0;
static void              *bios_base        = NULL;
static int               bios_major        = 0;
static int               bios_minor        = 0;
static int               PCI_bus           = 0;
static int               Quantum           = 0;	/* Quantum board variant */
static int               interrupt_level   = 0;
static volatile int      in_command        = 0;
static Scsi_Cmnd         *current_SC       = NULL;
static enum chip_type    chip              = unknown;
static int               adapter_mask      = 0;
static int               this_id           = 0;
static int               setup_called      = 0;

#if DEBUG_RACE
static volatile int      in_interrupt_flag = 0;
#endif

static int               SCSI_Mode_Cntl_port;
static int               FIFO_Data_Count_port;
static int               Interrupt_Cntl_port;
static int               Interrupt_Status_port;
static int               Read_FIFO_port;
static int               Read_SCSI_Data_port;
static int               SCSI_Cntl_port;
static int               SCSI_Data_NoACK_port;
static int               SCSI_Status_port;
static int               TMC_Cntl_port;
static int               TMC_Status_port;
static int               Write_FIFO_port;
static int               Write_SCSI_Data_port;

static int               FIFO_Size = 0x2000; /* 8k FIFO for
						pre-tmc18c30 chips */

extern void              fdomain_16x0_intr( int irq, void *dev_id, struct pt_regs * regs );

static void *addresses[] = {
   (void *)0xc8000,
   (void *)0xca000,
   (void *)0xce000,
   (void *)0xde000,
   (void *)0xcc000,		/* Extra addresses for PCI boards */
   (void *)0xd0000,
   (void *)0xe0000,
};
#define ADDRESS_COUNT (sizeof( addresses ) / sizeof( unsigned ))
		       
static unsigned short ports[] = { 0x140, 0x150, 0x160, 0x170 };
#define PORT_COUNT (sizeof( ports ) / sizeof( unsigned short ))

static unsigned short ints[] = { 3, 5, 10, 11, 12, 14, 15, 0 };

/*

  READ THIS BEFORE YOU ADD A SIGNATURE!

  READING THIS SHORT NOTE CAN SAVE YOU LOTS OF TIME!

  READ EVERY WORD, ESPECIALLY THE WORD *NOT*

  This driver works *ONLY* for Future Domain cards using the TMC-1800,
  TMC-18C50, or TMC-18C30 chip.  This includes models TMC-1650, 1660, 1670,
  and 1680.

  The following BIOS signature signatures are for boards which do *NOT*
  work with this driver (these TMC-8xx and TMC-9xx boards may work with the
  Seagate driver):

  FUTURE DOMAIN CORP. (C) 1986-1988 V4.0I 03/16/88
  FUTURE DOMAIN CORP. (C) 1986-1989 V5.0C2/14/89
  FUTURE DOMAIN CORP. (C) 1986-1989 V6.0A7/28/89
  FUTURE DOMAIN CORP. (C) 1986-1990 V6.0105/31/90
  FUTURE DOMAIN CORP. (C) 1986-1990 V6.0209/18/90
  FUTURE DOMAIN CORP. (C) 1986-1990 V7.009/18/90
  FUTURE DOMAIN CORP. (C) 1992 V8.00.004/02/92

*/

struct signature {
   const char *signature;
   int  sig_offset;
   int  sig_length;
   int  major_bios_version;
   int  minor_bios_version;
   int  flag; /* 1 == PCI_bus, 2 == ISA_200S, 3 == ISA_250MG, 4 == ISA_200S */
} signatures[] = {
   /*          1         2         3         4         5         6 */
   /* 123456789012345678901234567890123456789012345678901234567890 */
   { "FUTURE DOMAIN CORP. (C) 1986-1990 1800-V2.07/28/89",  5, 50,  2,  0, 0 },
   { "FUTURE DOMAIN CORP. (C) 1986-1990 1800-V1.07/28/89",  5, 50,  2,  0, 0 },
   { "FUTURE DOMAIN CORP. (C) 1986-1990 1800-V2.07/28/89", 72, 50,  2,  0, 2 },
   { "FUTURE DOMAIN CORP. (C) 1986-1990 1800-V2.0",        73, 43,  2,  0, 3 },
   { "FUTURE DOMAIN CORP. (C) 1991 1800-V2.0.",            72, 39,  2,  0, 4 },
   { "FUTURE DOMAIN CORP. (C) 1992 V3.00.004/02/92",        5, 44,  3,  0, 0 },
   { "FUTURE DOMAIN TMC-18XX (C) 1993 V3.203/12/93",        5, 44,  3,  2, 0 },
   { "IBM F1 P2 BIOS v1.0104/29/93",                        5, 28,  3, -1, 0 },
   { "Future Domain Corp. V1.0008/18/93",                   5, 33,  3,  4, 0 },
   { "Future Domain Corp. V1.0008/18/93",                  26, 33,  3,  4, 1 },
   { "Adaptec AHA-2920 PCI-SCSI Card",                     42, 31,  3, -1, 1 },
   { "IBM F1 P264/32",                                      5, 14,  3, -1, 1 },
				/* This next signature may not be a 3.5 bios */
   { "Future Domain Corp. V2.0108/18/93",                   5, 33,  3,  5, 0 },
   { "FUTURE DOMAIN CORP.  V3.5008/18/93",                  5, 34,  3,  5, 0 },
   { "FUTURE DOMAIN 18c30/18c50/1800 (C) 1994 V3.5",        5, 44,  3,  5, 0 },
   { "FUTURE DOMAIN CORP.  V3.6008/18/93",                  5, 34,  3,  6, 0 },
   { "FUTURE DOMAIN CORP.  V3.6108/18/93",                  5, 34,  3,  6, 0 },
   { "FUTURE DOMAIN TMC-18XX",                              5, 22, -1, -1, 0 },

   /* READ NOTICE ABOVE *BEFORE* YOU WASTE YOUR TIME ADDING A SIGNATURE
    Also, fix the disk geometry code for your signature and send your
    changes for faith@cs.unc.edu.  Above all, do *NOT* change any old
    signatures!

    Note that the last line will match a "generic" 18XX bios.  Because
    Future Domain has changed the host SCSI ID and/or the location of the
    geometry information in the on-board RAM area for each of the first
    three BIOS's, it is still important to enter a fully qualified
    signature in the table for any new BIOS's (after the host SCSI ID and
    geometry location are verified). */
};

#define SIGNATURE_COUNT (sizeof( signatures ) / sizeof( struct signature ))

static void print_banner( struct Scsi_Host *shpnt )
{
   if (!shpnt) return;		/* This won't ever happen */

   if (bios_major < 0 && bios_minor < 0) {
      printk( "scsi%d <fdomain>: No BIOS; using scsi id %d\n",
	      shpnt->host_no, shpnt->this_id );
   } else {
      printk( "scsi%d <fdomain>: BIOS version ", shpnt->host_no );

      if (bios_major >= 0) printk( "%d.", bios_major );
      else                 printk( "?." );

      if (bios_minor >= 0) printk( "%d", bios_minor );
      else                 printk( "?." );
   
      printk( " at 0x%x using scsi id %d\n",
	      (unsigned)bios_base, shpnt->this_id );
   }

				/* If this driver works for later FD PCI
				   boards, we will have to modify banner
				   for additional PCI cards, but for now if
				   it's PCI it's a TMC-3260 - JTM */
   printk( "scsi%d <fdomain>: %s chip at 0x%x irq ",
	   shpnt->host_no,
	   chip == tmc1800 ? "TMC-1800"
	   : (chip == tmc18c50 ? "TMC-18C50"
	      : (chip == tmc18c30 ?
		 (PCI_bus ? "TMC-36C70 (PCI bus)" : "TMC-18C30")
		 : "Unknown")),
	   port_base );

   if (interrupt_level) printk( "%d", interrupt_level );
   else                 printk( "<none>" );

   printk( "\n" );
}

void fdomain_setup( char *str, int *ints )
{
   if (setup_called++ || ints[0] < 2 || ints[0] > 3) {
      printk( "fdomain: usage: fdomain=<PORT_BASE>,<IRQ>[,<ADAPTER_ID>]\n" );
      printk( "fdomain: bad LILO parameters?\n" );
   }

   port_base       = ints[0] >= 1 ? ints[1] : 0;
   interrupt_level = ints[0] >= 2 ? ints[2] : 0;
   this_id         = ints[0] >= 3 ? ints[3] : 0;
   
   bios_major = bios_minor = -1; /* Use geometry for BIOS version >= 3.4 */
}


static void do_pause( unsigned amount )	/* Pause for amount*10 milliseconds */
{
   unsigned long the_time = jiffies + amount; /* 0.01 seconds per jiffy */

   while (jiffies < the_time);
}

inline static void fdomain_make_bus_idle( void )
{
   outb( 0, SCSI_Cntl_port );
   outb( 0, SCSI_Mode_Cntl_port );
   if (chip == tmc18c50 || chip == tmc18c30)
	 outb( 0x21 | PARITY_MASK, TMC_Cntl_port ); /* Clear forced intr. */
   else
	 outb( 0x01 | PARITY_MASK, TMC_Cntl_port );
}

static int fdomain_is_valid_port( int port )
{
#if DEBUG_DETECT 
   printk( " (%x%x),",
	   inb( port + MSB_ID_Code ), inb( port + LSB_ID_Code ) );
#endif

   /* The MCA ID is a unique id for each MCA compatible board.  We
      are using ISA boards, but Future Domain provides the MCA ID
      anyway.  We can use this ID to ensure that this is a Future
      Domain TMC-1660/TMC-1680.
    */

   if (inb( port + LSB_ID_Code ) != 0xe9) { /* test for 0x6127 id */
      if (inb( port + LSB_ID_Code ) != 0x27) return 0;
      if (inb( port + MSB_ID_Code ) != 0x61) return 0;
      chip = tmc1800;
   } else {				    /* test for 0xe960 id */
      if (inb( port + MSB_ID_Code ) != 0x60) return 0;
      chip = tmc18c50;

#if 0

				/* Try to toggle 32-bit mode.  This only
				   works on an 18c30 chip.  (User reports
				   say this works, so we should switch to
				   it in the near future.) */

      outb( 0x80, port + IO_Control );
      if ((inb( port + Configuration2 ) & 0x80) == 0x80) {
	 outb( 0x00, port + IO_Control );
	 if ((inb( port + Configuration2 ) & 0x80) == 0x00) {
	    chip = tmc18c30;
	    FIFO_Size = 0x800;	/* 2k FIFO */
	 }
      }
#else

				/* That should have worked, but appears to
				   have problems.  Let's assume it is an
				   18c30 if the RAM is disabled. */

      if (inb( port + Configuration2 ) & 0x02) {
	 chip      = tmc18c30;
	 FIFO_Size = 0x800;	/* 2k FIFO */
      }
#endif
				/* If that failed, we are an 18c50. */
   }

   return 1;
}

static int fdomain_test_loopback( void )
{
   int i;
   int result;

   for (i = 0; i < 255; i++) {
      outb( i, port_base + Write_Loopback );
      result = inb( port_base + Read_Loopback );
      if (i != result)
	    return 1;
   }
   return 0;
}

/* fdomain_get_irq assumes that we have a valid MCA ID for a
   TMC-1660/TMC-1680 Future Domain board.  Now, check to be sure the
   bios_base matches these ports.  If someone was unlucky enough to have
   purchased more than one Future Domain board, then they will have to
   modify this code, as we only detect one board here.  [The one with the
   lowest bios_base.]

   Note that this routine is only used for systems without a PCI BIOS32
   (e.g., ISA bus).  For PCI bus systems, this routine will likely fail
   unless one of the IRQs listed in the ints array is used by the board.
   Sometimes it is possible to use the computer's BIOS setup screen to
   configure a PCI system so that one of these IRQs will be used by the
   Future Domain card. */

static int fdomain_get_irq( int base )
{
   int options = inb( base + Configuration1 );

#if DEBUG_DETECT
   printk( " Options = %x\n", options );
#endif
   
				/* Check for board with lowest bios_base --
				   this isn't valid for the 18c30 or for
				   boards on the PCI bus, so just assume we
				   have the right board. */

   if (chip != tmc18c30
       && !PCI_bus
       && addresses[ (options & 0xc0) >> 6 ] != bios_base) return 0;

   return ints[ (options & 0x0e) >> 1 ];
}

static int fdomain_isa_detect( int *irq, int *iobase )
{
   int i;
   int base;
   int flag = 0;

   if (bios_major == 2) {
      /* The TMC-1660/TMC-1680 has a RAM area just after the BIOS ROM.
	 Assuming the ROM is enabled (otherwise we wouldn't have been
	 able to read the ROM signature :-), then the ROM sets up the
	 RAM area with some magic numbers, such as a list of port
	 base addresses and a list of the disk "geometry" reported to
	 DOS (this geometry has nothing to do with physical geometry).
       */

      switch (Quantum) {
      case 2:			/* ISA_200S */
      case 3:			/* ISA_250MG */
	 base = *((char *)bios_base + 0x1fa2)
	       + (*((char *)bios_base + 0x1fa3) << 8);
	 break;
      case 4:			/* ISA_200S (another one) */
	 base = *((char *)bios_base + 0x1fa3)
	       + (*((char *)bios_base + 0x1fa4) << 8);
	 break;
      default:
	 base = *((char *)bios_base + 0x1fcc)
	       + (*((char *)bios_base + 0x1fcd) << 8);
	 break;
      }
   
#if DEBUG_DETECT
      printk( " %x,", base );
#endif

      for (flag = 0, i = 0; !flag && i < PORT_COUNT; i++) {
	 if (base == ports[i])
	       ++flag;
      }

      if (flag && fdomain_is_valid_port( base )) {
	 *irq    = fdomain_get_irq( base );
	 *iobase = base;
	 return 1;
      }
      
      /* This is a bad sign.  It usually means that someone patched the
	 BIOS signature list (the signatures variable) to contain a BIOS
	 signature for a board *OTHER THAN* the TMC-1660/TMC-1680. */
      
#if DEBUG_DETECT
      printk( " RAM FAILED, " );
#endif
   }

   /* Anyway, the alternative to finding the address in the RAM is to just
      search through every possible port address for one that is attached
      to the Future Domain card.  Don't panic, though, about reading all
      these random port addresses -- there are rumors that the Future
      Domain BIOS does something very similar.

      Do not, however, check ports which the kernel knows are being used by
      another driver. */

   for (i = 0; i < PORT_COUNT; i++) {
      base = ports[i];
      if (check_region( base, 0x10 )) {
#if DEBUG_DETECT
	 printk( " (%x inuse),", base );
#endif
	 continue;
      }
#if DEBUG_DETECT
      printk( " %x,", base );
#endif
      if ((flag = fdomain_is_valid_port( base ))) break;
   }

   if (!flag) return 0;		/* iobase not found */

   *irq    = fdomain_get_irq( base );
   *iobase = base;

   return 1;			/* success */
}

static int fdomain_pci_nobios_detect( int *irq, int *iobase )
{
   int i;
   int flag = 0;

   /* The proper way of doing this is to use ask the PCI bus for the device
      IRQ and interrupt level.  But we can't do that if PCI BIOS32 support
      isn't compiled into the kernel, or if a PCI BIOS32 isn't present.

      Instead, we scan down a bunch of addresses (Future Domain tech
      support says we will probably find the address before we get to
      0xf800).  This works fine on some systems -- other systems may have
      to scan more addresses.  If you have to modify this section for your
      installation, please send mail to faith@cs.unc.edu. */

   for (i = 0xfff8; i > 0xe000; i -= 8) {
      if (check_region( i, 0x10 )) {
#if DEBUG_DETECT
	 printk( " (%x inuse)," , i );
#endif
	 continue;
      }
      if ((flag = fdomain_is_valid_port( i ))) break;
   }

   if (!flag) return 0;		/* iobase not found */

   *irq    = fdomain_get_irq( i );
   *iobase = i;

   return 1;			/* success */
}

/* PCI detection function: int fdomain_pci_bios_detect(int* irq, int*
   iobase) This function gets the Interrupt Level and I/O base address from
   the PCI configuration registers.  The I/O base address is masked with
   0xfff8 since on my card the address read from the PCI config registers
   is off by one from the actual I/O base address necessary for accessing
   the status and control registers on the card (PCI config register gives
   0xf801, actual address is 0xf800).  This is likely a bug in the FD
   config code that writes to the PCI registers, however using a mask
   should be safe since I think the scan done by the card to determine the
   I/O base is done in increments of 8 (i.e., 0xf800, 0xf808, ...), at
   least the old scan code we used to use to get the I/O base did...  Also,
   the device ID from the PCI config registers is 0x0 and should be 0x60e9
   as it is in the status registers (offset 5 from I/O base).  If this is
   changed in future hardware/BIOS changes it will need to be fixed in this
   detection function.  Comments, bug reports, etc... on this function
   should be sent to mckinley@msupa.pa.msu.edu - James T. McKinley.  */

#ifdef CONFIG_PCI
static int fdomain_pci_bios_detect( int *irq, int *iobase )
{
   int              error;
   unsigned char    pci_bus, pci_dev_fn;    /* PCI bus & device function */
   unsigned char    pci_irq;                /* PCI interrupt line */
   unsigned int     pci_base;               /* PCI I/O base address */
   unsigned short   pci_vendor, pci_device; /* PCI vendor & device IDs */

   /* If the PCI BIOS doesn't exist, use the old-style detection routines.
      Otherwise, get the I/O base address and interrupt from the PCI config
      registers. */
   
   if (!pcibios_present()) return fdomain_pci_nobios_detect( irq, iobase );

#if DEBUG_DETECT
   /* Tell how to print a list of the known PCI devices from bios32 and
      list vendor and device IDs being used if in debug mode.  */
      
   printk( "\nINFO: cat /proc/pci to see list of PCI devices from bios32\n" );
   printk( "\nTMC-3260 detect:"
	   " Using PCI Vendor ID: 0x%x, PCI Device ID: 0x%x\n",
	   PCI_VENDOR_ID_FD, 
	   PCI_DEVICE_ID_FD_36C70 );
#endif 

   /* We will have to change this if more than 1 PCI bus is present and the
      FD scsi host is not on the first bus (i.e., a PCI to PCI bridge,
      which is not supported by bios32 right now anyway).  This should
      probably be done by a call to pcibios_find_device but I can't get it
      to work...  Also the device ID reported from the PCI config registers
      does not match the device ID quoted in the tech manual or available
      from offset 5 from the I/O base address.  It should be 0x60E9, but it
      is 0x0 if read from the PCI config registers.  I guess the FD folks
      neglected to write it to the PCI registers...  This loop is necessary
      to get the device function (at least until someone can get
      pcibios_find_device to work, I cannot but 53c7,8xx.c uses it...). */
    
   pci_bus = 0;

   for (pci_dev_fn = 0x0; pci_dev_fn < 0xff; pci_dev_fn++) {
      pcibios_read_config_word( pci_bus,
				pci_dev_fn,
				PCI_VENDOR_ID,
				&pci_vendor );

      if (pci_vendor == PCI_VENDOR_ID_FD) {
	 pcibios_read_config_word( pci_bus,
				   pci_dev_fn,
				   PCI_DEVICE_ID,
				   &pci_device );

	 if (pci_device == PCI_DEVICE_ID_FD_36C70) {
	    /* Break out once we have the correct device.  If other FD
	       PCI devices are added to this driver we will need to add
	       an or of the other PCI_DEVICE_ID_FD_XXXXX's here. */
	    break;
	 } else {
	    /* If we can't find an FD scsi card we give up. */
	    return 0;
	 }
      }
   }
       
#if DEBUG_DETECT
   printk( "Future Domain 36C70 : at PCI bus %u, device %u, function %u\n",
	   pci_bus,
	   (pci_dev_fn & 0xf8) >> 3, 
	   pci_dev_fn & 7 );
#endif

   /* We now have the appropriate device function for the FD board so we
      just read the PCI config info from the registers.  */

   if ((error = pcibios_read_config_dword( pci_bus,
					   pci_dev_fn, 
					   PCI_BASE_ADDRESS_0,
					   &pci_base ))
       || (error = pcibios_read_config_byte( pci_bus,
					     pci_dev_fn, 
					     PCI_INTERRUPT_LINE,
					     &pci_irq ))) {
      printk ( "PCI ERROR: Future Domain 36C70 not initializing"
	       " due to error reading configuration space\n" );
      return 0;
   } else {
#if DEBUG_DETECT
      printk( "TMC-3260 PCI: IRQ = %u, I/O base = 0x%lx\n", 
	      pci_irq, pci_base );
#endif

      /* Now we have the I/O base address and interrupt from the PCI
	 configuration registers.  Unfortunately it seems that the I/O base
	 address is off by one on my card so I mask it with 0xfff8.  This
	 must be some kind of goof in the FD code that does the autoconfig
	 and writes to the PCI registers (or maybe I just don't understand
	 something).  If they fix it in later versions of the card or BIOS
	 we may have to adjust the address based on the signature or
	 something...  */

      *irq    = pci_irq;
      *iobase = (pci_base & 0xfff8);

#if DEBUG_DETECT
      printk( "TMC-3260 fix: Masking I/O base address with 0xff00.\n" ); 
      printk( "TMC-3260: IRQ = %d, I/O base = 0x%x\n", *irq, *iobase );
#endif

      if (!fdomain_is_valid_port( *iobase )) return 0;
      return 1;
   }
   return 0;
}
#endif

int fdomain_16x0_detect( Scsi_Host_Template *tpnt )
{
   int              i, j;
   int              retcode;
   struct Scsi_Host *shpnt;
#if DO_DETECT
   const int        buflen = 255;
   Scsi_Cmnd        SCinit;
   unsigned char    do_inquiry[] =       { INQUIRY, 0, 0, 0, buflen, 0 };
   unsigned char    do_request_sense[] = { REQUEST_SENSE, 0, 0, 0, buflen, 0 };
   unsigned char    do_read_capacity[] = { READ_CAPACITY,
					   0, 0, 0, 0, 0, 0, 0, 0, 0 };
   unsigned char    buf[buflen];
#endif

#if DEBUG_DETECT
   printk( "fdomain_16x0_detect()," );
#endif
   tpnt->proc_dir = &proc_scsi_fdomain;

   if (setup_called) {
#if DEBUG_DETECT
      printk( "no BIOS, using port_base = 0x%x, irq = %d\n",
	      port_base, interrupt_level );
#endif
      if (!fdomain_is_valid_port( port_base )) {
	 printk( "fdomain: cannot locate chip at port base 0x%x\n",
		 port_base );
	 printk( "fdomain: bad LILO parameters?\n" );
	 return 0;
      }
   } else {
      int flag = 0;
      
      for (i = 0; !bios_base && i < ADDRESS_COUNT; i++) {
#if DEBUG_DETECT
	 printk( " %x(%x),", (unsigned)addresses[i], (unsigned)bios_base );
#endif
	 for (j = 0; !bios_base && j < SIGNATURE_COUNT; j++) {
	    if (!memcmp( ((char *)addresses[i] + signatures[j].sig_offset),
			 signatures[j].signature, signatures[j].sig_length )) {
	       bios_major = signatures[j].major_bios_version;
	       bios_minor = signatures[j].minor_bios_version;
	       PCI_bus    = (signatures[j].flag == 1);
	       Quantum    = (signatures[j].flag > 1) ? signatures[j].flag : 0;
	       bios_base  = addresses[i];
	    }
	 }
      }

      if (!bios_base) {
#if DEBUG_DETECT
	 printk( " FAILED: NO BIOS\n" );
#endif
	 return 0;
      }

      if (!PCI_bus) {
	 flag = fdomain_isa_detect( &interrupt_level, &port_base );
      } else {
#ifdef CONFIG_PCI
	 flag = fdomain_pci_bios_detect( &interrupt_level, &port_base );
#else
	 flag = fdomain_pci_nobios_detect( &interrupt_level, &port_base );
#endif
      }
	 
      if (!flag) {
#if DEBUG_DETECT
	 printk( " FAILED: NO PORT\n" );
#endif
#ifdef CONFIG_PCI
	 printk( "\nTMC-3260 36C70 PCI scsi chip detection failed.\n" );
	 printk( "Send mail to mckinley@msupa.pa.msu.edu.\n" );
#endif
	 return 0;		/* Cannot find valid set of ports */
      }
   }

   SCSI_Mode_Cntl_port   = port_base + SCSI_Mode_Cntl;
   FIFO_Data_Count_port  = port_base + FIFO_Data_Count;
   Interrupt_Cntl_port   = port_base + Interrupt_Cntl;
   Interrupt_Status_port = port_base + Interrupt_Status;
   Read_FIFO_port        = port_base + Read_FIFO;
   Read_SCSI_Data_port   = port_base + Read_SCSI_Data;
   SCSI_Cntl_port        = port_base + SCSI_Cntl;
   SCSI_Data_NoACK_port  = port_base + SCSI_Data_NoACK;
   SCSI_Status_port      = port_base + SCSI_Status;
   TMC_Cntl_port         = port_base + TMC_Cntl;
   TMC_Status_port       = port_base + TMC_Status;
   Write_FIFO_port       = port_base + Write_FIFO;
   Write_SCSI_Data_port  = port_base + Write_SCSI_Data;

   fdomain_16x0_reset( NULL, 0 );

   if (fdomain_test_loopback()) {
#if DEBUG_DETECT
      printk( "fdomain: LOOPBACK TEST FAILED, FAILING DETECT!\n" );
#endif
      if (setup_called) {
	 printk( "fdomain: loopback test failed at port base 0x%x\n",
		 port_base );
	 printk( "fdomain: bad LILO parameters?\n" );
      }
      return 0;
   }

   if (this_id) {
      tpnt->this_id = (this_id & 0x07);
      adapter_mask  = (1 << tpnt->this_id);
   } else {
      if (PCI_bus || (bios_major == 3 && bios_minor >= 2) || bios_major < 0) {
	 tpnt->this_id = 7;
	 adapter_mask  = 0x80;
      } else {
	 tpnt->this_id = 6;
	 adapter_mask  = 0x40;
      }
   }

				/* Print out a banner here in case we can't
				   get resources.  */

   shpnt = scsi_register( tpnt, 0 );
   shpnt->irq = interrupt_level;
   shpnt->io_port = port_base;
   shpnt->n_io_port = 0x10;
   print_banner( shpnt );

				/* Log IRQ with kernel */   
   if (!interrupt_level) {
      panic( "fdomain: *NO* interrupt level selected!\n" );
   } else {
      /* Register the IRQ with the kernel */

      retcode = request_irq( interrupt_level,
			     fdomain_16x0_intr, SA_INTERRUPT, "fdomain", NULL);

      if (retcode < 0) {
	 if (retcode == -EINVAL) {
	    printk( "fdomain: IRQ %d is bad!\n", interrupt_level );
	    printk( "         This shouldn't happen!\n" );
	    printk( "         Send mail to faith@cs.unc.edu\n" );
	 } else if (retcode == -EBUSY) {
	    printk( "fdomain: IRQ %d is already in use!\n", interrupt_level );
	    printk( "         Please use another IRQ!\n" );
	 } else {
	    printk( "fdomain: Error getting IRQ %d\n", interrupt_level );
	    printk( "         This shouldn't happen!\n" );
	    printk( "         Send mail to faith@cs.unc.edu\n" );
	 }
	 panic( "fdomain: Driver requires interruptions\n" );
      }
   }

				/* Log I/O ports with kernel */
   request_region( port_base, 0x10, "fdomain" );

#if DO_DETECT

   /* These routines are here because of the way the SCSI bus behaves after
      a reset.  This appropriate behavior was not handled correctly by the
      higher level SCSI routines when I first wrote this driver.  Now,
      however, correct scan routines are part of scsi.c and these routines
      are no longer needed.  However, this code is still good for
      debugging.  */

   SCinit.request_buffer  = SCinit.buffer = buf;
   SCinit.request_bufflen = SCinit.bufflen = sizeof(buf)-1;
   SCinit.use_sg          = 0;
   SCinit.lun             = 0;

   printk( "fdomain: detection routine scanning for devices:\n" );
   for (i = 0; i < 8; i++) {
      SCinit.target = i;
      if (i == tpnt->this_id)	/* Skip host adapter */
	    continue;
      memcpy(SCinit.cmnd, do_request_sense, sizeof(do_request_sense));
      retcode = fdomain_16x0_command(&SCinit);
      if (!retcode) {
	 memcpy(SCinit.cmnd, do_inquiry, sizeof(do_inquiry));
	 retcode = fdomain_16x0_command(&SCinit);
	 if (!retcode) {
	    printk( "     SCSI ID %d: ", i );
	    for (j = 8; j < (buf[4] < 32 ? buf[4] : 32); j++)
		  printk( "%c", buf[j] >= 20 ? buf[j] : ' ' );
	    memcpy(SCinit.cmnd, do_read_capacity, sizeof(do_read_capacity));
	    retcode = fdomain_16x0_command(&SCinit);
	    if (!retcode) {
	       unsigned long blocks, size, capacity;
	       
	       blocks = (buf[0] << 24) | (buf[1] << 16)
		     | (buf[2] << 8) | buf[3];
	       size = (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];
	       capacity = +( +(blocks / 1024L) * +(size * 10L)) / 1024L;
	       
	       printk( "%lu MB (%lu byte blocks)",
		       ((capacity + 5L) / 10L), size );
	    } else {
	       memcpy(SCinit.cmnd, do_request_sense, sizeof(do_request_sense));
	       retcode = fdomain_16x0_command(&SCinit);
	    }
	    printk ("\n" );
	 } else {
	    memcpy(SCinit.cmnd, do_request_sense, sizeof(do_request_sense));
	    retcode = fdomain_16x0_command(&SCinit);
	 }
      }
   }
#endif

   return 1;			/* Maximum of one adapter will be detected. */
}

const char *fdomain_16x0_info( struct Scsi_Host *ignore )
{
   static char buffer[80];
   char        *pt;
   
   strcpy( buffer, "Future Domain TMC-16x0 SCSI driver, version" );
   if (strchr( VERSION, ':')) { /* Assume VERSION is an RCS Revision string */
      strcat( buffer, strchr( VERSION, ':' ) + 1 );
      pt = strrchr( buffer, '$') - 1;
      if (!pt)  		/* Stripped RCS Revision string? */
	    pt = buffer + strlen( buffer ) - 1;
      if (*pt != ' ')
	    ++pt;
      *pt = '\0';
   } else {			/* Assume VERSION is a number */
      strcat( buffer, " " VERSION );
   }
      
   return buffer;
}

				/* First pass at /proc information routine. */
/*
 * inout : decides on the direction of the dataflow and the meaning of the 
 *         variables
 * buffer: If inout==FALSE data is being written to it else read from it
 * *start: If inout==FALSE start of the valid data in the buffer
 * offset: If inout==FALSE offset from the beginning of the imaginary file 
 *         from which we start writing into the buffer
 * length: If inout==FALSE max number of bytes to be written into the buffer 
 *         else number of bytes in the buffer
 */
int fdomain_16x0_proc_info( char *buffer, char **start, off_t offset,
			    int length, int hostno, int inout )
{
   const char *info = fdomain_16x0_info( NULL );
   int        len;
   int        pos;
   int        begin;

   if (inout) return(-ENOSYS);
    
   begin = 0;
   strcpy( buffer, info );
   strcat( buffer, "\n" );

   pos = len = strlen( buffer );

   if(pos < offset) {
      len = 0;
      begin = pos;
   }
    
   *start = buffer + (offset - begin);   /* Start of wanted data */
   len -= (offset - begin);
   if(len > length) len = length;
   
   return(len);
}
   
#if 0
static int fdomain_arbitrate( void )
{
   int           status = 0;
   unsigned long timeout;

#if EVERY_ACCESS
   printk( "fdomain_arbitrate()\n" );
#endif
   
   outb( 0x00, SCSI_Cntl_port );              /* Disable data drivers */
   outb( adapter_mask, port_base + SCSI_Data_NoACK ); /* Set our id bit */
   outb( 0x04 | PARITY_MASK, TMC_Cntl_port ); /* Start arbitration */

   timeout = jiffies + 50;		      /* 500 mS */
   while (jiffies < timeout) {
      status = inb( TMC_Status_port );        /* Read adapter status */
      if (status & 0x02)		      /* Arbitration complete */
	    return 0;	
   }

   /* Make bus idle */
   fdomain_make_bus_idle();

#if EVERY_ACCESS
   printk( "Arbitration failed, status = %x\n", status );
#endif
#if ERRORS_ONLY
   printk( "fdomain: Arbitration failed, status = %x\n", status );
#endif
   return 1;
}
#endif

static int fdomain_select( int target )
{
   int           status;
   unsigned long timeout;
   static int    flag = 0;


   outb( 0x82, SCSI_Cntl_port ); /* Bus Enable + Select */
   outb( adapter_mask | (1 << target), SCSI_Data_NoACK_port );

   /* Stop arbitration and enable parity */
   outb( PARITY_MASK, TMC_Cntl_port ); 

   timeout = jiffies + 35;		/* 350mS -- because of timeouts
					   (was 250mS) */

   while (jiffies < timeout) {
      status = inb( SCSI_Status_port ); /* Read adapter status */
      if (status & 1) {			/* Busy asserted */
	 /* Enable SCSI Bus (on error, should make bus idle with 0) */
	 outb( 0x80, SCSI_Cntl_port );
	 return 0;
      }
   }
   /* Make bus idle */
   fdomain_make_bus_idle();
#if EVERY_ACCESS
   if (!target) printk( "Selection failed\n" );
#endif
#if ERRORS_ONLY
   if (!target) {
      if (!flag) /* Skip first failure for all chips. */
	    ++flag;
      else
	    printk( "fdomain: Selection failed\n" );
   }
#endif
   return 1;
}

void my_done( int error )
{
   if (in_command) {
      in_command = 0;
      outb( 0x00, Interrupt_Cntl_port );
      fdomain_make_bus_idle();
      current_SC->result = error;
      if (current_SC->scsi_done)
	    current_SC->scsi_done( current_SC );
      else panic( "fdomain: current_SC->scsi_done() == NULL" );
   } else {
      panic( "fdomain: my_done() called outside of command\n" );
   }
#if DEBUG_RACE
   in_interrupt_flag = 0;
#endif
}

void fdomain_16x0_intr( int irq, void *dev_id, struct pt_regs * regs )
{
   int      status;
   int      done = 0;
   unsigned data_count;

				/* The fdomain_16x0_intr is only called via
				   the interrupt handler.  The goal of the
				   sti() here is to allow other
				   interruptions while this routine is
				   running. */

   sti();			/* Yes, we really want sti() here */
   
   outb( 0x00, Interrupt_Cntl_port );

   /* We usually have one spurious interrupt after each command.  Ignore it. */
   if (!in_command || !current_SC) {	/* Spurious interrupt */
#if EVERY_ACCESS
      printk( "Spurious interrupt, in_command = %d, current_SC = %x\n",
	      in_command, current_SC );
#endif
      return;
   }

   /* Abort calls my_done, so we do nothing here. */
   if (current_SC->SCp.phase & aborted) {
#if DEBUG_ABORT
      printk( "Interrupt after abort, ignoring\n" );
#endif
      /*
      return; */
   }

#if DEBUG_RACE
   ++in_interrupt_flag;
#endif

   if (current_SC->SCp.phase & in_arbitration) {
      status = inb( TMC_Status_port );        /* Read adapter status */
      if (!(status & 0x02)) {
#if EVERY_ACCESS
	 printk( " AFAIL " );
#endif
	 my_done( DID_BUS_BUSY << 16 );
	 return;
      }
      current_SC->SCp.phase = in_selection;
      
      outb( 0x40 | FIFO_COUNT, Interrupt_Cntl_port );

      outb( 0x82, SCSI_Cntl_port ); /* Bus Enable + Select */
      outb( adapter_mask | (1 << current_SC->target), SCSI_Data_NoACK_port );
      
      /* Stop arbitration and enable parity */
      outb( 0x10 | PARITY_MASK, TMC_Cntl_port );
#if DEBUG_RACE
      in_interrupt_flag = 0;
#endif
      return;
   } else if (current_SC->SCp.phase & in_selection) {
      status = inb( SCSI_Status_port );
      if (!(status & 0x01)) {
	 /* Try again, for slow devices */
	 if (fdomain_select( current_SC->target )) {
#if EVERY_ACCESS
	    printk( " SFAIL " );
#endif
	    my_done( DID_NO_CONNECT << 16 );
	    return;
	 } else {
#if EVERY_ACCESS
	    printk( " AltSel " );
#endif
	    /* Stop arbitration and enable parity */
	    outb( 0x10 | PARITY_MASK, TMC_Cntl_port );
	 }
      }
      current_SC->SCp.phase = in_other;
      outb( 0x90 | FIFO_COUNT, Interrupt_Cntl_port );
      outb( 0x80, SCSI_Cntl_port );
#if DEBUG_RACE
      in_interrupt_flag = 0;
#endif
      return;
   }
   
   /* current_SC->SCp.phase == in_other: this is the body of the routine */
   
   status = inb( SCSI_Status_port );
   
   if (status & 0x10) {	/* REQ */
      
      switch (status & 0x0e) {
       
      case 0x08:		/* COMMAND OUT */
	 outb( current_SC->cmnd[current_SC->SCp.sent_command++],
	       Write_SCSI_Data_port );
#if EVERY_ACCESS
	 printk( "CMD = %x,",
		 current_SC->cmnd[ current_SC->SCp.sent_command - 1] );
#endif
	 break;
      case 0x00:		/* DATA OUT -- tmc18c50/tmc18c30 only */
	 if (chip != tmc1800 && !current_SC->SCp.have_data_in) {
	    current_SC->SCp.have_data_in = -1;
	    outb( 0xd0 | PARITY_MASK, TMC_Cntl_port );
	 }
	 break;
      case 0x04:		/* DATA IN -- tmc18c50/tmc18c30 only */
	 if (chip != tmc1800 && !current_SC->SCp.have_data_in) {
	    current_SC->SCp.have_data_in = 1;
	    outb( 0x90 | PARITY_MASK, TMC_Cntl_port );
	 }
	 break;
      case 0x0c:		/* STATUS IN */
	 current_SC->SCp.Status = inb( Read_SCSI_Data_port );
#if EVERY_ACCESS
	 printk( "Status = %x, ", current_SC->SCp.Status );
#endif
#if ERRORS_ONLY
	 if (current_SC->SCp.Status
	     && current_SC->SCp.Status != 2
	     && current_SC->SCp.Status != 8) {
	    printk( "fdomain: target = %d, command = %x, status = %x\n",
		    current_SC->target,
		    current_SC->cmnd[0],
		    current_SC->SCp.Status );
	 }
#endif
	       break;
      case 0x0a:		/* MESSAGE OUT */
	 outb( MESSAGE_REJECT, Write_SCSI_Data_port ); /* Reject */
	 break;
      case 0x0e:		/* MESSAGE IN */
	 current_SC->SCp.Message = inb( Read_SCSI_Data_port );
#if EVERY_ACCESS
	 printk( "Message = %x, ", current_SC->SCp.Message );
#endif
	 if (!current_SC->SCp.Message) ++done;
#if DEBUG_MESSAGES || EVERY_ACCESS
	 if (current_SC->SCp.Message) {
	    printk( "fdomain: message = %x\n", current_SC->SCp.Message );
	 }
#endif
	 break;
      }
   }

   if (chip == tmc1800
       && !current_SC->SCp.have_data_in
       && (current_SC->SCp.sent_command
	   >= current_SC->cmd_len)) {
				/* We have to get the FIFO direction
				   correct, so I've made a table based
				   on the SCSI Standard of which commands
				   appear to require a DATA OUT phase.
				 */
      /*
	p. 94: Command for all device types
	CHANGE DEFINITION            40 DATA OUT
	COMPARE                      39 DATA OUT
	COPY                         18 DATA OUT
	COPY AND VERIFY              3a DATA OUT
	INQUIRY                      12 
	LOG SELECT                   4c DATA OUT
	LOG SENSE                    4d
	MODE SELECT (6)              15 DATA OUT
	MODE SELECT (10)             55 DATA OUT
	MODE SENSE (6)               1a
	MODE SENSE (10)              5a
	READ BUFFER                  3c
	RECEIVE DIAGNOSTIC RESULTS   1c
	REQUEST SENSE                03
	SEND DIAGNOSTIC              1d DATA OUT
	TEST UNIT READY              00
	WRITE BUFFER                 3b DATA OUT

	p.178: Commands for direct-access devices (not listed on p. 94)
	FORMAT UNIT                  04 DATA OUT
	LOCK-UNLOCK CACHE            36
	PRE-FETCH                    34
	PREVENT-ALLOW MEDIUM REMOVAL 1e
	READ (6)/RECEIVE             08
	READ (10)                    3c
	READ CAPACITY                25
	READ DEFECT DATA (10)        37
	READ LONG                    3e
	REASSIGN BLOCKS              07 DATA OUT
	RELEASE                      17
	RESERVE                      16 DATA OUT
	REZERO UNIT/REWIND           01
	SEARCH DATA EQUAL (10)       31 DATA OUT
	SEARCH DATA HIGH (10)        30 DATA OUT
	SEARCH DATA LOW (10)         32 DATA OUT
	SEEK (6)                     0b
	SEEK (10)                    2b
	SET LIMITS (10)              33
	START STOP UNIT              1b
	SYNCHRONIZE CACHE            35
	VERIFY (10)                  2f
	WRITE (6)/PRINT/SEND         0a DATA OUT
	WRITE (10)/SEND              2a DATA OUT
	WRITE AND VERIFY (10)        2e DATA OUT
	WRITE LONG                   3f DATA OUT
	WRITE SAME                   41 DATA OUT ?

	p. 261: Commands for sequential-access devices (not previously listed)
	ERASE                        19
	LOAD UNLOAD                  1b
	LOCATE                       2b
	READ BLOCK LIMITS            05
	READ POSITION                34
	READ REVERSE                 0f
	RECOVER BUFFERED DATA        14
	SPACE                        11
	WRITE FILEMARKS              10 ?

	p. 298: Commands for printer devices (not previously listed)
	****** NOT SUPPORTED BY THIS DRIVER, since 0b is SEEK (6) *****
	SLEW AND PRINT               0b DATA OUT  -- same as seek
	STOP PRINT                   1b
	SYNCHRONIZE BUFFER           10

	p. 315: Commands for processor devices (not previously listed)
	
	p. 321: Commands for write-once devices (not previously listed)
	MEDIUM SCAN                  38
	READ (12)                    a8
	SEARCH DATA EQUAL (12)       b1 DATA OUT
	SEARCH DATA HIGH (12)        b0 DATA OUT
	SEARCH DATA LOW (12)         b2 DATA OUT
	SET LIMITS (12)              b3
	VERIFY (12)                  af
	WRITE (12)                   aa DATA OUT
	WRITE AND VERIFY (12)        ae DATA OUT

	p. 332: Commands for CD-ROM devices (not previously listed)
	PAUSE/RESUME                 4b
	PLAY AUDIO (10)              45
	PLAY AUDIO (12)              a5
	PLAY AUDIO MSF               47
	PLAY TRACK RELATIVE (10)     49
	PLAY TRACK RELATIVE (12)     a9
	READ HEADER                  44
	READ SUB-CHANNEL             42
	READ TOC                     43

	p. 370: Commands for scanner devices (not previously listed)
	GET DATA BUFFER STATUS       34
	GET WINDOW                   25
	OBJECT POSITION              31
	SCAN                         1b
	SET WINDOW                   24 DATA OUT

	p. 391: Commands for optical memory devices (not listed)
	ERASE (10)                   2c
	ERASE (12)                   ac
	MEDIUM SCAN                  38 DATA OUT
	READ DEFECT DATA (12)        b7
	READ GENERATION              29
	READ UPDATED BLOCK           2d
	UPDATE BLOCK                 3d DATA OUT

	p. 419: Commands for medium changer devices (not listed)
	EXCHANGE MEDIUM              46
	INITIALIZE ELEMENT STATUS    07
	MOVE MEDIUM                  a5
	POSITION TO ELEMENT          2b
	READ ELEMENT STATUS          b8
	REQUEST VOL. ELEMENT ADDRESS b5
	SEND VOLUME TAG              b6 DATA OUT

	p. 454: Commands for communications devices (not listed previously)
	GET MESSAGE (6)              08
	GET MESSAGE (10)             28
	GET MESSAGE (12)             a8
      */
	
      switch (current_SC->cmnd[0]) {
      case CHANGE_DEFINITION: case COMPARE:         case COPY:
      case COPY_VERIFY:       case LOG_SELECT:      case MODE_SELECT:
      case MODE_SELECT_10:    case SEND_DIAGNOSTIC: case WRITE_BUFFER:

      case FORMAT_UNIT:       case REASSIGN_BLOCKS: case RESERVE:
      case SEARCH_EQUAL:      case SEARCH_HIGH:     case SEARCH_LOW:
      case WRITE_6:           case WRITE_10:        case WRITE_VERIFY:
      case 0x3f:              case 0x41:

      case 0xb1:              case 0xb0:            case 0xb2:
      case 0xaa:              case 0xae:

      case 0x24:

      case 0x38:              case 0x3d:

      case 0xb6:
	 
      case 0xea:		/* alternate number for WRITE LONG */
	 
	 current_SC->SCp.have_data_in = -1;
	 outb( 0xd0 | PARITY_MASK, TMC_Cntl_port );
	 break;

      case 0x00:
      default:
	 
	 current_SC->SCp.have_data_in = 1;
	 outb( 0x90 | PARITY_MASK, TMC_Cntl_port );
	 break;
      }
   }

   if (current_SC->SCp.have_data_in == -1) { /* DATA OUT */
      while ( (data_count = FIFO_Size - inw( FIFO_Data_Count_port )) > 512 ) {
#if EVERY_ACCESS
	 printk( "DC=%d, ", data_count ) ;
#endif
	 if (data_count > current_SC->SCp.this_residual)
	       data_count = current_SC->SCp.this_residual;
	 if (data_count > 0) {
#if EVERY_ACCESS
	    printk( "%d OUT, ", data_count );
#endif
	    if (data_count == 1) {
	       outb( *current_SC->SCp.ptr++, Write_FIFO_port );
	       --current_SC->SCp.this_residual;
	    } else {
	       data_count >>= 1;
	       outsw( Write_FIFO_port, current_SC->SCp.ptr, data_count );
	       current_SC->SCp.ptr += 2 * data_count;
	       current_SC->SCp.this_residual -= 2 * data_count;
	    }
	 }
	 if (!current_SC->SCp.this_residual) {
	    if (current_SC->SCp.buffers_residual) {
	       --current_SC->SCp.buffers_residual;
	       ++current_SC->SCp.buffer;
	       current_SC->SCp.ptr = current_SC->SCp.buffer->address;
	       current_SC->SCp.this_residual = current_SC->SCp.buffer->length;
	    } else
		  break;
	 }
      }
   }
   
   if (current_SC->SCp.have_data_in == 1) { /* DATA IN */
      while ((data_count = inw( FIFO_Data_Count_port )) > 0) {
#if EVERY_ACCESS
	 printk( "DC=%d, ", data_count );
#endif
	 if (data_count > current_SC->SCp.this_residual)
	       data_count = current_SC->SCp.this_residual;
	 if (data_count) {
#if EVERY_ACCESS
	    printk( "%d IN, ", data_count );
#endif
	    if (data_count == 1) {
	       *current_SC->SCp.ptr++ = inb( Read_FIFO_port );
	       --current_SC->SCp.this_residual;
	    } else {
	       data_count >>= 1; /* Number of words */
	       insw( Read_FIFO_port, current_SC->SCp.ptr, data_count );
	       current_SC->SCp.ptr += 2 * data_count;
	       current_SC->SCp.this_residual -= 2 * data_count;
	    }
	 }
	 if (!current_SC->SCp.this_residual
	     && current_SC->SCp.buffers_residual) {
	    --current_SC->SCp.buffers_residual;
	    ++current_SC->SCp.buffer;
	    current_SC->SCp.ptr = current_SC->SCp.buffer->address;
	    current_SC->SCp.this_residual = current_SC->SCp.buffer->length;
	 }
      }
   }
   
   if (done) {
#if EVERY_ACCESS
      printk( " ** IN DONE %d ** ", current_SC->SCp.have_data_in );
#endif

#if ERRORS_ONLY
      if (current_SC->cmnd[0] == REQUEST_SENSE && !current_SC->SCp.Status) {
	 if ((unsigned char)(*((char *)current_SC->request_buffer+2)) & 0x0f) {
	    unsigned char key;
	    unsigned char code;
	    unsigned char qualifier;

	    key = (unsigned char)(*((char *)current_SC->request_buffer + 2))
		  & 0x0f;
	    code = (unsigned char)(*((char *)current_SC->request_buffer + 12));
	    qualifier = (unsigned char)(*((char *)current_SC->request_buffer
					  + 13));

	    if (key != UNIT_ATTENTION
		&& !(key == NOT_READY
		     && code == 0x04
		     && (!qualifier || qualifier == 0x02 || qualifier == 0x01))
		&& !(key == ILLEGAL_REQUEST && (code == 0x25
						|| code == 0x24
						|| !code)))
		  
		  printk( "fdomain: REQUEST SENSE "
			  "Key = %x, Code = %x, Qualifier = %x\n",
			  key, code, qualifier );
	 }
      }
#endif
#if EVERY_ACCESS
      printk( "BEFORE MY_DONE. . ." );
#endif
      my_done( (current_SC->SCp.Status & 0xff)
	       | ((current_SC->SCp.Message & 0xff) << 8) | (DID_OK << 16) );
#if EVERY_ACCESS
      printk( "RETURNING.\n" );
#endif
      
   } else {
      if (current_SC->SCp.phase & disconnect) {
	 outb( 0xd0 | FIFO_COUNT, Interrupt_Cntl_port );
	 outb( 0x00, SCSI_Cntl_port );
      } else {
	 outb( 0x90 | FIFO_COUNT, Interrupt_Cntl_port );
      }
   }
#if DEBUG_RACE
   in_interrupt_flag = 0;
#endif
   return;
}

int fdomain_16x0_queue( Scsi_Cmnd * SCpnt, void (*done)(Scsi_Cmnd *))
{
   if (in_command) {
      panic( "fdomain: fdomain_16x0_queue() NOT REENTRANT!\n" );
   }
#if EVERY_ACCESS
   printk( "queue: target = %d cmnd = 0x%02x pieces = %d size = %u\n",
	   SCpnt->target,
	   *(unsigned char *)SCpnt->cmnd,
	   SCpnt->use_sg,
	   SCpnt->request_bufflen );
#endif

   fdomain_make_bus_idle();

   current_SC            = SCpnt; /* Save this for the done function */
   current_SC->scsi_done = done;

   /* Initialize static data */

   if (current_SC->use_sg) {
      current_SC->SCp.buffer =
	    (struct scatterlist *)current_SC->request_buffer;
      current_SC->SCp.ptr              = current_SC->SCp.buffer->address;
      current_SC->SCp.this_residual    = current_SC->SCp.buffer->length;
      current_SC->SCp.buffers_residual = current_SC->use_sg - 1;
   } else {
      current_SC->SCp.ptr              = (char *)current_SC->request_buffer;
      current_SC->SCp.this_residual    = current_SC->request_bufflen;
      current_SC->SCp.buffer           = NULL;
      current_SC->SCp.buffers_residual = 0;
   }
	 
   
   current_SC->SCp.Status              = 0;
   current_SC->SCp.Message             = 0;
   current_SC->SCp.have_data_in        = 0;
   current_SC->SCp.sent_command        = 0;
   current_SC->SCp.phase               = in_arbitration;

   /* Start arbitration */
   outb( 0x00, Interrupt_Cntl_port );
   outb( 0x00, SCSI_Cntl_port );              /* Disable data drivers */
   outb( adapter_mask, SCSI_Data_NoACK_port ); /* Set our id bit */
   ++in_command;
   outb( 0x20, Interrupt_Cntl_port );
   outb( 0x14 | PARITY_MASK, TMC_Cntl_port ); /* Start arbitration */

   return 0;
}

/* The following code, which simulates the old-style command function, was
   taken from Tommy Thorn's aha1542.c file.  This code is Copyright (C)
   1992 Tommy Thorn. */

static volatile int internal_done_flag    = 0;
static volatile int internal_done_errcode = 0;

static void internal_done( Scsi_Cmnd *SCpnt )
{
    internal_done_errcode = SCpnt->result;
    ++internal_done_flag;
}

int fdomain_16x0_command( Scsi_Cmnd *SCpnt )
{
    fdomain_16x0_queue( SCpnt, internal_done );

    while (!internal_done_flag)
	  ;
    internal_done_flag = 0;
    return internal_done_errcode;
}

/* End of code derived from Tommy Thorn's work. */

void print_info( Scsi_Cmnd *SCpnt )
{
   unsigned int imr;
   unsigned int irr;
   unsigned int isr;

   if (!SCpnt || !SCpnt->host) {
      printk( "fdomain: cannot provide detailed information\n" );
   }
   
   printk( "%s\n", fdomain_16x0_info( SCpnt->host ) );
   print_banner( SCpnt->host );
   switch (SCpnt->SCp.phase) {
   case in_arbitration: printk( "arbitration " ); break;
   case in_selection:   printk( "selection " );   break;
   case in_other:       printk( "other " );       break;
   default:             printk( "unknown " );     break;
   }

   printk( "(%d), target = %d cmnd = 0x%02x pieces = %d size = %u\n",
	   SCpnt->SCp.phase,
	   SCpnt->target,
	   *(unsigned char *)SCpnt->cmnd,
	   SCpnt->use_sg,
	   SCpnt->request_bufflen );
   printk( "sent_command = %d, have_data_in = %d, timeout = %d\n",
	   SCpnt->SCp.sent_command,
	   SCpnt->SCp.have_data_in,
	   SCpnt->timeout );
#if DEBUG_RACE
   printk( "in_interrupt_flag = %d\n", in_interrupt_flag );
#endif

   imr = (inb( 0x0a1 ) << 8) + inb( 0x21 );
   outb( 0x0a, 0xa0 );
   irr = inb( 0xa0 ) << 8;
   outb( 0x0a, 0x20 );
   irr += inb( 0x20 );
   outb( 0x0b, 0xa0 );
   isr = inb( 0xa0 ) << 8;
   outb( 0x0b, 0x20 );
   isr += inb( 0x20 );

				/* Print out interesting information */
   printk( "IMR = 0x%04x", imr );
   if (imr & (1 << interrupt_level))
	 printk( " (masked)" );
   printk( ", IRR = 0x%04x, ISR = 0x%04x\n", irr, isr );

   printk( "SCSI Status      = 0x%02x\n", inb( SCSI_Status_port ) );
   printk( "TMC Status       = 0x%02x", inb( TMC_Status_port ) );
   if (inb( TMC_Status_port & 1))
	 printk( " (interrupt)" );
   printk( "\n" );
   printk( "Interrupt Status = 0x%02x", inb( Interrupt_Status_port ) );
   if (inb( Interrupt_Status_port ) & 0x08)
	 printk( " (enabled)" );
   printk( "\n" );
   if (chip == tmc18c50 || chip == tmc18c30) {
      printk( "FIFO Status      = 0x%02x\n", inb( port_base + FIFO_Status ) );
      printk( "Int. Condition   = 0x%02x\n",
	      inb( port_base + Interrupt_Cond ) );
   }
   printk( "Configuration 1  = 0x%02x\n", inb( port_base + Configuration1 ) );
   if (chip == tmc18c50 || chip == tmc18c30)
	 printk( "Configuration 2  = 0x%02x\n",
		 inb( port_base + Configuration2 ) );
}

int fdomain_16x0_abort( Scsi_Cmnd *SCpnt)
{
   unsigned long flags;
#if EVERY_ACCESS || ERRORS_ONLY || DEBUG_ABORT
   printk( "fdomain: abort " );
#endif

   save_flags( flags );
   cli();
   if (!in_command) {
#if EVERY_ACCESS || ERRORS_ONLY
      printk( " (not in command)\n" );
#endif
      restore_flags( flags );
      return SCSI_ABORT_NOT_RUNNING;
   } else printk( "\n" );

#if DEBUG_ABORT
   print_info( SCpnt );
#endif

   fdomain_make_bus_idle();

   current_SC->SCp.phase |= aborted;

   current_SC->result = DID_ABORT << 16;

   restore_flags( flags );
   
   /* Aborts are not done well. . . */
   my_done( DID_ABORT << 16 );

   return SCSI_ABORT_SUCCESS;
}

int fdomain_16x0_reset( Scsi_Cmnd *SCpnt, unsigned int flags )
{
#if DEBUG_RESET
   static int called_once = 0;
#endif

#if ERRORS_ONLY
   if (SCpnt) printk( "fdomain: SCSI Bus Reset\n" );
#endif

#if DEBUG_RESET
   if (called_once) print_info( current_SC );
   called_once = 1;
#endif
   
   outb( 1, SCSI_Cntl_port );
   do_pause( 2 );
   outb( 0, SCSI_Cntl_port );
   do_pause( 115 );
   outb( 0, SCSI_Mode_Cntl_port );
   outb( PARITY_MASK, TMC_Cntl_port );

   /* Unless this is the very first call (i.e., SCPnt == NULL), everything
      is probably hosed at this point.  We will, however, try to keep
      things going by informing the high-level code that we need help. */

   return SCSI_RESET_WAKEUP;
}

#include "sd.h"
#include <scsi/scsi_ioctl.h>

int fdomain_16x0_biosparam( Scsi_Disk *disk, kdev_t dev, int *info_array )
{
   int              drive;
   unsigned char    buf[512 + sizeof( int ) * 2];
   int		    size      = disk->capacity;
   int              *sizes    = (int *)buf;
   unsigned char    *data     = (unsigned char *)(sizes + 2);
   unsigned char    do_read[] = { READ_6, 0, 0, 0, 1, 0 };
   int              retcode;
   struct drive_info {
      unsigned short cylinders;
      unsigned char  heads;
      unsigned char  sectors;
   } *i;
   
   /* NOTES:
      The RAM area starts at 0x1f00 from the bios_base address.

      For BIOS Version 2.0:
      
      The drive parameter table seems to start at 0x1f30.
      The first byte's purpose is not known.
      Next is the cylinder, head, and sector information.
      The last 4 bytes appear to be the drive's size in sectors.
      The other bytes in the drive parameter table are unknown.
      If anyone figures them out, please send me mail, and I will
      update these notes.

      Tape drives do not get placed in this table.

      There is another table at 0x1fea:
      If the byte is 0x01, then the SCSI ID is not in use.
      If the byte is 0x18 or 0x48, then the SCSI ID is in use,
      although tapes don't seem to be in this table.  I haven't
      seen any other numbers (in a limited sample).

      0x1f2d is a drive count (i.e., not including tapes)

      The table at 0x1fcc are I/O ports addresses for the various
      operations.  I calculate these by hand in this driver code.

      
      
      For the ISA-200S version of BIOS Version 2.0:

      The drive parameter table starts at 0x1f33.

      WARNING: Assume that the table entry is 25 bytes long.  Someone needs
      to check this for the Quantum ISA-200S card.

      
      
      For BIOS Version 3.2:

      The drive parameter table starts at 0x1f70.  Each entry is
      0x0a bytes long.  Heads are one less than we need to report.
    */

   drive = MINOR(dev) / 16;

   if (bios_major == 2) {
      switch (Quantum) {
      case 2:			/* ISA_200S */
				/* The value of 25 has never been verified.
				   It should probably be 15. */
	 i = (struct drive_info *)( (char *)bios_base + 0x1f33 + drive * 25 );
	 break;
      case 3:			/* ISA_250MG */
	 i = (struct drive_info *)( (char *)bios_base + 0x1f36 + drive * 15 );
	 break;
      case 4:			/* ISA_200S (another one) */
	 i = (struct drive_info *)( (char *)bios_base + 0x1f34 + drive * 15 );
	 break;
      default:
	 i = (struct drive_info *)( (char *)bios_base + 0x1f31 + drive * 25 );
	 break;
      }
      info_array[0] = i->heads;
      info_array[1] = i->sectors;
      info_array[2] = i->cylinders;
   } else if (bios_major == 3
	      && bios_minor >= 0
	      && bios_minor < 4) { /* 3.0 and 3.2 BIOS */
      i = (struct drive_info *)( (char *)bios_base + 0x1f71 + drive * 10 );
      info_array[0] = i->heads + 1;
      info_array[1] = i->sectors;
      info_array[2] = i->cylinders;
   } else {			/* 3.4 BIOS (and up?) */
      /* This algorithm was provided by Future Domain (much thanks!). */

      sizes[0] = 0;		/* zero bytes out */
      sizes[1] = 512;		/* one sector in */
      memcpy( data, do_read, sizeof( do_read ) );
      retcode = kernel_scsi_ioctl( disk->device,
				   SCSI_IOCTL_SEND_COMMAND,
				   (void *)buf );
      if (!retcode				    /* SCSI command ok */
	  && data[511] == 0xaa && data[510] == 0x55 /* Partition table valid */
	  && data[0x1c2]) {			    /* Partition type */

	 /* The partition table layout is as follows:

	    Start: 0x1b3h
	    Offset: 0 = partition status
		    1 = starting head
		    2 = starting sector and cylinder (word, encoded)
		    4 = partition type
		    5 = ending head
		    6 = ending sector and cylinder (word, encoded)
		    8 = starting absolute sector (double word)
		    c = number of sectors (double word)
	    Signature: 0x1fe = 0x55aa

	    So, this algorithm assumes:
	    1) the first partition table is in use,
	    2) the data in the first entry is correct, and
	    3) partitions never divide cylinders

	    Note that (1) may be FALSE for NetBSD (and other BSD flavors),
	    as well as for Linux.  Note also, that Linux doesn't pay any
	    attention to the fields that are used by this algorithm -- it
	    only uses the absolute sector data.  Recent versions of Linux's
	    fdisk(1) will fill this data in correctly, and forthcoming
	    versions will check for consistency.

	    Checking for a non-zero partition type is not part of the
	    Future Domain algorithm, but it seemed to be a reasonable thing
	    to do, especially in the Linux and BSD worlds. */

	 info_array[0] = data[0x1c3] + 1;	    /* heads */
	 info_array[1] = data[0x1c4] & 0x3f;	    /* sectors */
      } else {

 	 /* Note that this new method guarantees that there will always be
	    less than 1024 cylinders on a platter.  This is good for drives
	    up to approximately 7.85GB (where 1GB = 1024 * 1024 kB). */

	 if ((unsigned int)size >= 0x7e0000U) {
	    info_array[0] = 0xff; /* heads   = 255 */
	    info_array[1] = 0x3f; /* sectors =  63 */
	 } else if ((unsigned int)size >= 0x200000U) {
	    info_array[0] = 0x80; /* heads   = 128 */
	    info_array[1] = 0x3f; /* sectors =  63 */
	 } else {
	    info_array[0] = 0x40; /* heads   =  64 */
	    info_array[1] = 0x20; /* sectors =  32 */
	 }
      }
				/* For both methods, compute the cylinders */
      info_array[2] = (unsigned int)size / (info_array[0] * info_array[1] );
   }
   
   return 0;
}

#ifdef MODULE
/* Eventually this will go into an include file, but this will be later */
Scsi_Host_Template driver_template = FDOMAIN_16X0;

#include "scsi_module.c"
#endif
