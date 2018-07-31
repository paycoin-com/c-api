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

#ifndef BITPRIM_NODECINT_CHAIN_BLOCK_LIST_H_
#define BITPRIM_NODECINT_CHAIN_BLOCK_LIST_H_

#include <stdint.h>

#include <bitprim/nodecint/primitives.h>
#include <bitprim/nodecint/visibility.h>

#ifdef __cplusplus
extern "C" {
#endif

BITPRIM_EXPORT
block_list_t chain_block_list_construct_default(void);

BITPRIM_EXPORT
block_list_t chain_block_list_construct_reserve(uint64_t /*size_t*/ n);

BITPRIM_EXPORT
void chain_block_list_push_back(block_list_t list, block_t block);

// BITPRIM_EXPORT
// void chain_block_list_push_back_const(block_list_t list, const block_t block);

BITPRIM_EXPORT
void chain_block_list_destruct(block_list_t list);

BITPRIM_EXPORT
uint64_t /*size_t*/ chain_block_list_count(block_list_t list);

BITPRIM_EXPORT
block_t chain_block_list_nth(block_list_t list, uint64_t /*size_t*/ n);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* BITPRIM_NODECINT_CHAIN_BLOCK_LIST_H_ */
