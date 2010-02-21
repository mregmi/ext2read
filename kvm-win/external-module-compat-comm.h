
/*
 * Compatibility header for building as an external module.
 */

/*
 * Avoid picking up the kernel's kvm.h in case we have a newer one.
 */

#include <linux/compiler.h>
#include <linux/version.h>
#include <linux/string.h>
#include <linux/kvm.h>
#include <linux/kvm_para.h>
#include <linux/cpu.h>
#include <linux/time.h>
#include <asm/processor.h>
#include <linux/hrtimer.h>
#include <asm/bitops.h>

/*
 * 2.6.16 does not have GFP_NOWAIT
 */

#include <linux/gfp.h>

#ifndef GFP_NOWAIT
#define GFP_NOWAIT (GFP_ATOMIC & ~__GFP_HIGH)
#endif


/*
 * kvm profiling support needs 2.6.20
 */
#include <linux/profile.h>

#ifndef KVM_PROFILING
#define KVM_PROFILING 1234
#define prof_on       4321
#endif

/*
 * smp_call_function_single() is not exported below 2.6.20, and has different
 * semantics below 2.6.23.  The 'nonatomic' argument was removed in 2.6.27.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)

int kvm_smp_call_function_single(int cpu, void (*func)(void *info),
				 void *info, int wait);
#undef smp_call_function_single
#define smp_call_function_single kvm_smp_call_function_single

#endif

/* on_each_cpu() lost an argument in 2.6.27. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)

#define kvm_on_each_cpu(func, info, wait) on_each_cpu(func, info, 0, wait)

#else

#define kvm_on_each_cpu(func, info, wait) on_each_cpu(func, info, wait)

#endif

/*
 * The cpu hotplug stubs are broken if !CONFIG_CPU_HOTPLUG
 */

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,15)
#define DEFINE_MUTEX(a) DECLARE_MUTEX(a)
#define mutex_lock_interruptible(a) down_interruptible(a)
#define mutex_unlock(a) up(a)
#define mutex_lock(a) down(a)
#define mutex_init(a) init_MUTEX(a)
#define mutex_trylock(a) down_trylock(a)
#define mutex semaphore
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,14)
#ifndef kzalloc
#define kzalloc(size,flags)			\
({						\
	void *__ret = kmalloc(size, flags);	\
	if (__ret)				\
		memset(__ret, 0, size);		\
	__ret;					\
})
#endif
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,17)
#ifndef kmem_cache_zalloc
#define kmem_cache_zalloc(cache,flags)			  \
({							  \
	void *__ret = kmem_cache_alloc(cache, flags);	  \
	if (__ret)                                        \
		memset(__ret, 0, kmem_cache_size(cache)); \
	__ret;                                            \
})
#endif
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)

#ifndef CONFIG_HOTPLUG_CPU
#define register_cpu_notifier(nb) (0)
#endif

#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)
#define nr_cpu_ids NR_CPUS
#endif

#include <linux/miscdevice.h>
#ifndef KVM_MINOR
#define KVM_MINOR 232
#endif

#include <linux/notifier.h>
#ifndef CPU_TASKS_FROZEN

#define CPU_TASKS_FROZEN       0x0010
#define CPU_ONLINE_FROZEN      (CPU_ONLINE | CPU_TASKS_FROZEN)
#define CPU_UP_PREPARE_FROZEN  (CPU_UP_PREPARE | CPU_TASKS_FROZEN)
#define CPU_UP_CANCELED_FROZEN (CPU_UP_CANCELED | CPU_TASKS_FROZEN)
#define CPU_DOWN_PREPARE_FROZEN        (CPU_DOWN_PREPARE | CPU_TASKS_FROZEN)
#define CPU_DOWN_FAILED_FROZEN (CPU_DOWN_FAILED | CPU_TASKS_FROZEN)
#define CPU_DEAD_FROZEN                (CPU_DEAD | CPU_TASKS_FROZEN)

#endif

#ifndef CPU_DYING
#define CPU_DYING 0x000A
#define CPU_DYING_FROZEN (CPU_DYING | CPU_TASKS_FROZEN)
#endif

#include <asm/system.h>

struct inode;
#include <linux/anon_inodes.h>
#define anon_inode_getfd kvm_anon_inode_getfd
int kvm_init_anon_inodes(void);
void kvm_exit_anon_inodes(void);
int anon_inode_getfd(const char *name,
		     const struct file_operations *fops,
		     void *priv , int flags);

