#pragma once

#include <cstdint>
#include <string>

#include <dydx_v4_futures/types.h>

namespace dydx_v4_client_lib {

constexpr uint32_t SHORT_TERM_ORDER_EXPIRATION_FROM_CURRENT_BLOCK = 20;
constexpr AssetId ASSET_USDC = 0;
inline const std::string BECH32_PREFIX = "dydx";
inline const std::string BIP44_DERIVATION_PATH = "m/44'/118'/0'/0/0";

}  // namespace dydx_v4_client_lib
