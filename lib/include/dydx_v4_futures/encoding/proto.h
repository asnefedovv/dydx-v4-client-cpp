#pragma once

#include <cstdint>
#include <functional>
#include <string>

#include <cosmos/tx/v1beta1/tx.pb.h>
#include <dydxprotocol/clob/order.pb.h>
#include <dydxprotocol/clob/tx.pb.h>
#include <dydxprotocol/sending/transfer.pb.h>
#include <dydxprotocol/sending/tx.pb.h>

#include <common/encoding/base64.h>
#include <common/types.h>

#include <dydx_v4_futures/account_info.h>
#include <dydx_v4_futures/encoding/signing.h>
#include <dydx_v4_futures/enums.h>
#include <dydx_v4_futures/exchange_info.h>
#include <dydx_v4_futures/instrument_info.h>
#include <dydx_v4_futures/types.h>

namespace dydx_v4_client_lib {

inline static constexpr int QUOTE_QUANTUMS_ATOMIC_RESOLUTION = -6;

using GasLimitEstimator = std::function<uint64_t(std::string)>;

cosmos::tx::v1beta1::Tx SignMessageProto(
    const ExchangeInfo& exchange_info,
    const AccountInfo& account_info,
    const google::protobuf::Message& message,
    uint64_t gas_limit
);
std::string SignMessage(
    const ExchangeInfo& exchange_info,
    const AccountInfo& account_info,
    const google::protobuf::Message& message,
    std::optional<uint64_t> gas_limit,
    GasLimitEstimator estimator = {}
);

Quantums ToProtoQuantums(common::Quantity base_volume, const InstrumentInfo& info);
Subticks ToProtoSubticks(common::Price price, const InstrumentInfo& info);

enum class PlaceOrderTimeInForce {
    UNSPECIFIED,
    IOC,
    POST_ONLY,
    FILL_OR_KILL,
};

struct PlaceShortTermOrderParams {
    common::Symbol symbol;
    OrderSide side;
    PlaceOrderTimeInForce time_in_force = PlaceOrderTimeInForce::UNSPECIFIED;
    OrderCid order_cid;
    common::Price price;
    common::Quantity size;
    BlockNumber good_till_block;
    bool reduce_only = false;
    uint32_t client_metadata = 0;
};

std::string CreatePlaceOrderMessage(
    const ExchangeInfo& exchange_info, const AccountInfo& account_info, const PlaceShortTermOrderParams& params
);

enum class ConditionType {
    STOP_LOSS,
    TAKE_PROFIT,
};

struct ConditionalOrderParams {
    ConditionType condition_type;
    common::Price trigger_price;
};

struct PlaceLongTermOrderParams {
    common::Symbol symbol;
    OrderSide side;
    PlaceOrderTimeInForce time_in_force = PlaceOrderTimeInForce::UNSPECIFIED;
    OrderCid order_cid;
    common::Price price;
    common::Quantity size;
    uint32_t good_till_timestamp;
    bool reduce_only = false;
    std::optional<ConditionalOrderParams> conditional_order_params = std::nullopt;
    uint32_t client_metadata = 0;
};

std::string CreatePlaceOrderMessage(
    const ExchangeInfo& exchange_info, const AccountInfo& account_info, const PlaceLongTermOrderParams& params
);

struct CancelShortTermOrderParams {
    common::Symbol symbol;
    OrderCid order_cid;
    BlockNumber good_till_block;
};

std::string CreateCancelOrderMessage(
    const ExchangeInfo& exchange_info, const AccountInfo& account_info, const CancelShortTermOrderParams& params
);

struct CancelLongTermOrderParams {
    common::Symbol symbol;
    OrderCid order_cid;
    uint32_t good_till_timestamp;
    bool conditional = false;
};

std::string CreateCancelOrderMessage(
    const ExchangeInfo& exchange_info, const AccountInfo& account_info, const CancelLongTermOrderParams& params
);

std::string CreateTransferMessage(
    const ExchangeInfo& exchange_info,
    const AccountInfo& account_info,
    GasLimitEstimator estimator,
    Subaccount recipient_subaccount,
    common::Quantity amount,
    AssetId asset_id
);

std::string CreateDepositToSubaccountMessage(
    const ExchangeInfo& exchange_info,
    const AccountInfo& account_info,
    GasLimitEstimator estimator,
    common::Quantity amount,
    AssetId asset_id
);

std::string CreateWithdrawFromSubaccountMessage(
    const ExchangeInfo& exchange_info,
    const AccountInfo& account_info,
    GasLimitEstimator estimator,
    common::Quantity amount,
    AssetId asset_id
);

}  // namespace dydx_v4_client_lib