/*
 * 2.6.23 removed the cache destructor
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
#  define kmem_cache_create(name, size, align, flags, ctor) \
	kmem_cache_create(name, size, align, flags, ctor, NULL)
#endif

/* HRTIMER_MODE_ABS started life with a different name */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)
#define HRTIMER_MODE_ABS HRTIMER_ABS
#endif

/* div64_u64 is fairly new */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,26)

#define div64_u64 kvm_div64_u64

#ifdef CONFIG_64BIT

static inline uint64_t div64_u64(uint64_t dividend, uint64_t divisor)
{
	return dividend / divisor;
}

#else

uint64_t div64_u64(uint64_t dividend, uint64_t divisor);

#endif

#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)

#ifdef RHEL_RELEASE_CODE
#if RHEL_RELEASE_CODE >= RHEL_RELEASE_VERSION(5,2)
#define RHEL_BOOL 1
#endif
#endif

#ifndef RHEL_BOOL

typedef _Bool bool;

#define false 0
#define true 1

#endif

#endif

/*
 * PF_VCPU is a Linux 2.6.24 addition
 */

#include <linux/sched.h>

#ifndef PF_VCPU
#define PF_VCPU 0
#endif

/*
 * smp_call_function_mask() is not defined/exported below 2.6.24 on all
 * targets and below 2.6.26 on x86-64
 */

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24) || \
    (defined CONFIG_X86_64 && LINUX_VERSION_CODE < KERNEL_VERSION(2,6,26))

int kvm_smp_call_function_mask(cpumask_t mask, void (*func) (void *info),
			       void *info, int wait);

#define smp_call_function_mask kvm_smp_call_function_mask

void kvm_smp_send_reschedule(int cpu);

#else

#define kvm_smp_send_reschedule smp_send_reschedule

#endif

/* empty_zero_page isn't exported in all kernels */
#include <asm/pgtable.h>

#define empty_zero_page kvm_empty_zero_page

static char empty_zero_page[PAGE_SIZE];

static inline void blahblah(void)
{
	(void)empty_zero_page[0];
}

/* __mmdrop() is not exported before 2.6.25 */
#include <linux/sched.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,25)

#define mmdrop(x) do { (void)(x); } while (0)
#define mmget(x) do { (void)(x); } while (0)

#else

#define mmget(x) do { atomic_inc(x); } while (0)

#endif

/* pagefault_enable(), page_fault_disable() - 2.6.20 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
#  define KVM_NEED_PAGEFAULT_DISABLE 1
#  ifdef RHEL_RELEASE_CODE
#    if RHEL_RELEASE_CODE >= RHEL_RELEASE_VERSION(5,3)
#      undef KVM_NEED_PAGEFAULT_DISABLE
#    endif
#  endif
#endif

#ifdef KVM_NEED_PAGEFAULT_DISABLE

static inline void pagefault_disable(void)
{
	inc_preempt_count();
	/*
	 * make sure to have issued the store before a pagefault
	 * can hit.
	 */
	barrier();
}

static inline void pagefault_enable(void)
{
	/*
	 * make sure to issue those last loads/stores before enabling
	 * the pagefault handler again.
	 */
	barrier();
	dec_preempt_count();
	/*
	 * make sure we do..
	 */
	barrier();
	preempt_check_resched();
}

#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)
#include <asm/uaccess.h>
#else
#include <linux/uaccess.h>
#endif

/* vm ops ->fault() was introduced in 2.6.23. */
#include <linux/mm.h>

#ifdef KVM_MAIN
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)

struct vm_fault {
	unsigned int flags;
	pgoff_t pgoff;
	void __user *virtual_address;
	struct page *page;
};

static int kvm_vcpu_fault(struct vm_area_struct *vma, struct vm_fault *vmf);
static int kvm_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf);

static inline struct page *kvm_nopage_to_fault(
	int (*fault)(struct vm_area_struct *vma, struct vm_fault *vmf),
	struct vm_area_struct *vma,
	unsigned long address,
	int *type)
{
	struct vm_fault vmf;
	int ret;

	vmf.pgoff = ((address - vma->vm_start) >> PAGE_SHIFT) + vma->vm_pgoff;
	vmf.virtual_address = (void __user *)address;
	ret = fault(vma, &vmf);
	if (ret)
		return NOPAGE_SIGBUS;
	*type = VM_FAULT_MINOR;
	return vmf.page;
}

