#!/usr/bin/env sh

USERNAME="jadyoung"
HOSTNAME="lab02.cs.ndsu.nodak.edu"
TBALL="jadyoungAssig2.tgz"
SCRIPT="rm -rf hw2; mkdir hw2; tar -xzf $TBALL -C hw2; rm $TBALL; cd hw2; make pdf; make clean; make dist;"

make distclean
make dist
cd dist
mkdir lab
scp "$TBALL" "$USERNAME@$HOSTNAME:~/"
ssh -l ${USERNAME} ${HOSTNAME} ${SCRIPT}
scp "$USERNAME@$HOSTNAME:~/hw2/dist/$TBALL" ./lab/
rm "$TBALL"
cd lab
tar -xzvf "$TBALL"
evince readme.pdf
