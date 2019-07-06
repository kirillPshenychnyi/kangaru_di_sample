#ifndef KANGARU_DI_SAMPLE_EXCEPTIONS_MESSAGES_HPP
#define KANGARU_DI_SAMPLE_EXCEPTIONS_MESSAGES_HPP

namespace Model {
namespace Exceptions {
    const char * const unknownAccount = "Account with id %1% does not exist";

    const char * const unknownDeposit = "Deposit with id %1% does not exist";

    const char * const negativeValue = "%1% value should be positive";

    const char * const ovedraftOverflow = "Cannot withdraw because of an overdraft limit";

    const char * const wrongFixedDepositOperation = "This operation is not allowed with fixed deposit";

    const char * const depositExpected = "%1% deposit expected for this operation";

    const char * const wrongStringParamValue = "All string args must be non empty";

    const char * const minBalanceViolation = "This withdraw will violate minimum balance";
}
}

#endif //KANGARU_DI_SAMPLE_EXCEPTIONS_MESSAGES_HPP
