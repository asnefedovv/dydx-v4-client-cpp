#pragma once

#include <optional>
#include <string>
#include <vector>

#include <dydx_v4_futures/constants.h>
#include <dydx_v4_futures/encoding/proto.h>
#include <dydx_v4_futures/exchange_info.h>
#include <dydx_v4_futures/requests/indexer.h>
#include <dydx_v4_futures/requests/node_grpc_gateway.h>

namespace dydx_v4_client_lib {

struct CompositeClient {
    CompositeClient(ExchangeConfig config, BroadcastMode broadcast_mode = BroadcastMode::BROADCAST_MODE_SYNC)
        : broadcast_mode(broadcast_mode)
    {
        if (config.indexer_rest_config) {
            indexer_rest_client = IndexerRestClient(config);
        }
        if (config.node_grpc_gateway_rest_config) {
            node_grpc_gateway_rest_client = NodeGrpcGatewayRestClient(config);
        }
    }

    GasLimitEstimator GetGasLimitEstimator()
    {
        return [this](std::string message) {
            auto simulation_result = node_grpc_gateway_rest_client->Simulate(message);
            auto simulation_str = simulation_result.dump();
            auto gas_used_str = simulation_result["gas_info"]["gas_used"].get<std::string>();
            auto gas_used = stoull(gas_used_str);
            return static_cast<uint64_t>(gas_used * 1.65);
        };
    }

    nlohmann::json BroadcastTransaction(std::string transaction_bytes)
    {
        return node_grpc_gateway_rest_client->BroadcastTransaction(transaction_bytes, broadcast_mode);
    }

    nlohmann::json Simulate(std::string transaction_bytes)
    {
        return node_grpc_gateway_rest_client->Simulate(transaction_bytes);
    }

    nlohmann::json GetTransactionStatus(std::string tx_hash_hex)
    {
        return node_grpc_gateway_rest_client->GetTransactionStatus(tx_hash_hex);
    }

    nlohmann::json PlaceOrder(
        const ExchangeInfo& exchange_info, const AccountInfo& account_info, const PlaceShortTermOrderParams& params
    )
    {
        return BroadcastTransaction(CreatePlaceOrderMessage(exchange_info, account_info, params));
    }

    nlohmann::json PlaceOrder(
        const ExchangeInfo& exchange_info, AccountInfo& account_info, const PlaceLongTermOrderParams& params
    )
    {
        auto result = BroadcastTransaction(CreatePlaceOrderMessage(exchange_info, account_info, params));
        account_info.IncreaseSequence();
        return result;
    }

    nlohmann::json CancelOrder(
        const ExchangeInfo& exchange_info, const AccountInfo& account_info, const CancelShortTermOrderParams& params
    )
    {
        return BroadcastTransaction(CreateCancelOrderMessage(exchange_info, account_info, params));
    }

    nlohmann::json CancelOrder(
        const ExchangeInfo& exchange_info, AccountInfo& account_info, const CancelLongTermOrderParams& params
    )
    {
        auto result = BroadcastTransaction(CreateCancelOrderMessage(exchange_info, account_info, params));
        account_info.IncreaseSequence();
        return result;
    }

    nlohmann::json Transfer(
        const ExchangeInfo& exchange_info,
        AccountInfo& account_info,
        Subaccount recipient_subaccount,
        uint64_t amount,
        AssetId asset_id = ASSET_USDC
    )
    {
        auto result = BroadcastTransaction(CreateTransferMessage(
            exchange_info, account_info, GetGasLimitEstimator(), recipient_subaccount, amount, asset_id
        ));
        account_info.IncreaseSequence();
        return result;
    }

    nlohmann::json DepositToSubaccount(
        const ExchangeInfo& exchange_info, AccountInfo& account_info, Quantums quantums, AssetId asset_id = ASSET_USDC
    )
    {
        auto result = BroadcastTransaction(
            CreateDepositToSubaccountMessage(exchange_info, account_info, GetGasLimitEstimator(), quantums, asset_id)
        );
        account_info.IncreaseSequence();
        return result;
    }

    nlohmann::json WithdrawFromSubaccount(
        const ExchangeInfo& exchange_info, AccountInfo& account_info, Quantums quantums, AssetId asset_id = ASSET_USDC
    )
    {
        auto result = BroadcastTransaction(
            CreateWithdrawFromSubaccountMessage(exchange_info, account_info, GetGasLimitEstimator(), quantums, asset_id)
        );
        account_info.IncreaseSequence();
        return result;
    }

    BroadcastMode broadcast_mode;

    std::optional<IndexerRestClient> indexer_rest_client;
    std::optional<NodeGrpcGatewayRestClient> node_grpc_gateway_rest_client;
};

}  // namespace dydx_v4_client_lib
