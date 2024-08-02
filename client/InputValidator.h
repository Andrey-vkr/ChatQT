#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

#include <string>


class InputValidator
{
  public:
    InputValidator() = delete;

    static uint8_t getMinLength();
    static bool isTooShort(const std::string& input);
    static bool isContainsForbidenChars(const std::string& input);
    static bool isStartsByNumber(const std::string& input);
};

#endif

