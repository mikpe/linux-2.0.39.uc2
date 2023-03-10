/* $Id: isdnif.h,v 1.1.1.1 1999-11-22 03:47:00 christ Exp $
 *
 * Linux ISDN subsystem
 *
 * Definition of the interface between the subsystem and its low-level drivers.
 *
 * Copyright 1994-1998  by Fritz Elfert (fritz@isdn4linux.de)
 * Copyright 1995,96    Thinking Objects Software GmbH Wuerzburg
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
 * $Log: isdnif.h,v $
 * Revision 1.1.1.1  1999-11-22 03:47:00  christ
 * Importing new-wave v1.0.4
 *
 * Revision 1.20.2.2  1998/11/05 22:13:33  fritz
 * Changed mail-address.
 *
 * Revision 1.20.2.1  1998/03/07 23:00:50  tsbogend
 * added defines for Linux/Alpha 2.0.x with alpha-patches
 *
 * Revision 1.20  1997/05/27 15:18:06  fritz
 * Added changes for recent 2.1.x kernels:
 *   changed return type of isdn_close
 *   queue_task_* -> queue_task
 *   clear/set_bit -> test_and_... where apropriate.
 *   changed type of hard_header_cache parameter.
 *
 * Revision 1.19  1997/03/25 23:13:56  keil
 * NI-1 US protocol
 *
 * Revision 1.18  1997/03/04 22:09:18  calle
 * Change macros copy_from_user and copy_to_user in inline function.
 * These are now correct replacements of the functions for 2.1.xx
 *
 * Revision 1.17  1997/02/10 21:12:53  fritz
 * More setup-interface changes.
 *
 * Revision 1.16  1997/02/10 19:42:57  fritz
 * New interface for reporting incoming calls.
 *
 * Revision 1.15  1997/02/09 00:18:42  keil
 * leased line support
 *
 * Revision 1.14  1997/02/03 23:43:00  fritz
 * Misc changes for Kernel 2.1.X compatibility.
 *
 * Revision 1.13  1996/11/13 02:39:59  fritz
 * More compatibility changes.
 *
 * Revision 1.12  1996/11/06 17:38:48  keil
 * more changes for 2.1.X
 *
 * Revision 1.11  1996/10/23 11:59:42  fritz
 * More compatibility changes.
 *
 * Revision 1.10  1996/10/22 23:14:19  fritz
 * Changes for compatibility to 2.0.X and 2.1.X kernels.
 *
 * Revision 1.9  1996/06/06 21:24:24  fritz
 * Started adding support for suspend/resume.
 *
 * Revision 1.8  1996/05/18 01:45:37  fritz
 * More spelling corrections.
 *
 * Revision 1.7  1996/05/18 01:37:19  fritz
 * Added spelling corrections and some minor changes
 * to stay in sync with kernel.
 *
 * Revision 1.6  1996/05/17 03:59:28  fritz
 * Marked rcvcallb and writebuf obsolete.
 *
 * Revision 1.5  1996/05/01 11:43:54  fritz
 * Removed STANDALONE
 *
 * Revision 1.4  1996/05/01 11:38:40  fritz
 * Added ISDN_FEATURE_L2_TRANS
 *
 * Revision 1.3  1996/04/29 22:57:54  fritz
 * Added driverId and channel parameters to
 * writecmd() and readstat().
 * Added constant for voice-support.
 *
 * Revision 1.2  1996/04/20 17:02:40  fritz
 * Changes to support skbuffs for Lowlevel-Drivers.
 * Misc. typos
 *
 * Revision 1.1  1996/01/09 05:50:51  fritz
 * Initial revision
 *
 */

#ifndef isdnif_h
#define isdnif_h

/*
 * Values for general protocol-selection
 */
#define ISDN_PTYPE_UNKNOWN   0   /* Protocol undefined   */
#define ISDN_PTYPE_1TR6      1   /* german 1TR6-protocol */
#define ISDN_PTYPE_EURO      2   /* EDSS1-protocol       */
#define ISDN_PTYPE_LEASED    3   /* for leased lines     */
#define ISDN_PTYPE_NI1       4   /* US NI-1 protocol     */

/*
 * Values for Layer-2-protocol-selection
 */
