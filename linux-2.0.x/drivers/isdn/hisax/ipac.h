/* $Id: ipac.h,v 1.1.1.1 1999-11-22 03:47:20 christ Exp $

 * ipac.h   IPAC specific defines
 *
 * Author       Karsten Keil (keil@temic-ech.spacenet.de)
 *
 *
 * $Log: ipac.h,v $
 * Revision 1.1.1.1  1999-11-22 03:47:20  christ
 * Importing new-wave v1.0.4
 *
 * Revision 1.1.2.2  1998/04/11 18:49:48  keil
 * add IPAC_ATX
 *
 * Revision 1.1.2.1  1997/10/17 22:10:48  keil
 * new files on 2.0
 *
 *
 *
 */


/* All Registers original Siemens Spec  */

#define IPAC_CONF	0xC0
#define IPAC_MASK	0xC1
#define IPAC_ISTA	0xC1
#define IPAC_ID		0xC2
#define IPAC_ACFG	0xC3
#define IPAC_AOE	0xC4
#define IPAC_ARX	0xC5
#define IPAC_ATX	0xC5
#define IPAC_PITA1	0xC6
#define IPAC_PITA2	0xC7
#define IPAC_POTA1	0xC8
#define IPAC_POTA2	0xC9
#define IPAC_PCFG	0xCA
#define IPAC_SCFG	0xCB
#define IPAC_TIMR2	0xCC
