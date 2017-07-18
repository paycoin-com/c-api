/**
 * Copyright (c) 2017 Bitprim developers (see AUTHORS)
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

#ifndef BITPRIM_NODECINT_CHAIN_TRANSACTION_H_
#define BITPRIM_NODECINT_CHAIN_TRANSACTION_H_

#include <stdio.h>
#include <stdint.h>

#include <bitprim/nodecint/visibility.h>
#include <bitprim/nodecint/primitives.h>

#ifdef __cplusplus
extern "C" {
#endif

BITPRIM_EXPORT
void transaction_destruct(transaction_t transaction);

BITPRIM_EXPORT
int transaction_is_valid(transaction_t transaction);

BITPRIM_EXPORT
uint32_t transaction_version(transaction_t transaction);

BITPRIM_EXPORT
void transaction_set_version(transaction_t transaction, uint32_t version);

BITPRIM_EXPORT
hash_t transaction_hash(transaction_t transaction);

BITPRIM_EXPORT
hash_t transaction_hash_sighash_type(transaction_t transaction, uint32_t sighash_type);


BITPRIM_EXPORT
uint32_t transaction_locktime(transaction_t transaction);

BITPRIM_EXPORT
size_t transaction_serialized_size(transaction_t transaction, int wire /*= true*/);

BITPRIM_EXPORT
uint64_t transaction_fees(transaction_t transaction);

BITPRIM_EXPORT
size_t transaction_signature_operations(transaction_t transaction);

BITPRIM_EXPORT
size_t transaction_signature_operations_bip16_active(transaction_t transaction, int /*bool*/ bip16_active);

BITPRIM_EXPORT
uint64_t transaction_total_input_value(transaction_t transaction);

BITPRIM_EXPORT
uint64_t transaction_total_output_value(transaction_t transaction);

BITPRIM_EXPORT
int /*bool*/ transaction_is_coinbase(transaction_t transaction);

BITPRIM_EXPORT
int /*bool*/ transaction_is_null_non_coinbase(transaction_t transaction);

BITPRIM_EXPORT
int /*bool*/ transaction_is_oversized_coinbase(transaction_t transaction);

BITPRIM_EXPORT
int /*bool*/ transaction_is_immature(transaction_t transaction, size_t target_height);

BITPRIM_EXPORT
int /*bool*/ transaction_is_overspent(transaction_t transaction);

BITPRIM_EXPORT
int /*bool*/ transaction_is_double_spend(transaction_t transaction, int /*bool*/ include_unconfirmed);

BITPRIM_EXPORT
int /*bool*/ transaction_is_missing_previous_outputs(transaction_t transaction);

BITPRIM_EXPORT
int /*bool*/ transaction_is_final(transaction_t transaction, size_t block_height, uint32_t block_time);

BITPRIM_EXPORT
int /*bool*/ transaction_is_locktime_conflict(transaction_t transaction);

BITPRIM_EXPORT
size_t transaction_output_count(transaction_t transaction);

BITPRIM_EXPORT
transaction_t transaction_output_nth(transaction_t transaction, size_t n);

BITPRIM_EXPORT
size_t transaction_input_count(transaction_t transaction);

BITPRIM_EXPORT
transaction_t transaction_input_nth(transaction_t transaction, size_t n);



#ifdef __cplusplus
} // extern "C"
#endif

#endif /* BITPRIM_NODECINT_CHAIN_TRANSACTION_H_ */