#define ISDN_PROTO_L2_X75I   0   /* X75/LAPB with I-Frames      */
#define ISDN_PROTO_L2_X75UI  1   /* X75/LAPB with UI-Frames     */
#define ISDN_PROTO_L2_X75BUI 2   /* X75/LAPB with UI-Frames     */
#define ISDN_PROTO_L2_HDLC   3   /* HDLC                        */
#define ISDN_PROTO_L2_TRANS  4   /* Transparent (Voice)         */

/*
 * Values for Layer-3-protocol-selection
 */
#define ISDN_PROTO_L3_TRANS  0   /* Transparent                 */

#ifdef __KERNEL__

#include <linux/skbuff.h>

/*
 * Commands from linklevel to lowlevel
 *
 */
#define ISDN_CMD_IOCTL   0       /* Perform ioctl                         */
#define ISDN_CMD_DIAL     1       /* Dial out                              */
#define ISDN_CMD_ACCEPTD  2       /* Accept an incoming call on D-Chan.    */
#define ISDN_CMD_ACCEPTB  3       /* Request B-Channel connect.            */
#define ISDN_CMD_HANGUP   4       /* Hangup                                */
#define ISDN_CMD_CLREAZ   5       /* Clear EAZ(s) of channel               */
#define ISDN_CMD_SETEAZ   6       /* Set EAZ(s) of channel                 */
#define ISDN_CMD_GETEAZ   7       /* Get EAZ(s) of channel                 */
#define ISDN_CMD_SETSIL   8       /* Set Service-Indicator-List of channel */
#define ISDN_CMD_GETSIL   9       /* Get Service-Indicator-List of channel */
#define ISDN_CMD_SETL2   10       /* Set B-Chan. Layer2-Parameter          */
#define ISDN_CMD_GETL2   11       /* Get B-Chan. Layer2-Parameter          */
#define ISDN_CMD_SETL3   12       /* Set B-Chan. Layer3-Parameter          */
#define ISDN_CMD_GETL3   13       /* Get B-Chan. Layer3-Parameter          */
#define ISDN_CMD_LOCK    14       /* Signal usage by upper levels          */
#define ISDN_CMD_UNLOCK  15       /* Release usage-lock                    */
#define ISDN_CMD_SUSPEND 16       /* Suspend connection                    */
#define ISDN_CMD_RESUME  17       /* Resume connection                     */

/*
 * Status-Values delivered from lowlevel to linklevel via
 * statcallb().
 *
 */
#define ISDN_STAT_STAVAIL 256    /* Raw status-data available             */
#define ISDN_STAT_ICALL   257    /* Incoming call detected                */
#define ISDN_STAT_RUN     258    /* Signal protocol-code is running       */
#define ISDN_STAT_STOP    259    /* Signal halt of protocol-code          */
#define ISDN_STAT_DCONN   260    /* Signal D-Channel connect              */
#define ISDN_STAT_BCONN   261    /* Signal B-Channel connect              */
#define ISDN_STAT_DHUP    262    /* Signal D-Channel disconnect           */
#define ISDN_STAT_BHUP    263    /* Signal B-Channel disconnect           */
#define ISDN_STAT_CINF    264    /* Charge-Info                           */
#define ISDN_STAT_LOAD    265    /* Signal new lowlevel-driver is loaded  */
#define ISDN_STAT_UNLOAD  266    /* Signal unload of lowlevel-driver      */
#define ISDN_STAT_BSENT   267    /* Signal packet sent                    */
#define ISDN_STAT_NODCH   268    /* Signal no D-Channel                   */
#define ISDN_STAT_ADDCH   269    /* Add more Channels                     */
#define ISDN_STAT_CAUSE   270    /* Cause-Message                         */

/*
 * Values for feature-field of interface-struct.
 */
/* Layer 2 */
#define ISDN_FEATURE_L2_X75I    (0x0001 << ISDN_PROTO_L2_X75I)
#define ISDN_FEATURE_L2_X75UI   (0x0001 << ISDN_PROTO_L2_X75UI)
#define ISDN_FEATURE_L2_X75BUI  (0x0001 << ISDN_PROTO_L2_X75BUI)
#define ISDN_FEATURE_L2_HDLC    (0x0001 << ISDN_PROTO_L2_HDLC)
#define ISDN_FEATURE_L2_TRANS   (0x0001 << ISDN_PROTO_L2_TRANS)

