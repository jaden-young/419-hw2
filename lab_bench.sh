#!/usr/bin/env sh

USERNAME="jadyoung"
HOSTNAME="lab02.cs.ndsu.nodak.edu"
TBALL="jadyoungAssignment2.tgz"
SCRIPT="rm -rf hw2; mkdir hw2; tar -xzf $TBALL -C hw2; rm $TBALL; cd hw2; make; make dist;"

make clean
make dist
cd dist
mkdir lab
scp "$TBALL" "$USERNAME@$HOSTNAME:~/"
ssh -l ${USERNAME} ${HOSTNAME} ${SCRIPT}
scp "$USERNAME@$HOSTNAME:~/hw2/dist/$TBALL" ./lab/
