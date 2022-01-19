:

. ../cc_env.sh

CCPOPTS="$CCOPTS $CCPOPTS -I$ENV_PATH/include -DFcy=80000000UL -DFpb=40000000UL"
LDOPTS="-Wl,-L$ENV_PATH $LDOPTS"

doCC()
{
  echo "Compiling $1"
  "$CCPATH"g++ -c $CCPOPTS "$1".cc &&
  "$CCPATH"g++ -o "$1".elf $CCPOPTS $LDOPTS -Wl,-Map="$1".map "$1".o devcfg.o $2 -lc &&
  "$CCPATH"objcopy -S -O ihex "$1".elf "$1".hex
}

echo libc     && "$CCPATH"g++ $CCPOPTS -c ./libc.cc &&
echo hardware && "$CCPATH"g++ $CCPOPTS -c ../src/pic_hardware_asm.S ../src/pic_hardware.cc &&
echo devcfg   && "$CCPATH"gcc $CCPOPTS -c ../src/devcfg.c &&
doCC test01_piclib "pic_hardware_asm.o pic_hardware.o" &&
doCC test02_uart   "pic_hardware_asm.o pic_hardware.o" &&
doCC test03_libc   "pic_hardware_asm.o pic_hardware.o libc.o" &&
doCC test04_stl    "pic_hardware_asm.o pic_hardware.o libc.o -lstdc++" &&
doCC test05_exc    "pic_hardware_asm.o pic_hardware.o libc.o -lstdc++ -lsupc++" &&
doCC test06_fpu    "pic_hardware_asm.o pic_hardware.o libc.o -lstdc++" &&
doCC test07_isr    "pic_hardware_asm.o pic_hardware.o libc.o" &&
doCC test08_GEH    "pic_hardware_asm.o pic_hardware.o libc.o" &&
true
