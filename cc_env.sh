PROC=mx
PART=32MX575F512H

ENV_PATH=`dirname ${BASH_SOURCE[0]}`
CCPATH="/opt/cross/mipsel-pic32$PROC-elf/bin/mipsel-pic32$PROC-elf-"

# MX m4k
# MZ m14kc

case "$PROC" in
  mx)
    CCOPTS="-D__PIC32MX__        -march=m4k   -msoft-float -mips32r2 -mips16 -minterlink-mips16"
    C0SFX="MX"
    ;;
  mz)
    CCOPTS="-D__PIC32MZ__ -DF240 -march=m14kc -mfp64       -mips32r2 -mmicromips"
    C0SFX="MZ"
    ;;
esac

CCOPTS="-Os -static -D__"$PART"__ -Wno-attributes -fexceptions -EL -mabi=32 -mno-shared -mno-plt $CCOPTS -I . -I ./include"
LDOPTS="-static -Wl,--script=$ENV_PATH/gcc_"$PART".ld -Wl,--unresolved-symbols=report-all"
CRT="init.c default-exception-handler.c"

echo PROC="$PROC"
echo CCPATH="$CCPATH"
echo CCOPTS="$CCOPTS"
