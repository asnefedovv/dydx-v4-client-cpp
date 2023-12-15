#pragma once

#include <fmt/core.h>

#include <common/streams/base.h>

#include <dydx_v4_futures/enums.h>

namespace dydx_v4_client_lib {

class IndexerWsClient : public common::WsClient {
public:
    using common::WsClient::WsClient;

    explicit IndexerWsClient(const ExchangeConfig& config)
        : IndexerWsClient(*config.indexer_ws_config)
    {}

    void Subscribe(std::string channel, std::optional<std::string> id = std::nullopt, bool batched = false)
    {
        SendMessage(fmt::format(
            R"({{"type":"subscribe","channel":"{}"{}{}}})",
            channel,
            id ? fmt::format(R"(,"id":"{}")", *id) : "",
            batched ? ",\"batched\":true" : ""
        ));
    }

    void Unsubscribe(std::string channel, std::optional<std::string> id = std::nullopt)
    {
        SendMessage(fmt::format(
            R"({{"type":"unsubscribe","channel":"{}"{}}})", channel, id ? fmt::format(R"(,"id":"{}")", *id) : ""
        ));
    }

    void SubscribeToMarkets()
    {
        Subscribe("v4_markets");
    }

    void UnsubscribeFromMarkets()
    {
        Unsubscribe("v4_markets");
    }

    void SubscribeToTrades(std::string market)
    {
        Subscribe("v4_trades", /*id=*/market, /*batched=*/true);
    }

    void UnsubscribeFromTrades(std::string market)
    {
        Unsubscribe("v4_trades", /*id=*/market);
    }

    void SubscribeToOrderbook(std::string market)
    {
        Subscribe("v4_orderbook", /*id=*/market, /*batched=*/true);
    }

    void UnsubscribeFromOrderbook(std::string market)
    {
        Unsubscribe("v4_orderbook", /*id=*/market);
    }

    void SubscribeToCandles(std::string market, CandlesResolution resolution)
    {
        auto id = market + "/" + to_string(resolution);
        Subscribe("v4_candles", /*id=*/id, /*batched=*/true);
    }

    void UnsubscribeFromCandles(std::string market, CandlesResolution resolution)
    {
        auto id = market + "/" + to_string(resolution);
        Unsubscribe("v4_candles", /*id=*/id);
    }

    void SubscribeToSubaccount(const Subaccount& subaccount)
    {
        auto id = subaccount.account_address + "/" + std::to_string(subaccount.subaccount_number);
        Subscribe("v4_subaccounts", /*id=*/id);
    }

    void UnsubscribeFromSubaccount(const Subaccount& subaccount)
    {
        auto id = subaccount.account_address + "/" + std::to_string(subaccount.subaccount_number);
        Unsubscribe("v4_subaccounts", /*id=*/id);
    }
};

}  // namespace dydx_v4_client_lib