static inline struct page *__kvm_vcpu_fault(struct vm_area_struct *vma,
					    unsigned long address,
					    int *type)
{
	return kvm_nopage_to_fault(kvm_vcpu_fault, vma, address, type);
}

static inline struct page *__kvm_vm_fault(struct vm_area_struct *vma,
					  unsigned long address,
					  int *type)
{
	return kvm_nopage_to_fault(kvm_vm_fault, vma, address, type);
}

#define VMA_OPS_FAULT(x) nopage
#define VMA_OPS_FAULT_FUNC(x) __##x

#else

#define VMA_OPS_FAULT(x) x
#define VMA_OPS_FAULT_FUNC(x) x

#endif
#endif

/* simple vfs attribute getter signature has changed to add a return code */

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,25)

#define MAKE_SIMPLE_ATTRIBUTE_GETTER(x)       \
	static u64 x(void *v)                 \
	{				      \
		u64 ret = 0;		      \
					      \
		__##x(v, &ret);		      \
		return ret;		      \
	}

#else

#define MAKE_SIMPLE_ATTRIBUTE_GETTER(x)       \
	static int x(void *v, u64 *val)	      \
	{				      \
		return __##x(v, val);	      \
	}

#endif

/* set_kset_name() is gone in 2.6.25 */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,25)

#define set_kset_name(x) .name = x

#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,25)
#ifndef FASTCALL
#define FASTCALL(x)	x
#define fastcall
#endif
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)

unsigned kvm_get_tsc_khz(void);
#define kvm_tsc_khz (kvm_get_tsc_khz())

#else

#define kvm_tsc_khz tsc_khz

#endif

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,21)

#include <linux/ktime.h>
#include <linux/hrtimer.h>

#define ktime_get kvm_ktime_get

static inline ktime_t ktime_get(void)
{
	struct timespec now;

	ktime_get_ts(&now);

	return timespec_to_ktime(now);
}

#endif

/* __aligned arrived in 2.6.21 */
#ifndef __aligned
#define __aligned(x) __attribute__((__aligned__(x)))
#endif

#include <linux/mm.h>

/* The shrinker API changed in 2.6.23 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)

struct kvm_shrinker {
	int (*shrink)(int nr_to_scan, gfp_t gfp_mask);
	int seeks;
	struct shrinker *kshrinker;
};

static inline void register_shrinker(struct kvm_shrinker *shrinker)
{
	shrinker->kshrinker = set_shrinker(shrinker->seeks, shrinker->shrink);
}

static inline void unregister_shrinker(struct kvm_shrinker *shrinker)
{
	if (shrinker->kshrinker)
		remove_shrinker(shrinker->kshrinker);
}

#define shrinker kvm_shrinker

#endif

/* clocksource */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)
static inline u32 clocksource_khz2mult(u32 khz, u32 shift_constant)
{
	/*  khz = cyc/(Million ns)
	 *  mult/2^shift  = ns/cyc
	 *  mult = ns/cyc * 2^shift
	 *  mult = 1Million/khz * 2^shift
	 *  mult = 1000000 * 2^shift / khz
	 *  mult = (1000000<<shift) / khz
	 */
	u64 tmp = ((u64)1000000) << shift_constant;

	tmp += khz/2; /* round for do_div */
	do_div(tmp, khz);

	return (u32)tmp;
}
#else
#include <linux/clocksource.h>
#endif

/* manually export hrtimer_init/start/cancel */
#include <linux/kallsyms.h>
extern void (*hrtimer_init_p)(struct hrtimer *timer, clockid_t which_clock,
			      enum hrtimer_mode mode);
extern int (*hrtimer_start_p)(struct hrtimer *timer, ktime_t tim,
			      const enum hrtimer_mode mode);
extern int (*hrtimer_cancel_p)(struct hrtimer *timer);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,17) && defined(CONFIG_KALLSYMS)
static inline void hrtimer_kallsyms_resolve(void)
{
	hrtimer_init_p = (void *) kallsyms_lookup_name("hrtimer_init");
	BUG_ON(!hrtimer_init_p);
	hrtimer_start_p = (void *) kallsyms_lookup_name("hrtimer_start");
	BUG_ON(!hrtimer_start_p);
	hrtimer_cancel_p = (void *) kallsyms_lookup_name("hrtimer_cancel");
	BUG_ON(!hrtimer_cancel_p);
}
#else
static inline void hrtimer_kallsyms_resolve(void)
{
	hrtimer_init_p = hrtimer_init;
	hrtimer_start_p = hrtimer_start;
	hrtimer_cancel_p = hrtimer_cancel;
}
#endif

