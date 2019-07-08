#ifndef KANGARU_DI_SAMPLE_EXCEPTION_MATCHER_HPP
#define KANGARU_DI_SAMPLE_EXCEPTION_MATCHER_HPP

#include <exception>
#include <catch2/catch.hpp>
#include <sstream>

namespace Test {
    class ExceptionMatcher : public Catch::MatcherBase<std::exception> {
        public:
            explicit ExceptionMatcher(std::string message) : message_(std::move(message)) {}

            bool match(const std::exception& exception) const final {
                return exception.what() == message_;
            }

            std::string describe() const final {
                std::ostringstream ss;
                ss << "has message " << message_;
                return ss.str();
            }

        private:
            const std::string message_;
    };

    // The builder function
    inline ExceptionMatcher HasMessage(std::string message) {
        return ExceptionMatcher(std::move(message));
    }
}

#endif //KANGARU_DI_SAMPLE_EXCEPTION_MATCHER_HPP
