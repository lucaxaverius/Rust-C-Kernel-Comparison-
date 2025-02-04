#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/ktime.h>
#include <linux/delay.h>  // For udelay or mdelay

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luca Saverio Esposito");
MODULE_DESCRIPTION("SpinLock Lock/Unlock Performance Test");

#define NUM_ITERATIONS 15000000

//#define NUM_EXECUTION 30
#define COUNT 3

int spinlock_benchmark_test(int count);
EXPORT_SYMBOL(spinlock_benchmark_test);
int spinlock_test_init(void);
EXPORT_SYMBOL(spinlock_test_init);

void lock_unlock_spin(int * count);
EXPORT_SYMBOL(lock_unlock_spin);


static spinlock_t test_spinlock;

int spinlock_test_init(void)
{
    int ret;
    pr_info("C-SpinLock-Benchmark: Initializing spinlock Lock/Unlock Performance Test...\n");
    
    pr_info("C-SpinLock-Benchmark: Number of iterations: %d\n", NUM_ITERATIONS);

    // Initialize the spinlock
    spin_lock_init(&test_spinlock);

    ret = spinlock_benchmark_test(COUNT);
    
    pr_info("C-SpinLock-Benchmark: Iteration %d-th ended\n", COUNT);
    pr_info("C-SpinLock-Benchmark: Test module completed.\n");

    return ret;
}

void lock_unlock_spin(int* value){
    unsigned long flags;
    spin_lock_irqsave(&test_spinlock, flags);
    (*value)++;
    spin_unlock_irqrestore(&test_spinlock, flags);
}


int spinlock_benchmark_test(int count){
    ktime_t start, end, lock_start, lock_end;
    s64 total_time_ms = 0, lock_time_ns = 0;
    s64 min_time_ns = LLONG_MAX, max_time_ns = 0;
    s64 elapsed_ns;
    int i;
    int j=0;


    // Record start time
    start = ktime_get();

    for (i = 0; i < NUM_ITERATIONS; i++) {
        // Measure the time for lock/unlock cycle

        lock_start = ktime_get();
        lock_unlock_spin(&j);
        lock_end = ktime_get();

        elapsed_ns = ktime_to_ns(ktime_sub(lock_end, lock_start));
        lock_time_ns += elapsed_ns;

        // Update min and max times
        if (elapsed_ns < min_time_ns)
            min_time_ns = elapsed_ns;
        if (elapsed_ns > max_time_ns)
            max_time_ns = elapsed_ns;
    }

    // Record end time
    end = ktime_get();
    total_time_ms = ktime_to_ms(ktime_sub(end, start));

    // Log results
    pr_info("C-SpinLock-Benchmark: spinlock %d Test Completed\n",count);
    pr_info("C-SpinLock-Benchmark: Total time: %lld ms\n", total_time_ms);
    pr_info("C-SpinLock-Benchmark: Total lock/unlock time: %lld ms\n", ktime_to_ms(lock_time_ns));
    pr_info("C-SpinLock-Benchmark: Average time per lock/unlock: %lld ns\n", lock_time_ns / NUM_ITERATIONS);
    pr_info("C-SpinLock-Benchmark: Minimum time per lock/unlock: %lld ns\n", min_time_ns);
    pr_info("C-SpinLock-Benchmark: Maximum time per lock/unlock: %lld ns\n", max_time_ns);
   
    return 0;
}

static void __exit spinlock_test_exit(void)
{
    pr_info("C-SpinLock-Benchmark: Exiting spinlock Lock/Unlock Performance Test.\n");
}

module_init(spinlock_test_init);
module_exit(spinlock_test_exit);
