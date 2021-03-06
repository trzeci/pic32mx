:

. ../cc_env.sh

CCPOPTS="$CCOPTS $CCPOPTS -I$ENV_PATH/include -DFcy=80000000UL -DFpb=40000000UL"
LDOPTS="-Wl,-L$ENV_PATH $LDOPTS"
OBJS="devcfg.o crc16.o nvmem.o pic_hardware_asm.o pic_hardware.o"

doCC()
{
  echo "Compiling $1"
  "$CCPATH"g++ -c $CCPOPTS "$1".cc &&
  "$CCPATH"g++ -Wl,--defsym=BOOTLOAD=1 -o "$1".elf $CCPOPTS $LDOPTS -Wl,-Map="$1".map "$1".o $OBJS -lc &&
  "$CCPATH"objcopy -S -O ihex "$1".elf "$1".hex
}

echo hardware && "$CCPATH"g++ $CCPOPTS -c ../src/pic_hardware_asm.S ../src/pic_hardware.cc &&
echo devcfg   && "$CCPATH"gcc $CCPOPTS -c ../src/devcfg.c &&
echo nvmem    && "$CCPATH"g++ $CCPOPTS -c nvmem.cc &&
echo crc16    && "$CCPATH"g++ $CCPOPTS -c crc16.cc &&
doCC bootloader &&
true
