#include <utility>
#include <vector>
#include <string>
#include <eosiolib/eosio.hpp>
#include <eosiolib/time.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/types.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.h>
#include <boost/algorithm/string.hpp>


using namespace eosio;

class pingpong : public contract{
    using contract::contract;
    public:

    pingpong(account_name self):contract(self){};
    
    [[eosio::action]]
    void transfer(const account_name& from,
                            const account_name& to,
                            const asset& quantity,
                            const std::string& memo){
        if (from == _self || to != _self) {
        return;
        }
        print("from ",name{from}," to ",name{to}," memo ",memo,"\n");
        uint64_t amnt=quantity.amount;
        printf(" amount ", quantity.amount);
        print_f("checking if quantity is valid \n");
        if(quantity.is_valid()){
            eosio::transaction t{};
            eosio_assert(quantity.amount >= 3, "Must bet greater than min");
            print_f("quanity valid, creating transaction ");
            t.actions.emplace_back(
                permission_level{_self, N(active)},
                N(eosio.token),
                N(transfer),
                std::make_tuple(_self,
                from,
                asset(amnt,symbol_type(S(4,SYS))),
                std::string("Pong madafaka!")
                )
            );
            t.delay_sec=10;
            t.send(string_to_name(memo.c_str()),_self, true);

        }
    };
    private:


};

#define EOSIO_ABI_EX( TYPE, MEMBERS ) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
      auto self = receiver; \
      if( code == self || code == N(eosio.token)) { \
      	 if( action == N(transfer)){ \
      	 	eosio_assert( code == N(eosio.token), "Must transfer EOS"); \
      	 } \
         TYPE thiscontract( self ); \
         switch( action ) { \
            EOSIO_API( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
      } \
   } \
}



EOSIO_ABI_EX( pingpong, 
	(transfer)
)

