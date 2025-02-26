#include <configs/xilinx_zynqmp.h>

/* I have no idea how else to do this dumbass thing */
/* I'll figure out what else is needed later */
/* This is a massive hack-and-a-half: it's called scsi-init b/c
   petalinux does a preboot=run scsi_init, so we abuse that */
/* Note: I'm having issue with escaping commands in setexpr,
   and thankfully we can avoid it because you can test for top
   bits being set with normal integer math. */
#define CONFIG_EXTRA_ENV_SETTINGS  \
  "mtdparts=nor0:1920k(boot),128k(bootscr),126M(qspifs)\0" \
  "checkpmic=mw.b 0x800000004 0x62 \
    && i2c mw 0x58 0x00 0x02 \
    && i2c read 0x58 0x81 1 0x800000000 \
    && cmp.b 0x800000000 0x800000004 1\0" \
  "checklock=i2c mw 0x58 0x00 0x00 \
    && i2c read 0x58 0x12 1 0x800000000 \
    && setexpr.b rval *0x800000000 \
    && itest 0x${rval} >= 0x80\0" \
  "boostvolt=i2c mw 0x58 0x00 0x00 \
    && i2c mw 0x58 0xAC 0x22 \
    && i2c mw 0x58 0xA9 0x18\0" \
  "setlock=i2c mw 0x58 0x00 0x00 \
    && i2c read 0x58 0x12 1 0x800000000 \
    && setexpr.b rval *0x800000000 + 0x80 \
    && i2c mw 0x58 0x12 0x${rval}\0" \
  "lockalready=echo Lock already set\0" \
  "pmicsetup=i2c dev 0; if run checkpmic ; then if run checklock ; then run lockalready ; else run boostvolt; run setlock; fi ; fi\0" \
  "scsi_init=run pmicsetup\0" \
  ""