/* Layer 3 */
#define ISDN_FEATURE_L3_TRANS   (0x0100 << ISDN_PROTO_L3_TRANS)

/* Signaling */
#define ISDN_FEATURE_P_UNKNOWN  (0x1000 << ISDN_PTYPE_UNKNOWN)
#define ISDN_FEATURE_P_1TR6     (0x1000 << ISDN_PTYPE_1TR6)
#define ISDN_FEATURE_P_EURO     (0x1000 << ISDN_PTYPE_EURO)
#define ISDN_FEATURE_P_NI1      (0x1000 << ISDN_PTYPE_NI1)

typedef struct setup_parm {
    char phone[32];         /* Remote Phone-Number */
    char eazmsn[32];        /* Local EAZ or MSN    */
    unsigned char si1;      /* Service Indicator 1 */
    unsigned char si2;      /* Service Indicator 2 */
    unsigned char plan;     /* Numbering plan      */
    unsigned char screen;   /* Screening info      */
} setup_parm;

/*
 * Structure for exchanging above infos
 *
 */
typedef struct {
  int   driver;                  /* Lowlevel-Driver-ID                    */
  int   command;                 /* Command or Status (see above)         */
  ulong arg;                     /* Additional Data                       */
  union {
	char  num[50];               /* Additional Data                       */
	setup_parm setup;
  } parm;
} isdn_ctrl;

/*
 * The interface-struct itself (initialized at load-time of lowlevel-driver)
 *
 * See Documentation/isdn/INTERFACE for a description, how the communication
 * between the ISDN subsystem and its drivers is done.
 *
 */
typedef struct {
  /* Number of channels supported by this driver
   */
  int channels;

  /* 
   * Maximum Size of transmit/receive-buffer this driver supports.
   */
  int maxbufsize;

  /* Feature-Flags for this driver.
   * See defines ISDN_FEATURE_... for Values
   */
  unsigned long features;

  /*
   * Needed for calculating
   * dev->hard_header_len = linklayer header + hl_hdrlen;
   * Drivers, not supporting sk_buff's should set this to 0.
   */
  unsigned short hl_hdrlen;

  /* Receive-Callback
   * Parameters:
   *             int    Driver-ID
   *             int    local channel-number (0 ...)
   *             u_char pointer to received data (in Kernel-Space, volatile)
   *             int    length of data
   *
   * NOTE: This callback is obsolete, and will be removed when all
   *       current LL-drivers support rcvcall_skb. Do NOT use for new
   *       drivers.
   */
  void (*rcvcallb)(int, int, u_char*, int);

  /*
   * Receive-Callback using sk_buff's
   * Parameters:
   *             int                    Driver-ID
   *             int                    local channel-number (0 ...)
   *             struct sk_buff *skb    received Data
   */
  void (*rcvcallb_skb)(int, int, struct sk_buff *);

  /* Status-Callback
   * Parameters:
   *             isdn_ctrl*
   *                   driver  = Driver ID.
   *                   command = One of above ISDN_STAT_... constants.
   *                   arg     = depending on status-type.
   *                   num     = depending on status-type.
   */
  int (*statcallb)(isdn_ctrl*);
  /* Send command
   * Parameters:
   *             isdn_ctrl*
   *                   driver  = Driver ID.
   *                   command = One of above ISDN_CMD_... constants.
   *                   arg     = depending on command.
   *                   num     = depending on command.
   */
  int (*command)(isdn_ctrl*);
  /* Send Data
   * Parameters:
   *             int    driverId
   *             int    local channel-number (0 ...)
   *             u_char pointer to data
   *             int    length of data
   *             int    Flag: 0 = Call form Kernel-Space (use memcpy,
   *                              no schedule allowed) 
   *                          1 = Data is in User-Space (use memcpy_fromfs,
   *                              may schedule)
   *
   * NOTE: This call is obsolete, and will be removed when all
   *       current LL-drivers support writebuf_skb. Do NOT use for new
   *       drivers.
   */
  int (*writebuf)(int, int, const u_char*, int, int);

  /*
   * Send data using sk_buff's
   * Parameters:
   *             int                    driverId
   *             int                    local channel-number (0...)
   *             struct sk_buff *skb    Data to send
   */
  int (*writebuf_skb) (int, int, struct sk_buff *);

  /* Send raw D-Channel-Commands
   * Parameters:
   *             u_char pointer data
   *             int    length of data
   *             int    Flag: 0 = Call form Kernel-Space (use memcpy,
   *                              no schedule allowed) 
   *                          1 = Data is in User-Space (use memcpy_fromfs,
   *                              may schedule)
   *             int    driverId
   *             int    local channel-number (0 ...)
   */
  int (*writecmd)(const u_char*, int, int, int, int);
  /* Read raw Status replies
   *             u_char pointer data (volatile)
   *             int    length of buffer
   *             int    Flag: 0 = Call form Kernel-Space (use memcpy,
   *                              no schedule allowed) 
   *                          1 = Data is in User-Space (use memcpy_fromfs,
   *                              may schedule)
   *             int    driverId
   *             int    local channel-number (0 ...)
   */
  int (*readstat)(u_char*, int, int, int, int);
  char id[20];
} isdn_if;

