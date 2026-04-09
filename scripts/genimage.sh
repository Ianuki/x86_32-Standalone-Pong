dd if=/dev/zero of=run.img bs=512 count=2880
dd if=bin/boot.bin of=run.img conv=notrunc
dd if=bin/main.bin of=run.img bs=512 seek=1 conv=notrunc