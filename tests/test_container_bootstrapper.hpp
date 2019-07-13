#ifndef KANGARU_DI_SAMPLE_TEST_CONTAINER_BOOTSTRAPPER_HPP
#define KANGARU_DI_SAMPLE_TEST_CONTAINER_BOOTSTRAPPER_HPP

#include "dependency/bootsrapper.hpp"

namespace Test {

    class TestContainerBootstrapper : public Dependency::Bootstrapper {
        public:
            TestContainerBootstrapper();
        private:
            void initRepos();

            void initServices();
    };
}

#endif //KANGARU_DI_SAMPLE_TEST_CONTAINER_BOOTSTRAPPER_HPP
