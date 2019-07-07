#ifndef KANGARU_DI_SAMPLE_BOOTSRAPPER_HPP
#define KANGARU_DI_SAMPLE_BOOTSRAPPER_HPP

#include <boost/noncopyable.hpp>

#include <kangaru/include/kangaru/kangaru.hpp>

namespace Dependency {

    class Bootstpapper : public boost::noncopyable {
        public:
            static Bootstpapper& getInstance();

        private:
            explicit Bootstpapper();

            virtual void initRepos();

            virtual void initServices();

        private:
            kgr::container container_;
    };
}

#endif //KANGARU_DI_SAMPLE_BOOTSRAPPER_HPP
