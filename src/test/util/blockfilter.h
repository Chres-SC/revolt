// Copyright (c) 2019 The Revolt Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef REVOLT_TEST_UTIL_BLOCKFILTER_H
#define REVOLT_TEST_UTIL_BLOCKFILTER_H

#include <blockfilter.h>
class CBlockIndex;

bool ComputeFilter(BlockFilterType filter_type, const CBlockIndex* block_index, BlockFilter& filter);

#endif // REVOLT_TEST_UTIL_BLOCKFILTER_H
