#ifndef _ASM_X86_KVM_PARA_H
#define _ASM_X86_KVM_PARA_H

#include <linux/types.h>

/* This CPUID returns the signature 'KVMKVMKVM' in ebx, ecx, and edx.  It
 * should be used to determine that a VM is running under KVM.
 */
#define KVM_CPUID_SIGNATURE	0x40000000

/* This CPUID returns a feature bitmap in eax.  Before enabling a particular
 * paravirtualization, the appropriate feature bit should be checked.
 */
#define KVM_CPUID_FEATURES	0x40000001
#define KVM_FEATURE_CLOCKSOURCE		0
#define KVM_FEATURE_NOP_IO_DELAY	1
#define KVM_FEATURE_MMU_OP		2

#define MSR_KVM_WALL_CLOCK  0x11
#define MSR_KVM_SYSTEM_TIME 0x12

#define KVM_MAX_MMU_OP_BATCH           32

/* Operations for KVM_HC_MMU_OP */
#define KVM_MMU_OP_WRITE_PTE            1
#define KVM_MMU_OP_FLUSH_TLB	        2
#define KVM_MMU_OP_RELEASE_PT	        3

/* Payload for KVM_HC_MMU_OP */
struct kvm_mmu_op_header {
	__u32 op;
	__u32 pad;
};

struct kvm_mmu_op_write_pte {
	struct kvm_mmu_op_header header;
	__u64 pte_phys;
	__u64 pte_val;
};

struct kvm_mmu_op_flush_tlb {
	struct kvm_mmu_op_header header;
};

struct kvm_mmu_op_release_pt {
	struct kvm_mmu_op_header header;
	__u64 pt_phys;
};


#endif /* _ASM_X86_KVM_PARA_H */
