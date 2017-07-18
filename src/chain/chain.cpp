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

#include <bitprim/nodecint/chain/chain.h>
#include <cstdio>
#include <memory>
#include <boost/thread/latch.hpp>
//#include <inttypes.h>   //TODO: Remove, it is for the printf (printing pointer addresses)
//#include <cinttypes>   //TODO: Remove, it is for the printf (printing pointer addresses)

#include <bitcoin/bitcoin/message/block.hpp>
#include <bitcoin/bitcoin/message/header.hpp>
#include <bitcoin/bitcoin/message/merkle_block.hpp>
#include <bitcoin/bitcoin/message/transaction.hpp>

#include <bitcoin/blockchain/interface/safe_chain.hpp>


namespace {

inline
libbitcoin::blockchain::safe_chain& safe_chain(chain_t chain) {
    return *static_cast<libbitcoin::blockchain::safe_chain*>(chain);
}

inline
libbitcoin::message::transaction::const_ptr tx_shared(transaction_t tx) {
    auto const& tx_ref = *static_cast<libbitcoin::message::transaction const*>(tx);
    auto* tx_new = new libbitcoin::message::transaction(tx_ref);
    return libbitcoin::message::transaction::const_ptr(tx_new);
}

inline
int char2int(char input) {
    if (input >= '0' && input <= '9')
        return input - '0';
    if (input >= 'A' && input <= 'F')
        return input - 'A' + 10;
    if (input >= 'a' && input <= 'f')
        return input - 'a' + 10;
    throw std::invalid_argument("Invalid input string");
}

inline
void hex2bin(const char* src, uint8_t* target) {
    while (*src && src[1]) {
        *(target++) = char2int(*src) * 16 + char2int(src[1]);
        src += 2;
    }
}

} /* end of anonymous namespace */


