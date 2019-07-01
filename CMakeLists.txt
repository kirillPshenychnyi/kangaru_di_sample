cmake_minimum_required(VERSION 3.14)
project(kangaru_di_sample)

set(CMAKE_CXX_STANDARD 14)

include_directories(${kangaru_di_sample_SOURCE_DIR})

add_executable(
        kangaru_di_sample
        main.cpp
        model/account.hpp model/employee.hpp
        model/entity.hpp model/deposit.hpp
        services/account_service.hpp model/typedefs.hpp
        services/deposit_service.hpp model/savings_deposit.hpp
        model/current_deposit.hpp model/fixed_deposit.hpp
        repositories/repository.hpp)