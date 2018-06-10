/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#include <utility>
#include <vector>
#include <string>
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/crypto.h>
#include <eosiolib/currency.hpp>
#include <math.h>
#include <eosiolib/dispatcher.hpp>

using eosio::indexed_by;
using eosio::const_mem_fun;
using std::string;
using eosio::permission_level;
using eosio::action;
using eosio::asset;
using eosio::print;
using eosio::name;
#define EOS_SYMBOL S(4,EOW)

class noteip : public eosio::contract
{
public:
    //Constructor
    explicit noteip(action_name self) : contract(self) {}

    //add ips
    //@abi action
    void create(account_name owner_account, const string &ip_name, const string &ip_url, const string &stoke_token_name, const string &stoke_token_symbol, const asset &total_supply, uint64_t crowdsale_amount, const asset &price)
    {
        //Get authorization, if no authorization, Action call will be aborted, transaction will be rolled back
        require_auth(owner_account);
        eosio_assert(total_supply.amount > 0, "total_supply must be positive");
        eosio_assert(crowdsale_amount > 0, "crowdsale_amount must be positive");
        eosio_assert(crowdsale_amount < total_supply.amount, "crowdsale_amount must be smaller than total_supply");

        ip_index ips(_self, _self);

        ips.emplace(owner_account, [&](auto & ip)
        {
            ip.id = ips.available_primary_key();
            ip.owner_account = owner_account;
            ip.ip_name = ip_name;
            ip.ip_url = ip_url;
            ip.stoke_token_name = stoke_token_name;
            ip.stoke_token_symbol = stoke_token_symbol;
            ip.total_supply = total_supply;
            ip.crowdsale_amount = crowdsale_amount;
            ip.price = price;
            ip.video_adaptation_right = "1";
            ip.game_adaptation_right = "1";
            ip.paper_book_publishment = "1";
        });

    }

    //buy ip
    //@abi action
    void buyip(uint64_t ip_id, const account_name buyer_account, const asset &buy_quantity, const asset &pay_quantity)
    {
        require_auth(buyer_account);
        ip_index ips(_self, _self);
        account_index accounts(_self, _self);

        auto itr = ips.find(ip_id);
        eosio_assert(itr != ips.end(), "IP not found");

        action(
            permission_level{ itr->owner_account, N(active)},
            N(eosio.token), N(transfer),
            std::make_tuple(itr->owner_account, buyer_account, buy_quantity, std::string(""))
        ).send();

        action(
            permission_level{ buyer_account, N(active)},
            N(eosio.token), N(transfer),
            std::make_tuple(buyer_account, itr->owner_account, pay_quantity, std::string(""))
        ).send();

        auto ctr = accounts.find(ip_id);
        if (ctr != accounts.end())
        {
            if (ctr->owner == buyer_account)
            {
                accounts.modify(ctr, buyer_account, [&](auto & myaccount)
                {
                    myaccount.amount += buy_quantity;
                });
            }
            else
            {
                accounts.emplace(buyer_account, [&](auto & myaccount)
                {
                    myaccount.id = accounts.available_primary_key();
                    myaccount.owner = buyer_account;
                    myaccount.ip_id = ip_id;
                    myaccount.amount = buy_quantity;
                });
            }
        }
        else
        {
            accounts.emplace(buyer_account, [&](auto & myaccount)
            {
                myaccount.id = accounts.available_primary_key();
                myaccount.owner = buyer_account;
                myaccount.ip_id = ip_id;
                myaccount.amount = buy_quantity;
            });
        }
    }

    //Dividend
    //@abi action
    void dividend(uint64_t ip_id, const string &flag, const account_name buyer_account, const asset &pay_quantity)
    {
        require_auth(buyer_account);
        eosio_assert(pay_quantity.amount > 0, "pay_quantity must be positive");
        ip_index ips(_self, _self);
        account_index accounts(_self, _self);

        auto itr = ips.find(ip_id);
        if (flag == "video_adaptation_right")
        {
            eosio_assert(itr->video_adaptation_right == "1", "video_adaptation_right has been sold");
            ips.modify(itr, buyer_account, [&](auto & ip)
            {
                ip.video_adaptation_right = "0";
            });
        }
        if (flag == "game_adaptation_right")
        {
            eosio_assert(itr->game_adaptation_right == "1", "game_adaptation_right has been sold");
            ips.modify(itr, buyer_account, [&](auto & ip)
            {
                ip.game_adaptation_right = "0";
            });
        }
        if (flag == "paper_book_publishment")
        {
            eosio_assert(itr->paper_book_publishment == "1", "paper_book_publishment has been sold");
            ips.modify(itr, buyer_account, [&](auto & ip)
            {
                ip.paper_book_publishment = "0";
            });
        }

        auto ip_index = accounts.get_index<N(ip_id)>();
        auto ctr = ip_index.lower_bound(ip_id);
        asset remain_amount = pay_quantity;
        for (; ctr != ip_index.end() && ctr->ip_id == ip_id; ++ctr)
        {
            action(
                permission_level{ buyer_account, N(active)},
                N(eosio.token), N(transfer),
                std::make_tuple(buyer_account, ctr->owner, (pay_quantity.amount / itr->crowdsale_amount / 10000)*ctr->amount.amount * asset(1, EOS_SYMBOL), std::string(""))
            ).send();
            remain_amount -= (pay_quantity.amount / itr->crowdsale_amount / 10000) * ctr->amount.amount * asset(1, EOS_SYMBOL);

        }
        action(
            permission_level{ buyer_account, N(active)},
            N(eosio.token), N(transfer),
            std::make_tuple(buyer_account, itr->owner_account, remain_amount, std::string(""))
        ).send();
    }

private:
    //ip table
    //@abi table ip i64
    struct ip
    {
        uint64_t id;
        account_name owner_account;
        string ip_name;
        string ip_url;
        string stoke_token_name;
        string stoke_token_symbol;
        asset total_supply;
        uint64_t crowdsale_amount;
        asset price;
        string video_adaptation_right;
        string game_adaptation_right;
        string paper_book_publishment;

        uint64_t primary_key()const
        {
            return id;
        }

        EOSLIB_SERIALIZE( ip, (id)(owner_account)(ip_name)(ip_url)(stoke_token_name)(stoke_token_symbol)(total_supply)(crowdsale_amount)(price)(video_adaptation_right)(game_adaptation_right)(paper_book_publishment))
    };

    typedef eosio::multi_index< N(ip), ip> ip_index;

    //@abi table myaccount i64
    struct myaccount
    {
        uint64_t id;
        account_name owner;
        uint64_t ip_id;
        asset amount;

        uint64_t primary_key() const
        {
            return id;
        }
        uint64_t get_ip_id() const
        {
            return ip_id;
        }

        EOSLIB_SERIALIZE( myaccount, (id)(owner)(ip_id)(amount))
    };

    typedef eosio::multi_index< N(myaccount), myaccount,
            indexed_by<N(ip_id), const_mem_fun<myaccount, uint64_t, &myaccount::get_ip_id>>>
            account_index;
};

EOSIO_ABI(noteip, (create)(buyip)(dividend))