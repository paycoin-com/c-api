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

#include <bitprim/nodecint/chain/utxo_list.h>

#include <vector>

#include <bitprim/nodecint/conversions.hpp>
#include <bitprim/nodecint/list_creator.h>
#include <bitprim/nodecint/chain/utxo.h>

BITPRIM_LIST_DEFINE_CONVERTERS(chain, utxo_list_t, libbitcoin::chain::utxo, utxo_list)
BITPRIM_LIST_DEFINE_CONSTRUCT_FROM_CPP(chain, utxo_list_t, libbitcoin::chain::utxo, utxo_list)

extern "C" {
BITPRIM_LIST_DEFINE(chain, utxo_list_t, utxo_t, utxo_list, libbitcoin::chain::utxo, chain_utxo_const_cpp)
} // extern "C"