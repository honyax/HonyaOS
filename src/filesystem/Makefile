FS_IMG	= filesystem.img
FS_DIR	= filesystem_dir

# sudoersにて、mount, umount, cpコマンドをNOPASSWDで登録しておく必要がある。
.PHONY: all
all:
	dd if=/dev/zero of=$(FS_IMG) bs=1024 count=320
	mkfs.vfat -F12 $(FS_IMG)
	mkdir $(FS_DIR)
	sudo mount $(FS_IMG) $(FS_DIR)
	sudo cp Makefile $(FS_DIR)
	sudo cp ../tasks/*.hny $(FS_DIR)
	sudo umount $(FS_DIR)
	rmdir $(FS_DIR)

.PHONY: clean
clean:
	rm -f $(FS_IMG)
