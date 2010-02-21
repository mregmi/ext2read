#ifndef KVM_UNIFDEF_H
#define KVM_UNIFDEF_H

#ifdef __i386__
#ifndef CONFIG_X86_32
#define CONFIG_X86_32 1
#endif
#endif

#ifdef __x86_64__
#ifndef CONFIG_X86_64
#define CONFIG_X86_64 1
#endif
#endif

#if defined(__i386__) || defined (__x86_64__)
#ifndef CONFIG_X86
#define CONFIG_X86 1
#endif
#endif

#ifdef __ia64__
#ifndef CONFIG_IA64
#define CONFIG_IA64 1
#endif
#endif

#ifdef __PPC__
#ifndef CONFIG_PPC
#define CONFIG_PPC 1
#endif
#endif

#ifdef __s390__
#ifndef CONFIG_S390
#define CONFIG_S390 1
#endif
#endif

#endif
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef __KVM_TYPES_H__
#define __KVM_TYPES_H__

#include <asm/types.h>

/*
 * Address types:
 *
 *  gva - guest virtual address
 *  gpa - guest physical address
 *  gfn - guest frame number
 *  hva - host virtual address
 *  hpa - host physical address
 *  hfn - host frame number
 */

typedef unsigned long  gva_t;
typedef u64            gpa_t;
typedef unsigned long  gfn_t;

typedef unsigned long  hva_t;
typedef u64            hpa_t;
typedef unsigned long  hfn_t;

typedef hfn_t pfn_t;

union kvm_ioapic_redirect_entry {
	u64 bits;
	struct {
		u8 vector;
		u8 delivery_mode:3;
		u8 dest_mode:1;
		u8 delivery_status:1;
		u8 polarity:1;
		u8 remote_irr:1;
		u8 trig_mode:1;
		u8 mask:1;
		u8 reserve:7;
		u8 reserved[4];
		u8 dest_id;
	} fields;
};

struct kvm_lapic_irq {
	u32 vector;
	u32 delivery_mode;
	u32 dest_mode;
	u32 level;
	u32 trig_mode;
	u32 shorthand;
	u32 dest_id;
};

#endif /* __KVM_TYPES_H__ */
