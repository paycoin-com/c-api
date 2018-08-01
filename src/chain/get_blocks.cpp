/**
 * Copyright (c) 2016-2018 Bitprim Inc.
 *
 * This file is part of Bitprim.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <bitprim/nodecint/chain/block.h>

#include <bitcoin/bitcoin/message/get_blocks.hpp>

#include <bitprim/nodecint/conversions.hpp>
#include <bitprim/nodecint/helpers.hpp>

BITPRIM_CONV_DEFINE(chain, get_blocks_t, libbitcoin::message::get_blocks, get_blocks)

// ---------------------------------------------------------------------------
extern "C" {

get_blocks_t chain_get_blocks_construct_default() {
    return new libbitcoin::message::get_blocks();
}

get_blocks_t chain_get_blocks_construct(hash_list_t start, hash_t stop) {
    auto const& start_cpp = core_hash_list_const_cpp(start);
    auto stop_cpp = bitprim::to_array(stop.hash);
    return new libbitcoin::message::get_blocks(start_cpp, stop_cpp);
}

void chain_get_blocks_destruct(get_blocks_t get_b) {
    delete &chain_get_blocks_cpp(get_b);
}

hash_list_t chain_get_blocks_start_hashes(get_blocks_t get_b) {
    auto& list = chain_get_blocks_cpp(get_b).start_hashes();
    return core_hash_list_construct_from_cpp(list);
}

void chain_get_blocks_set_start_hashes(get_blocks_t get_b, hash_list_t value) {
    auto const& value_cpp = core_hash_list_const_cpp(value);
    chain_get_blocks_cpp(get_b).set_start_hashes(value_cpp);
}

hash_t chain_get_blocks_stop_hash(get_blocks_t get_b) {
    auto& stop = chain_get_blocks_cpp(get_b).stop_hash();
    return bitprim::to_hash_t(stop);
}

void chain_get_blocks_stop_hash_out(get_blocks_t get_b, hash_t* out_stop_hash) {
    auto& stop = chain_get_blocks_cpp(get_b).stop_hash();
    bitprim::copy_c_hash(stop, out_stop_hash);
}

//TODO(fernando): pass the hash_t by reference (pointer)
void chain_get_blocks_set_stop_hash(get_blocks_t get_b, hash_t value) {
    auto value_cpp = bitprim::to_array(value.hash);
    chain_get_blocks_cpp(get_b).set_stop_hash(value_cpp);
}

bool_t chain_get_blocks_is_valid(get_blocks_t get_b) {
    return bitprim::bool_to_int(chain_get_blocks_cpp(get_b).is_valid());
}

void chain_get_blocks_reset(get_blocks_t get_b) {
    chain_get_blocks_cpp(get_b).reset();
}

uint64_t /*size_t*/ chain_get_blocks_serialized_size(get_blocks_t get_b, uint32_t version) {
    return chain_get_blocks_cpp(get_b).serialized_size(version);
}

} // extern "C"
