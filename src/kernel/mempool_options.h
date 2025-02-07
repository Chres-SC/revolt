// Copyright (c) 2022 The Revolt Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef REVOLT_KERNEL_MEMPOOL_OPTIONS_H
#define REVOLT_KERNEL_MEMPOOL_OPTIONS_H

#include <kernel/mempool_limits.h>

#include <chrono>
#include <cstdint>

class CBlockPolicyEstimator;

/** Default for -maxmempool, maximum megabytes of mempool memory usage */
static constexpr unsigned int DEFAULT_MAX_MEMPOOL_SIZE_MB{300};
/** Default for -mempoolexpiry, expiration time for mempool transactions in hours */
static constexpr unsigned int DEFAULT_MEMPOOL_EXPIRY_HOURS{336};
/** Default for -mempoolfullrbf, if the transaction replaceability signaling is ignored */
static constexpr bool DEFAULT_MEMPOOL_FULL_RBF{false};

namespace kernel {
/**
 * Options struct containing options for constructing a CTxMemPool. Default
 * constructor populates the struct with sane default values which can be
 * modified.
 *
 * Most of the time, this struct should be referenced as CTxMemPool::Options.
 */
struct MemPoolOptions {
    /* Used to estimate appropriate transaction fees. */
    CBlockPolicyEstimator* estimator{nullptr};
    /* The ratio used to determine how often sanity checks will run.  */
    int check_ratio{0};
    int64_t max_size_bytes{DEFAULT_MAX_MEMPOOL_SIZE_MB * 1'000'000};
    std::chrono::seconds expiry{std::chrono::hours{DEFAULT_MEMPOOL_EXPIRY_HOURS}};
    bool full_rbf{DEFAULT_MEMPOOL_FULL_RBF};
    MemPoolLimits limits{};
};
} // namespace kernel

#endif // REVOLT_KERNEL_MEMPOOL_OPTIONS_H
