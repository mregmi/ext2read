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
#include <linux/kvm_host.h>
#include <linux/kvm.h>
#include <linux/hrtimer.h>
#include <asm/atomic.h>
#include "kvm_timer.h"

static int __kvm_timer_fn(struct kvm_vcpu *vcpu, struct kvm_timer *ktimer)
{
	int restart_timer = 0;
	wait_queue_head_t *q = &vcpu->wq;

	/*
	 * There is a race window between reading and incrementing, but we do
	 * not care about potentially loosing timer events in the !reinject
	 * case anyway.
	 */
	if (ktimer->reinject || !atomic_read(&ktimer->pending)) {
		atomic_inc(&ktimer->pending);
		/* FIXME: this code should not know anything about vcpus */
		set_bit(KVM_REQ_PENDING_TIMER, &vcpu->requests);
	}

	if (waitqueue_active(q))
		wake_up_interruptible(q);

	if (ktimer->t_ops->is_periodic(ktimer)) {
		kvm_hrtimer_add_expires_ns(&ktimer->timer, ktimer->period);
		restart_timer = 1;
	}

	return restart_timer;
}

enum hrtimer_restart kvm_timer_fn(struct hrtimer *data)
{
	int restart_timer;
	struct kvm_vcpu *vcpu;
	struct kvm_timer *ktimer = container_of(data, struct kvm_timer, timer);

	vcpu = ktimer->vcpu;
	if (!vcpu)
		return HRTIMER_NORESTART;

	restart_timer = __kvm_timer_fn(vcpu, ktimer);
	if (restart_timer)
		return HRTIMER_RESTART;
	else
		return HRTIMER_NORESTART;
}