/* handle old hrtimer API with data pointer */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,17)
static inline void hrtimer_data_pointer(struct hrtimer *timer)
{
	timer->data = (void *)timer;
}
#else
static inline void hrtimer_data_pointer(struct hrtimer *timer) {}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22)

#define ns_to_timespec kvm_ns_to_timespec

struct timespec kvm_ns_to_timespec(const s64 nsec);

#endif

/* work_struct lost the 'data' field in 2.6.20 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)

#define kvm_INIT_WORK(work, handler) \
	INIT_WORK(work, (void (*)(void *))handler, work)

#else

#define kvm_INIT_WORK(work, handler) INIT_WORK(work, handler)

#endif

/* cancel_work_sync() was flush_work() in 2.6.21 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22)

static inline int cancel_work_sync(struct work_struct *work)
{
	/*
	 * FIXME: actually cancel.  How?  Add own implementation of workqueues?
	 */
	return 0;
}

/* ... and it returned void before 2.6.23 */
#elif LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)

#define cancel_work_sync(work) ({ cancel_work_sync(work); 0; })

#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)

static inline void flush_work(struct work_struct *work)
{
	cancel_work_sync(work);
}

#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)

struct pci_dev;

struct pci_dev *pci_get_bus_and_slot(unsigned int bus, unsigned int devfn);

#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,17)
#include <linux/relayfs_fs.h>
#else
#include <linux/relay.h>
#endif

/* relay_open() interface has changed on 2.6.21 */

struct rchan *kvm_relay_open(const char *base_filename,
			 struct dentry *parent,
			 size_t subbuf_size,
			 size_t n_subbufs,
			 struct rchan_callbacks *cb,
			 void *private_data);

#else

#define kvm_relay_open relay_open

#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)

static inline int get_user_pages_fast(unsigned long start, int nr_pages,
				      int write, struct page **pages)
{
	int npages;

	down_read(&current->mm->mmap_sem);
	npages = get_user_pages(current, current->mm, start, nr_pages, write,
				0, pages, NULL);
	up_read(&current->mm->mmap_sem);

	return npages;
}

#endif

/* spin_needbreak() was called something else in 2.6.24 */
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,24)

#define spin_needbreak need_lockbreak

#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)

static inline void kvm_hrtimer_add_expires_ns(struct hrtimer *timer, u64 delta)
{
	timer->expires = ktime_add_ns(timer->expires, delta);
}

static inline ktime_t kvm_hrtimer_get_expires(struct hrtimer *timer)
{
	return timer->expires;
}

static inline u64 kvm_hrtimer_get_expires_ns(struct hrtimer *timer)
{
	return ktime_to_ns(timer->expires);
}

static inline void kvm_hrtimer_start_expires(struct hrtimer *timer, int mode)
{
	hrtimer_start_p(timer, timer->expires, mode);
}

static inline ktime_t kvm_hrtimer_expires_remaining(const struct hrtimer *timer)
{
    return ktime_sub(timer->expires, timer->base->get_time());
}

#else

#define kvm_hrtimer_add_expires_ns hrtimer_add_expires_ns
#define kvm_hrtimer_get_expires hrtimer_get_expires
#define kvm_hrtimer_get_expires_ns hrtimer_get_expires_ns
#define kvm_hrtimer_start_expires hrtimer_start_expires
#define kvm_hrtimer_expires_remaining hrtimer_expires_remaining

#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)

static inline int pci_reset_function(struct pci_dev *dev)
{
	return 0;
}

#endif

#include <linux/interrupt.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)

typedef irqreturn_t (*kvm_irq_handler_t)(int, void *);
int kvm_request_irq(unsigned int a, kvm_irq_handler_t handler, unsigned long c,
		    const char *d, void *e);
void kvm_free_irq(unsigned int irq, void *dev_id);

#else

#define kvm_request_irq request_irq
#define kvm_free_irq free_irq

#endif

