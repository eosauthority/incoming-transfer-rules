#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <cctype>

using namespace eosio;

constexpr name token_contract = "eosio.token"_n;

class [[eosio::contract("transferfilter")]] transfer_filter : public contract {
public:
   using contract::contract;

   void ontransfer(const name& from, const name& to, const asset& quantity,
                   const std::string& memo) {

      if( to == _self ) {
         const char* error_str = "Memo has to be exactly 8 symbols long and contain only digits";
         //Rule 1: Memo has to be 8 characters long
         eosio::check(memo.length() == 8, error_str);
         //Rule 2: Memo has to be all digits
         eosio::check(std::all_of(memo.begin(), memo.end(), ::isdigit), error_str);
      }
   }
};

extern "C" {
void apply(uint64_t receiver, uint64_t code, uint64_t action) {
   if( code == token_contract.value && action == "transfer"_n.value ) {
      eosio::execute_action(eosio::name(receiver), token_contract, &transfer_filter::ontransfer);
   }
}
}