/*
 * Function which must be called by lowlevel-driver at loadtime with
 * the following fields of above struct set:
 *
 * channels     Number of channels that will be supported.
 * hl_hdrlen    Space to preserve in sk_buff's when sending. Drivers, not
 *              supporting sk_buff's should set this to 0.
 * command      Address of Command-Handler.
 * features     Bitwise coded Features of this driver. (use ISDN_FEATURE_...)
 * writebuf     Address of Send-Command-Handler. OBSOLETE do NOT use anymore.
 * writebuf_skb Address of Skbuff-Send-Handler. (NULL if not supported)
 * writecmd        "    "  D-Channel  " which accepts raw D-Ch-Commands.
 * readstat        "    "  D-Channel  " which delivers raw Status-Data.
 *
 * The linklevel-driver fills the following fields:
 *
 * channels      Driver-ID assigned to this driver. (Must be used on all
 *               subsequent callbacks.
 * rcvcallb      Address of handler for received data. OBSOLETE, do NOT use anymore.
 * rcvcallb_skb  Address of handler for received Skbuff's. (NULL if not supp.)
 * statcallb        "    "     "    for status-changes.
 *
 */
extern int register_isdn(isdn_if*);

/* Compatibility Linux-2.0.X <-> Linux-2.1.X */

#ifndef LINUX_VERSION_CODE
#include <linux/version.h>
#endif
#if (LINUX_VERSION_CODE < 0x020100)
#include <linux/mm.h>

static inline unsigned long copy_from_user(void *to, const void *from, unsigned long n)
{
	int i;
	if ((i = verify_area(VERIFY_READ, from, n)) != 0)
		return i;
	memcpy_fromfs(to, from, n);
	return 0;
}

static inline unsigned long copy_to_user(void *to, const void *from, unsigned long n)
{
	int i;
	if ((i = verify_area(VERIFY_WRITE, to, n)) != 0)
		return i;
	memcpy_tofs(to, from, n);
	return 0;
}

#define GET_USER(x, addr) ( x = get_user(addr) )
#ifdef __alpha__ /* needed for 2.0.x with alpha-patches */
#define RWTYPE long
#define LSTYPE long
#define RWARG unsigned long
#else
#define RWTYPE int
#define LSTYPE int
#define RWARG int
#endif
#define LSARG off_t
#else
#include <asm/uaccess.h>
#define GET_USER get_user
#define PUT_USER put_user
#define RWTYPE long
#define LSTYPE long long
#define RWARG unsigned long
#define LSARG long long
#endif

#if (LINUX_VERSION_CODE < 0x02010F)
#define SET_SKB_FREE(x) ( x->free = 1 )
#else
#define SET_SKB_FREE(x)
#endif

#if (LINUX_VERSION_CODE < 0x02011F)
#define CLOSETYPE void
#define CLOSEVAL
#else
#define CLOSETYPE int
#define CLOSEVAL (0)
#endif

#if (LINUX_VERSION_CODE < 0x020125)
#define test_and_clear_bit clear_bit
#define test_and_set_bit set_bit
#endif

#endif /* __KERNEL__ */
#endif /* isdnif_h */