/* dynamically allocated cpu masks introduced in 2.6.28 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)

typedef cpumask_t cpumask_var_t[1];

static inline bool alloc_cpumask_var(cpumask_var_t *mask, gfp_t flags)
{
	return 1;
}

static inline void free_cpumask_var(cpumask_var_t mask)
{
}

static inline void cpumask_clear(cpumask_var_t mask)
{
	cpus_clear(*mask);
}

static inline void cpumask_set_cpu(int cpu, cpumask_var_t mask)
{
	cpu_set(cpu, *mask);
}

static inline int smp_call_function_many(cpumask_var_t cpus,
					 void (*func)(void *data), void *data,
					 int sync)
{
	return smp_call_function_mask(*cpus, func, data, sync);
}

static inline int cpumask_empty(cpumask_var_t mask)
{
	return cpus_empty(*mask);
}

static inline int cpumask_test_cpu(int cpu, cpumask_var_t mask)
{
	return cpu_isset(cpu, *mask);
}

static inline void cpumask_clear_cpu(int cpu, cpumask_var_t mask)
{
	cpu_clear(cpu, *mask);
}

#define cpu_online_mask (&cpu_online_map)

#endif

/* A zeroing constructor was added late 2.6.30 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30)

static inline bool zalloc_cpumask_var(cpumask_var_t *mask, gfp_t flags)
{
	bool ret;

	ret = alloc_cpumask_var(mask, flags);
	if (ret)
		cpumask_clear(*mask);
	return ret;
}

#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29)

#define IF_ANON_INODES_DOES_REFCOUNTS(x)

#else

#define IF_ANON_INODES_DOES_REFCOUNTS(x) x

#endif


/* Macro introduced only on newer kernels: */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)
#define marker_synchronize_unregister() synchronize_sched()
#endif

/* pci_dev.msi_enable was introduced in 2.6.18 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)

struct pci_dev;

int kvm_pcidev_msi_enabled(struct pci_dev *dev);

#else

#define kvm_pcidev_msi_enabled(dev)	(dev)->msi_enabled

#endif

/* compound_head() was introduced in 2.6.22 */

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22)
#  define NEED_COMPOUND_HEAD 1
#  ifdef RHEL_RELEASE_CODE
#    if RHEL_RELEASE_CODE >= RHEL_RELEASE_VERSION(5,2)
#      undef NEED_COMPOUND_HEAD
#    endif
#  endif
#endif

#ifdef NEED_COMPOUND_HEAD

static inline struct page *compound_head(struct page *page)
{
	if (PageCompound(page))
		page = (struct page *)page_private(page);
	return page;
}

#endif

#include <linux/iommu.h>
#ifndef IOMMU_CACHE

#define IOMMU_CACHE	(4)
#define IOMMU_CAP_CACHE_COHERENCY	0x1
static inline int iommu_domain_has_cap(struct iommu_domain *domain,
				       unsigned long cap)
{
	return 0;
}

#endif

#include <linux/file.h>

/* eventfd_fget() will be introduced in 2.6.32 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)

static inline struct file *eventfd_fget(int fd)
{
    return fget(fd);
}

#endif

/* srcu was born in 2.6.19 */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)

#define kvm_init_srcu_struct init_srcu_struct
#define kvm_cleanup_srcu_struct cleanup_srcu_struct
#define kvm_srcu_read_lock srcu_read_lock
#define kvm_srcu_read_unlock srcu_read_unlock
#define kvm_synchronize_srcu synchronize_srcu
#define kvm_srcu_batches_completed srcu_batches_completed

#endif

/* tracepoints were introduced in 2.6.28, but changed in 2.6.30 */

#include <linux/tracepoint.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31)

struct tracepoint;

#undef DECLARE_TRACE
#undef DEFINE_TRACE
#undef PARAMS
#undef TP_PROTO
#undef TP_ARGS
#undef EXPORT_TRACEPOINT_SYMBOL
#undef EXPORT_TRACEPOINT_SYMBOL_GPL

#define DECLARE_TRACE(name, proto, args)				\
	static inline void _do_trace_##name(struct tracepoint *tp, proto) \
	{ }								\
	static inline void trace_##name(proto)				\
	{ }								\
	static inline int register_trace_##name(void (*probe)(proto))	\
	{								\
		return -ENOSYS;						\
	}								\
	static inline int unregister_trace_##name(void (*probe)(proto))	\
	{								\
		return -ENOSYS;						\
	}

#define tracepoint_update_probe_range(begin, end) do {} while (0)

#define DEFINE_TRACE(name)
#define EXPORT_TRACEPOINT_SYMBOL_GPL(name)
#define EXPORT_TRACEPOINT_SYMBOL(name)

