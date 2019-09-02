# kangaru_di_sample

[![Build Status](https://travis-ci.com/kirillPshenychnyi/kangaru_di_sample.svg?branch=master)](https://travis-ci.com/kirillPshenychnyi/kangaru_di_sample)

This repository represents test example of using different C++ DI frameworks. 

### Model 
#### Deposits
Domain model represents trivial bank system with 3 types of deposits, accounts, services and repositories. 
There are 3 deposit types:
- `SavingsDeposit` - deposit with low rate of interest and obligatory permanent on a deposit  
- `FixedDeposit` - deposit which rate of interest grows with its maturity. For such deposit withdraw and refill operations are forbidden
- `CurrentDeposit` - zero rate of interest deposit with an overdraft limit
#### Accounts
There two account types:
- `Account` - represents client of a bank
- `Employee` - represents employee of a bank system
#### Services 
User doesn't and mustn't create, modify, update directly. Instead interaction is done via specific services:
- `AccountService` - API for creating, modifying, deleting accounts 
- `DepositService` - API for creating, modifying, deleting deposits
#### Repositories
Repositories represent CRUD operations and query operations for obtaining specific enteties. For example, `DepositRepository` has method `findSavingsDeposit` for `SavingsDeposit`retrievement.  

### Dependency Injection 
The main purpose of this project is to demonstrate dependency injection mechanism. `Bootstrapper` interface is an API which encapsulates concrete DI implementation. User should use this interface object for services access. `TestContainerBootstrapper` is the implementation which should be used for test purposes. 
There two branches in this repo:
- [master](https://github.com/kirillPshenychnyi/kangaru_di_sample/tree/master) - contains DI implementation using [kangaru framework](https://github.com/gracicot/kangaru)
- [di_dependency](https://github.com/kirillPshenychnyi/kangaru_di_sample/tree/di_dependency) - contains DI implementation using [[BOOST].DI](https://boost-experimental.github.io/di/)
 
