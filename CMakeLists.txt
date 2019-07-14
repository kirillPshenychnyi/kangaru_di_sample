cmake_minimum_required(VERSION 3.9)
project(kangaru_di_sample)

set(CMAKE_CXX_STANDARD 14)

include_directories(${kangaru_di_sample_SOURCE_DIR})

add_executable(
        kangaru_di_sample
        main.cpp
        third_party/catch.hpp
        model/account.hpp model/employee.hpp
        model/entity.hpp model/deposit.hpp
        services/account_service.hpp model/typedefs.hpp
        services/deposit_service.hpp model/savings_deposit.hpp
        model/current_deposit.hpp model/fixed_deposit.hpp
        repositories/repository.hpp repositories/account_repository.hpp
        repositories/deposit_repository.hpp
        local_repositories_impl/repository_base_impl.hpp
        local_repositories_impl/account_repository_impl.hpp
        model/utils/generic_visitor.hpp model/utils/default_visitor.hpp
        model/utils/cast_visitor.hpp model/visitors/account_visitor.hpp
        services_impl/account_service_impl.cpp
        services_impl/account_service_impl.hpp
        services_impl/base_service_impl.hpp
        local_repositories_impl/deposit_repository_impl.hpp
        model/visitors/deposit_visitor.hpp
        services_impl/deposit_service_impl.hpp
        services_impl/deposit_service_impl.cpp model/exceptions_messages.hpp
        dependency/bootsrapper.hpp
        tests/account_service_test.cpp
        tests/deposit_service_test.cpp
        third_party/boost_di.hpp tests/test_container_bootstrapper.hpp
        tests/test_container_bootstrapper.cpp)