#define PARAMS(args...) args
#define TP_PROTO(args...)	args
#define TP_ARGS(args...)		args

#define TRACE_EVENT(name, proto, args, struct, assign, print)	\
	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))

#undef tracepoint_synchronize_unregister
#define tracepoint_synchronize_unregister() do {} while (0)

#endif

#include <linux/ftrace_event.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31)

struct trace_print_flags {
	unsigned long		mask;
	const char		*name;
};

#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31)

#define alloc_pages_exact_node alloc_pages_node

#endif

/* eventfd accessors, new in 2.6.31 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31)

#include <linux/eventfd.h>
#include <linux/fs.h>

struct eventfd_ctx;

static inline struct eventfd_ctx *eventfd_ctx_get(struct eventfd_ctx *ctx)
{
	struct file *filp = (struct file *)ctx;

	get_file(filp);
	return ctx;
}

static inline void eventfd_ctx_put(struct eventfd_ctx *ctx)
{
	struct file *filp = (struct file *)ctx;

	fput(filp);
}

static inline struct eventfd_ctx *eventfd_ctx_fdget(int fd)
{
	struct file *filp = eventfd_fget(fd);

	return (struct eventfd_ctx *)filp;
}

static inline struct eventfd_ctx *eventfd_ctx_fileget(struct file *file)
{
	return (struct eventfd_ctx *)file;
}

static inline int kvm_eventfd_signal(struct eventfd_ctx *ctx, int n)
{
	return -ENOSYS;
}

#else

#define kvm_eventfd_signal eventfd_signal

#endif

#include <linux/hugetlb.h>

/* vma_kernel_pagesize, exported since 2.6.32 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)

#if defined(CONFIG_HUGETLB_PAGE) && LINUX_VERSION_CODE > KERNEL_VERSION(2,6,26)
static inline
unsigned long kvm_vma_kernel_pagesize(struct vm_area_struct *vma)
{
	struct hstate *hstate;

	if (!is_vm_hugetlb_page(vma))
		return PAGE_SIZE;

	hstate = hstate_vma(vma);

	return 1UL << (hstate->order + PAGE_SHIFT);
}
#else /* !CONFIG_HUGETLB_SIZE || <= 2.6.26 */
#define kvm_vma_kernel_pagesize(v) PAGE_SIZE
#endif

#else /* >= 2.6.32 */

#define kvm_vma_kernel_pagesize vma_kernel_pagesize

#endif

#ifndef printk_once
/*
 * Print a one-time message (analogous to WARN_ONCE() et al):
 */
#define printk_once(x...) ({			\
	static int __print_once = 1;		\
						\
	if (__print_once) {			\
		__print_once = 0;		\
		printk(x);			\
	}					\
})
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,33) && !defined(CONFIG_CPU_FREQ)
static inline unsigned int cpufreq_get(unsigned int cpu)
{
	return 0;
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)
int schedule_hrtimeout(ktime_t *expires, const enum hrtimer_mode mode);
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
#ifndef CONFIG_MMU_NOTIFIER
struct mmu_notifier {};
#endif
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static inline void hlist_del_init_rcu(struct hlist_node *n)
{
	if (!hlist_unhashed(n)) {
		__hlist_del(n);
		n->pprev = NULL;
	}
}
#endif

#ifndef CONFIG_USER_RETURN_NOTIFIER

#include <linux/percpu.h>

struct kvm_user_return_notifier {
	void (*on_user_return)(struct kvm_user_return_notifier *urn);
};

DECLARE_PER_CPU(struct kvm_user_return_notifier *, kvm_urn);

static inline void
kvm_user_return_notifier_register(struct kvm_user_return_notifier *urn)
{
	__get_cpu_var(kvm_urn) = urn;
}

static inline void
kvm_user_return_notifier_unregister(struct kvm_user_return_notifier *urn)
{
	__get_cpu_var(kvm_urn) = NULL;
}

static inline void kvm_fire_urn(void)
{
	struct kvm_user_return_notifier *urn = __get_cpu_var(kvm_urn);

	if (urn)
		urn->on_user_return(urn);
}

#else /* CONFIG_USER_RETURN_NOTIFIER */

#define kvm_user_return_notifier		user_return_notifier
#define kvm_user_return_notifier_register	user_return_notifier_register
#define kvm_user_return_notifier_unregister	user_return_notifier_unregister

static inline void kvm_fire_urn(void) {}

#endif /* CONFIG_USER_RETURN_NOTIFIER */
