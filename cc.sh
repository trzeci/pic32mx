:

. cc_env.sh

doCRT()
{
  (
    cd crt &&
    rm -f *.o &&
    for f in *.c
    do
      "$CCPATH"gcc -I../include $CCOPTS -c $f
    done &&
    rm -f ../c0_"$C0SFX".a &&
    "$CCPATH"ar rv ../c0_"$C0SFX".a *.o &&
    "$CCPATH"ranlib ../c0_"$C0SFX".a
  )
}

echo c0   && "$CCPATH"gcc $CCOPTS  -c src/c0.S -o c0_"$C0SFX".o &&
echo crt  && doCRT &&
true
