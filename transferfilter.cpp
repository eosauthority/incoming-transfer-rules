#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <cctype>

constexpr account_name token_contract = N(eosio.token);

using namespace eosio;

class transfer_filter : public contract {
public:
   transfer_filter(account_name self) : contract(self) {}

   void ontransfer(const account_name from, const account_name to, const asset& quantity,
                   const std::string& memo) {

      if( to == _self ) {
         const char* error_str = "Memo has to be exactly 8 symbols long and contain only digits";
         //Rule 1: Memo has to be 8 characters long
         eosio_assert(memo.length() == 8, error_str);
         //Rule 2: Memo has to be all digits
         eosio_assert(std::all_of(memo.begin(), memo.end(), ::isdigit), error_str);
      }
   }
};

extern "C" {
void apply(uint64_t receiver, uint64_t code, uint64_t action) {

   auto self = receiver;
   if( action == N(onerror) ) {
      /* onerror is only valid if it is for the "eosio" code account and authorized by "eosio"'s "active permission */ \
      eosio_assert(code == N(eosio),
                   "onerror action's are only valid from the \"eosio\" system account"); \
   }

   if( code == token_contract && action == N(transfer) ) {
      transfer_filter contract(self);
      eosio::execute_action(&contract, &transfer_filter::ontransfer);
   }
}
}
