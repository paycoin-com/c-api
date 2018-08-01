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

#include <bitprim/nodecint/keoken/get_assets_by_address_data.h>

#include <bitprim/keoken/state_dto.hpp>
#include <bitprim/nodecint/helpers.hpp>
#include <bitprim/nodecint/type_conversions.h>

BITPRIM_CONV_DEFINE(keoken, get_assets_by_address_data_t, bitprim::keoken::get_assets_by_address_data, get_assets_by_address_data)

// ---------------------------------------------------------------------------
extern "C" {

// get_assets_by_address_data_t keoken_get_assets_by_address_data_construct_default(void) {
//     return new bitprim::keoken::get_assets_by_address_data();
// }

get_assets_by_address_data_t keoken_get_assets_by_address_data_construct(keoken_asset_id_t asset_id, char const* asset_name, payment_address_t asset_creator, keoken_amount_t amount) {
    libbitcoin::wallet::payment_address const& asset_creator_cpp = *static_cast<const libbitcoin::wallet::payment_address*>(asset_creator);
    return new bitprim::keoken::get_assets_by_address_data(asset_id, std::string(asset_name), asset_creator_cpp, amount);
}

void keoken_get_assets_by_address_data_destruct(get_assets_by_address_data_t obj) {
    delete &keoken_get_assets_by_address_data_cpp(obj);
}

keoken_asset_id_t keoken_get_assets_by_address_data_asset_id(get_assets_by_address_data_t obj) {
    return keoken_get_assets_by_address_data_const_cpp(obj).asset_id;
}

char const* keoken_get_assets_by_address_data_asset_name(get_assets_by_address_data_t obj) {
    auto const& asset_name = keoken_get_assets_by_address_data_const_cpp(obj).asset_name;
    return bitprim::create_c_str(asset_name);
}

payment_address_t keoken_get_assets_by_address_data_asset_creator(get_assets_by_address_data_t obj) {
    auto asset_creator = keoken_get_assets_by_address_data_const_cpp(obj).asset_creator;
    return new libbitcoin::wallet::payment_address(asset_creator);
}

keoken_amount_t keoken_get_assets_by_address_data_amount(get_assets_by_address_data_t obj) {
    return keoken_get_assets_by_address_data_const_cpp(obj).amount;
}

} // extern "C"