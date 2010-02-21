$(if $(wildcard config.mak),,$(error Please run configure first))
include config.mak

ARCH_DIR = $(if $(filter $(ARCH),x86_64 i386),x86,$(ARCH))
ARCH_CONFIG := $(shell echo $(ARCH_DIR) | tr '[:lower:]' '[:upper:]')
# NONARCH_CONFIG used for unifdef, and only cover X86 and IA64 now
NONARCH_CONFIG = $(filter-out $(ARCH_CONFIG),X86 IA64)

PREFIX = /usr/local
DESTDIR = /

MAKEFILE_PRE = $(ARCH_DIR)/Makefile.pre

export INSTALL_MOD_DIR=updates
HEADERDIR = $(PREFIX)/include/kvm-kmod
PKGCONFIGDIR = $(PREFIX)/lib/pkgconfig

rpmrelease = devel

LINUX = ./linux-2.6

all:: prerequisite
#	include header priority 1) $LINUX 2) $KERNELDIR 3) include-compat
	$(MAKE) -C $(KERNELDIR) M=`pwd` \
		LINUXINCLUDE="-I`pwd`/include -Iinclude \
		$(if $(KERNELSOURCEDIR),\
			-Iinclude2 -I$(KERNELSOURCEDIR)/include -I$(KERNELSOURCEDIR)/arch/${ARCH_DIR}/include, \
			-Iarch/${ARCH_DIR}/include) -I`pwd`/include-compat -I`pwd`/${ARCH_DIR} \
		-include include/linux/autoconf.h \
		-include `pwd`/$(ARCH_DIR)/external-module-compat.h" \
		"$$@"

include $(MAKEFILE_PRE)

KVM_VERSION_GIT = $(if $(and $(filter kvm-devel,$(KVM_VERSION)), \
			 $(wildcard $(LINUX)/.git)), \
			   $(shell git --git-dir=$(LINUX)/.git describe), \
			   $(KVM_VERSION))

sync-kmod:
	./sync -v $(KVM_VERSION_GIT) -l $(LINUX)

sync-hdr:
	for a in ia64 x86; do \
		$(MAKE) -C $(LINUX) INSTALL_HDR_PATH=`pwd`/.tmp-hdrs SRCARCH="$$a" headers_install; \
		mkdir -p usr/include/asm-"$$a"; \
		cp .tmp-hdrs/include/asm/{kvm,kvm_para}.h usr/include/asm-"$$a"; \
	done
	mkdir -p usr/include/linux
	cp .tmp-hdrs/include/linux/{kvm,kvm_para}.h usr/include/linux
	rm -rf .tmp-hdrs

sync: sync-kmod sync-hdr

tmppc = .tmp.kvm-kmod.pc

KVM_KMOD_VERSION = $(strip $(if $(wildcard KVM_VERSION), \
			$(shell cat KVM_VERSION), \
			$(if $(wildcard .git), \
				$(shell git describe), \
				kvm-devel)))

install-hdr:
	mkdir -p $(DESTDIR)/$(HEADERDIR)/asm/
	install -m 644 usr/include/asm-$(ARCH_DIR)/{kvm,kvm_para}.h $(DESTDIR)/$(HEADERDIR)/asm/
	mkdir -p $(DESTDIR)/$(HEADERDIR)/linux/
	install -m 644 usr/include/linux/{kvm,kvm_para}.h $(DESTDIR)/$(HEADERDIR)/linux/
	sed 's|PREFIX|$(PREFIX)|; s/VERSION/$(KVM_KMOD_VERSION)/' kvm-kmod.pc > $(tmppc)
	install -m 644 -D $(tmppc) $(DESTDIR)/$(PKGCONFIGDIR)/kvm-kmod.pc

modules_install:
	$(MAKE) -C $(KERNELDIR) M=`pwd` INSTALL_MOD_PATH=$(DESTDIR)/$(INSTALL_MOD_PATH) $@

install: install-hdr modules_install
	install -m 644 -D scripts/65-kvm.rules $(DESTDIR)/etc/udev/rules.d/65-kvm.rules

tmpspec = .tmp.kvm-kmod.spec

rpm-topdir := $$(pwd)/rpmtop

RPMDIR = $(rpm-topdir)/RPMS

rpm:	all
	mkdir -p $(rpm-topdir)/BUILD $(RPMDIR)/$$(uname -i)
	sed 's/^Release:.*/Release: $(rpmrelease)/; s/^%define kverrel.*/%define kverrel $(KERNELVERSION)/' \
	     kvm-kmod.spec > $(tmpspec)
	rpmbuild --define="kverrel $(KERNELVERSION)" \
		 --define="objdir $$(pwd)/$(ARCH_DIR)" \
		 --define="_rpmdir $(RPMDIR)" \
		 --define="_topdir $(rpm-topdir)" \
		-bb $(tmpspec)

clean:
	$(MAKE) -C $(KERNELDIR) M=`pwd` $@

distclean: clean
	rm -f config.mak include/asm include-compat/asm $(tmppc) $(tmpspec)

.PHONY: all sync sync-kmod sync-hdr install install-hdr rpm clean distclean