extern "C" {

void chain_fetch_last_height(chain_t chain, void* ctx, last_height_fetch_handler_t handler) {
    safe_chain(chain).fetch_last_height([chain, ctx, handler](std::error_code const& ec, size_t h) {
            handler(chain, ctx, ec.value(), h);
        });
}

int chain_get_last_height(chain_t chain, size_t* height) {
    boost::latch latch(2); //Note: workaround to fix an error on some versions of Boost.Threads

    int res;
    safe_chain(chain).fetch_last_height([&](std::error_code const& ec, size_t h) {
        *height = h;
        res = ec.value();
        latch.count_down();
    });

    latch.count_down_and_wait();
    return res;
}

void chain_fetch_block_height(chain_t chain, void* ctx, hash_t hash, block_height_fetch_handler_t handler) {

    libbitcoin::hash_digest hash_cpp;
    std::copy_n(hash, hash_cpp.size(), std::begin(hash_cpp));

    safe_chain(chain).fetch_block_height(hash_cpp, [chain, ctx, handler](std::error_code const& ec, size_t h) {
        handler(chain, ctx, ec.value(), h);
    });
}

int chain_get_block_height(chain_t chain, hash_t hash, size_t* height) {
    boost::latch latch(2); //Note: workaround to fix an error on some versions of Boost.Threads
    int res;

    libbitcoin::hash_digest hash_cpp;
    std::copy_n(hash, hash_cpp.size(), std::begin(hash_cpp));

    safe_chain(chain).fetch_block_height(hash_cpp, [&](std::error_code const& ec, size_t h) {
        *height = h;
        res = ec.value();
        latch.count_down();
    });

    latch.count_down_and_wait();
    return res;
}

void chain_fetch_block_header_by_height(chain_t chain, void* ctx, size_t height, block_header_fetch_handler_t handler) {
    safe_chain(chain).fetch_block_header(height, [chain, ctx, handler](std::error_code const& ec, libbitcoin::message::header::ptr header, size_t h) {
        auto new_header = new libbitcoin::message::header(*header.get());
//        auto new_header = std::make_unique(*header.get()).release();
        //Note: It is the responsability of the user to release/destruct the object
        handler(chain, ctx, ec.value(), new_header, h);
    });
}

int chain_get_block_header_by_height(chain_t chain, size_t height, header_t* out_header, size_t* out_height) {
    boost::latch latch(2); //Note: workaround to fix an error on some versions of Boost.Threads
    int res;

    safe_chain(chain).fetch_block_header(height, [&](std::error_code const& ec, libbitcoin::message::header::ptr header, size_t h) {
        *out_header = new libbitcoin::message::header(*header.get());
        //Note: It is the responsability of the user to release/destruct the object

        *out_height = h;
        res = ec.value();
        latch.count_down();
    });

    latch.count_down_and_wait();
    return res;
}

void chain_fetch_block_header_by_hash(chain_t chain, void* ctx, hash_t hash, block_header_fetch_handler_t handler) {

    libbitcoin::hash_digest hash_cpp;
    std::copy_n(hash, hash_cpp.size(), std::begin(hash_cpp));

    safe_chain(chain).fetch_block_header(hash_cpp, [chain, ctx, handler](std::error_code const& ec, libbitcoin::message::header::ptr header, size_t h) {
        auto new_header = new libbitcoin::message::header(*header.get());
//        auto new_header = std::make_unique(*header.get()).release();
        //Note: It is the responsability of the user to release/destruct the object
        handler(chain, ctx, ec.value(), new_header, h);
    });
}

int chain_get_block_header_by_hash(chain_t chain, hash_t hash, header_t* out_header, size_t* out_height) {
    boost::latch latch(2); //Note: workaround to fix an error on some versions of Boost.Threads
    int res;

    libbitcoin::hash_digest hash_cpp;
    std::copy_n(hash, hash_cpp.size(), std::begin(hash_cpp));

    safe_chain(chain).fetch_block_header(hash_cpp, [&](std::error_code const& ec, libbitcoin::message::header::ptr header, size_t h) {
        *out_header = new libbitcoin::message::header(*header.get());
        //Note: It is the responsability of the user to release/destruct the object

        *out_height = h;
        res = ec.value();
        latch.count_down();
    });

    latch.count_down_and_wait();
    return res;
}

void chain_fetch_block_by_height(chain_t chain, void* ctx, size_t height, block_fetch_handler_t handler) {
    safe_chain(chain).fetch_block(height, [chain, ctx, handler](std::error_code const& ec, libbitcoin::message::block::ptr block, size_t h) {

        auto new_block = new libbitcoin::message::block(*block.get());
        //Note: It is the responsability of the user to release/destruct the object
        handler(chain, ctx, ec.value(), new_block, h);
    });
}

int chain_get_block_by_height(chain_t chain, size_t height, block_t* out_block, size_t* out_height) {
    boost::latch latch(2); //Note: workaround to fix an error on some versions of Boost.Threads
    int res;

    safe_chain(chain).fetch_block(height, [&](std::error_code const& ec, libbitcoin::message::block::ptr block, size_t h) {
        *out_block = new libbitcoin::message::block(*block.get());
        //Note: It is the responsability of the user to release/destruct the object

        *out_height = h;
        res = ec.value();
        latch.count_down();
    });

    latch.count_down_and_wait();
    return res;
}

void chain_fetch_block_by_hash(chain_t chain, void* ctx, hash_t hash, block_fetch_handler_t handler) {

    libbitcoin::hash_digest hash_cpp;
    std::copy_n(hash, hash_cpp.size(), std::begin(hash_cpp));

    safe_chain(chain).fetch_block(hash_cpp, [chain, ctx, handler](std::error_code const& ec, libbitcoin::message::block::ptr block, size_t h) {
        //Note: It is the responsability of the user to release/destruct the object
        auto new_block = new libbitcoin::message::block(*block.get());
        handler(chain, ctx, ec.value(), new_block, h);
    });
}

int chain_get_block_by_hash(chain_t chain, hash_t hash, block_t* out_block, size_t* out_height) {
    boost::latch latch(2); //Note: workaround to fix an error on some versions of Boost.Threads
    int res;

    libbitcoin::hash_digest hash_cpp;
    std::copy_n(hash, hash_cpp.size(), std::begin(hash_cpp));

    safe_chain(chain).fetch_block(hash_cpp, [&](std::error_code const& ec, libbitcoin::message::block::ptr block, size_t h) {
        //Note: It is the responsability of the user to release/destruct the object
        *out_block = new libbitcoin::message::block(*block.get());
        *out_height = h;
        res = ec.value();
        latch.count_down();
    });

    latch.count_down_and_wait();
    return res;
}

void chain_fetch_merkle_block_by_height(chain_t chain, void* ctx, size_t height, merkle_block_fetch_handler_t handler){
    safe_chain(chain).fetch_merkle_block(height, [chain, ctx, handler](std::error_code const& ec, libbitcoin::message::merkle_block::ptr block, size_t h) {

        auto new_block = new libbitcoin::message::merkle_block(*block.get());
        //Note: It is the responsibility of the user to release/destruct the object
        handler(chain, ctx, ec.value(), new_block, h);
    });
}

void chain_fetch_merkle_block_by_hash(chain_t chain, void* ctx, hash_t hash, merkle_block_fetch_handler_t handler) {

    libbitcoin::hash_digest hash_cpp;
    std::copy_n(hash, hash_cpp.size(), std::begin(hash_cpp));

    safe_chain(chain).fetch_merkle_block(hash_cpp, [chain, ctx, handler](std::error_code const& ec, libbitcoin::message::merkle_block::ptr block, size_t h) {
        auto new_block = new libbitcoin::message::merkle_block(*block.get());
        handler(chain, ctx, ec.value(), new_block, h);
    });
}

void chain_fetch_transaction(chain_t chain, void* ctx, hash_t hash, int require_confirmed, transaction_fetch_handler_t handler) {
    //precondition:  [hash, 32] is a valid range

    libbitcoin::hash_digest hash_cpp;
    std::copy_n(hash, hash_cpp.size(), std::begin(hash_cpp));

    safe_chain(chain).fetch_transaction(hash_cpp, require_confirmed, [chain, ctx, handler](std::error_code const& ec, libbitcoin::message::transaction::ptr transaction, size_t h, size_t i) {
        auto new_transaction = new libbitcoin::message::transaction(*transaction.get());
        handler(chain, ctx, ec.value(), new_transaction, h, i);
    });
}

int chain_get_transaction(chain_t chain, hash_t hash, int require_confirmed, transaction_t* out_transaction, size_t* out_height, size_t* out_index) {
    boost::latch latch(2); //Note: workaround to fix an error on some versions of Boost.Threads
    int res;

    libbitcoin::hash_digest hash_cpp;
    std::copy_n(hash, hash_cpp.size(), std::begin(hash_cpp));

    safe_chain(chain).fetch_transaction(hash_cpp, require_confirmed, [&](std::error_code const& ec, libbitcoin::message::transaction::ptr transaction, size_t h, size_t i) {
        *out_transaction = new libbitcoin::message::transaction(*transaction.get());
        *out_height = h;
        *out_index = i;
        res = ec.value();
        latch.count_down();
    });

    latch.count_down_and_wait();
    return res;

}

void chain_fetch_output(chain_t chain, void* ctx, hash_t hash, uint32_t index, int require_confirmed, output_fetch_handler_t handler) {

    libbitcoin::hash_digest hash_cpp;
    std::copy_n(hash, hash_cpp.size(), std::begin(hash_cpp));

    libbitcoin::chain::output_point point(hash_cpp, index);

    safe_chain(chain).fetch_output(point, require_confirmed, [chain, ctx, handler](std::error_code const& ec, libbitcoin::chain::output const& output) {
        //It is the user's responsibility to release this memory
        auto new_output = new libbitcoin::chain::output(output);
        handler(chain, ctx, ec.value(), new_output);
    });
}

int chain_get_output(chain_t chain, hash_t hash, uint32_t index, int require_confirmed, output_t* out_output) {
    boost::latch latch(2); //Note: workaround to fix an error on some versions of Boost.Threads
    int res;

    libbitcoin::hash_digest hash_cpp;
    std::copy_n(hash, hash_cpp.size(), std::begin(hash_cpp));

    libbitcoin::chain::output_point point(hash_cpp, index);

    safe_chain(chain).fetch_output(point, require_confirmed, [&](std::error_code const& ec, libbitcoin::chain::output const& output) {
        *out_output = new libbitcoin::chain::output(output);

        res = ec.value();
        latch.count_down();
    });

    latch.count_down_and_wait();
    return res;
}

void chain_fetch_compact_block_by_height(chain_t chain, void* ctx, size_t height, compact_block_fetch_handler_t handler){
    safe_chain(chain).fetch_compact_block(height, [chain, ctx, handler](std::error_code const& ec, libbitcoin::message::compact_block::ptr block, size_t h) {
        auto new_block = new libbitcoin::message::compact_block(*block.get());
        //Note: It is the responsibility of the user to release/destruct the object
        handler(chain, ctx, ec.value(), new_block, h);
    });
}

void chain_fetch_compact_block_by_hash(chain_t chain, void* ctx, hash_t hash, compact_block_fetch_handler_t handler){
    libbitcoin::hash_digest hash_cpp;
    std::copy_n(hash, hash_cpp.size(), std::begin(hash_cpp));

    safe_chain(chain).fetch_compact_block(hash_cpp, [chain, ctx, handler](std::error_code const& ec, libbitcoin::message::compact_block::ptr block, size_t h) {
        auto new_block = new libbitcoin::message::compact_block(*block.get());
        //Note: It is the responsibility of the user to release/destruct the object
        handler(chain, ctx, ec.value(), new_block, h);
    });
}

void chain_fetch_transaction_position(chain_t chain, void* ctx, hash_t hash, int require_confirmed, transaction_index_fetch_handler_t handler){
    libbitcoin::hash_digest hash_cpp;
    std::copy_n(hash, hash_cpp.size(), std::begin(hash_cpp));

    safe_chain(chain).fetch_transaction_position(hash_cpp, require_confirmed, [chain, ctx, handler](std::error_code const& ec, size_t position, size_t height){
        handler(chain, ctx, ec.value(), position, height);
    });
}

//It is the user's responsibility to release the input point returned in the callback
void chain_fetch_spend(chain_t chain, void* ctx, output_point_t outpoint, spend_fetch_handler_t handler){
    libbitcoin::chain::output_point* outpoint_cpp = static_cast<libbitcoin::chain::output_point*>(outpoint);

    safe_chain(chain).fetch_spend(*outpoint_cpp, [chain, ctx, handler](std::error_code const& ec, libbitcoin::chain::input_point input){
        auto new_input = new libbitcoin::chain::input_point(input);
        handler(chain, ctx, ec.value(), new_input);
    });
}

//It is the user's responsibility to release the history returned in the callback
void chain_fetch_history(chain_t chain, void* ctx, payment_address_t address, size_t limit, size_t from_height, history_fetch_handler_t handler){
    libbitcoin::wallet::payment_address const& address_cpp = *static_cast<const libbitcoin::wallet::payment_address*>(address);

    safe_chain(chain).fetch_history(address_cpp, limit, from_height, [chain, ctx, handler](std::error_code const& ec, libbitcoin::chain::history_compact::list history){
        auto new_history = new libbitcoin::chain::history_compact::list(history);
        handler(chain, ctx, ec.value(), new_history);
    });
}

//It is the user's responsibility to release the history returned in the callback
int chain_get_history(chain_t chain, payment_address_t address, size_t limit, size_t from_height, history_compact_list_t* out_history) {
    boost::latch latch(2); //Note: workaround to fix an error on some versions of Boost.Threads
    int res;

    libbitcoin::wallet::payment_address const& address_cpp = *static_cast<const libbitcoin::wallet::payment_address*>(address);

    safe_chain(chain).fetch_history(address_cpp, limit, from_height, [&](std::error_code const& ec, libbitcoin::chain::history_compact::list history){
        *out_history = new libbitcoin::chain::history_compact::list(history);

        res = ec.value();
        latch.count_down();
    });

    latch.count_down_and_wait();
    return res;
}


//It is the user's responsibility to release the transaction returned
transaction_t chain_hex_to_tx(char const* tx_hex) {

    static const auto version = libbitcoin::message::version::level::canonical;

//    const auto tx = std::make_shared<libbitcoin::message::transaction>();
    auto* tx = new libbitcoin::message::transaction;

    std::string tx_hex_cpp(tx_hex);
    std::vector<uint8_t> data(tx_hex_cpp.size() / 2); // (tx_hex_cpp.begin(), tx_hex_cpp.end());
    //data.reserve(tx_hex_cpp.size() / 2);

    hex2bin(tx_hex_cpp.c_str(), data.data());

    if (!tx->from_data(version, data)) {
        return nullptr;
    }

    // Simulate organization into our chain.
    tx->validation.simulate = true;

    return tx;
}

void chain_validate_tx(chain_t chain, void* ctx, transaction_t tx, validate_tx_handler_t handler) {

    safe_chain(chain).organize(tx_shared(tx), [chain, ctx, handler](std::error_code const& ec) {

        bool is_error = (bool)ec;
        if (handler != nullptr) {
            char* msg_str_c = nullptr;
            if (ec) {
                auto* msg_str = new std::string(ec.message());
                msg_str_c = (char*)msg_str->c_str();
            }

            handler(chain, ctx, ec.value(), msg_str_c);
        }
    });
}

} /* extern "C" */