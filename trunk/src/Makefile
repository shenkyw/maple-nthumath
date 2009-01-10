# ------------------------------------------------------- #
#  src/Makefile	( NTHU CS MapleBBS Ver 3.10 )	          #
# ------------------------------------------------------- #
#  target : Makefile for ALL				  #
#  create : 00/02/12                                      #
#  update :   /  /                                        #
# ------------------------------------------------------- #


# 支援的 OS-type
# sun linux solaris sol-x86 freebsd bsd cygwin

# 需要 compile 的目錄
# lib daemon innbbsd maple so game pip util util/backup util/tran util/uno

all:
	@cd lib; make
	@cd daemon; make
	@cd innbbsd; make
	@cd maple; make
	@cd so; make
	@cd game; make
	@cd pip; make
	@cd util; make
	@cd util/backup; make
	@cd util/tran; make
	@cd util/uno; make

install:
	@cd daemon; make install
	@cd innbbsd; make install
	@cd maple; make install
	@cd so; make install
	@cd game; make install
	@cd pip; make install
	@cd util; make install
	@cd util/backup; make install
	@cd util/tran; make install
	@cd util/uno; make install

install-deb:
	@cd daemon; make install-deb
	@cd innbbsd; make install-deb
	@cd maple; make install-deb
	@cd so; make install-deb
	@cd game; make install-deb
	@cd pip; make install-deb
	@cd util; make install-deb
	@cd util/backup; make install-deb
	@cd util/tran; make install-deb
	@cd util/uno; make install-deb

update:
	@cd daemon; make update
	@cd innbbsd; make update
	@cd maple; make update

clean:
	@cd lib; make clean
	@cd daemon; make clean
	@cd innbbsd; make clean
	@cd maple; make clean
	@cd so; make clean
	@cd game; make clean
	@cd pip; make clean
	@cd util; make clean
	@cd util/backup; make clean
	@cd util/tran; make clean
	@cd util/uno; make clean
