// SPDX-License-Identifier: GPL-2.0

//! Test module for Mutex.

use kernel::prelude::*;
use kernel::sync::{Mutex, new_mutex};
use kernel::time::{Ktime, ktime_to_ns};

module! {
    type: MutexTestModule,
    name: "mutex_lock_test",
    author: "Luca Saverio Esposito",
    description: "Mutex Lock/Unlock Performance Test",
    license: "GPL",
}

struct MutexTestModule;

impl kernel::Module for MutexTestModule {
    fn init(_module: &'static ThisModule) -> Result<Self> {
        pr_info!("Initializing Mutex Lock/Unlock Performance Test...\n");

        let mutex = new_mutex!((), GFP_KERNEL);
        let num_iterations = 100_000;
        let mut total_lock_time = 0i64;
        let mut min_time = i64::MAX;
        let mut max_time = i64::MIN;

        let start = Ktime::ktime_get();
        for _ in 0..num_iterations {
            let lock_start = Ktime::ktime_get();

            {
                let _guard = mutex.lock(); // Lock and unlock the mutex
            }

            let lock_end = Ktime::ktime_get();
            let elapsed = ktime_to_ns(lock_end - lock_start);

            total_lock_time += elapsed;
            if elapsed < min_time {
                min_time = elapsed;
            }
            if elapsed > max_time {
                max_time = elapsed;
            }
        }
        let end = Ktime::ktime_get();
        let total_time = ktime_to_ns(end - start);

        pr_info!("Mutex Test Completed\n");
        pr_info!("Total time: {} ns\n", total_time);
        pr_info!("Total lock/unlock time: {} ns\n", total_lock_time);
        pr_info!(
            "Average time per lock/unlock: {} ns\n",
            total_lock_time / num_iterations as i64
        );
        pr_info!("Minimum time per lock/unlock: {} ns\n", min_time);
        pr_info!("Maximum time per lock/unlock: {} ns\n", max_time);

        Ok(Self)
    }
}

impl Drop for MutexTestModule {
    fn drop(&mut self) {
        pr_info!("Exiting Mutex Lock/Unlock Performance Test.\n");
    }
}
