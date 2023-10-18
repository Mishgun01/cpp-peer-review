#include <stdexcept>
#include <array>
#include <string>

// Объявление перечисления для ошибок
enum class TimeValidityError {
  OK,
  YEAR_TOO_SMALL,
  YEAR_TOO_BIG,
  MONTH_TOO_SMALL,
  MONTH_TOO_BIG,
  DAY_TOO_SMALL,
  DAY_TOO_BIG,
  HOUR_TOO_SMALL,
  HOUR_TOO_BIG,
  MINUTES_TOO_SMALL,
  MINUTES_TOO_BIG,
  SECONDS_TOO_SMALL,
  SECONDS_TOO_BIG,
};

// Функция для проверки, является ли ошибка OK (без ошибок)
inline constexpr bool IsError(TimeValidityError error) {
  return error != TimeValidityError::OK;
}

// Функция для получения текстового сообщения об ошибке на основе ее типа
const std::string ErrorMessage(TimeValidityError error) {
  static constexpr std::string ERROR_MESSAGES[] = {
    "no error",
    "year is too small",
    "year is too big",
    "month is too small",
    "month is too big",
    "day is too small",
    "day is too big",
    "hour is too small",
    "hour is too big",
    "minute is too small",
    "minute is too big",
    "second is too small",
    "second is too big",
  };
  return ERROR_MESSAGES[static_cast<int>(error)];
}

// Функция для проверки валидности даты и времени с использованием исключений
TimeValidityError CheckDateTimeValidity(const DateTime& dt) {
    if (dt.year < 1) {
        throw std::domain_error("year is too small");
        return TimeValidityError::YEAR_TOO_SMALL;
    }
    if (dt.year > 9999) {
        throw std::domain_error("year is too big");
        return TimeValidityError::YEAR_TOO_BIG;
    }

    if (dt.month < 1 || dt.month > 12) {
        throw std::domain_error("month is out of range");
        return (dt.month < 1) ? TimeValidityError::MONTH_TOO_SMALL : TimeValidityError::MONTH_TOO_BIG;
    }

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const std::array<int, 12> month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (dt.day < 1 || dt.day > month_lengths[dt.month - 1]) {
        throw std::domain_error("day is out of range");
        return (dt.day < 1) ? TimeValidityError::DAY_TOO_SMALL : TimeValidityError::DAY_TOO_BIG;
    }

    if (dt.hour < 0 || dt.hour > 23) {
        throw std::domain_error("hour is out of range");
        return (dt.hour < 0) ? TimeValidityError::HOUR_TOO_SMALL : TimeValidityError::HOUR_TOO_BIG;
    }

    if (dt.minute < 0 || dt.minute > 59) {
        throw std::domain_error("minute is out of range");
        return (dt.minute < 0) ? TimeValidityError::MINUTES_TOO_SMALL : TimeValidityError::MINUTES_TOO_BIG;
    }

    if (dt.second < 0 || dt.second > 59) {
        throw std::domain_error("second is out of range");
        return (dt.second < 0) ? TimeValidityError::SECONDS_TOO_SMALL : TimeValidityError::SECONDS_TOO_BIG;
    }

    return TimeValidityError::OK;  // Все проверки пройдены, нет ошибок
}

// Функция для проверки валидности даты и времени без использования исключений
TimeValidityError CheckDateTimeValidityNoExcept(const DateTime& dt) {
    TimeValidityError error = TimeValidityError::OK;

    if (dt.year < 1) {
        error = TimeValidityError::YEAR_TOO_SMALL;
    } else if (dt.year > 9999) {
        error = TimeValidityError::YEAR_TOO_BIG;
    } else if (dt.month < 1 || dt.month > 12) {
        error = (dt.month < 1) ? TimeValidityError::MONTH_TOO_SMALL : TimeValidityError::MONTH_TOO_BIG;
    } else {
        const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
        const std::array<int, 12> month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        if (dt.day < 1 || dt.day > month_lengths[dt.month - 1]) {
            error = (dt.day < 1) ? TimeValidityError::DAY_TOO_SMALL : TimeValidityError::DAY_TOO_BIG;
        } else if (dt.hour < 0 || dt.hour > 23) {
            error = (dt.hour < 0) ? TimeValidityError::HOUR_TOO_SMALL : TimeValidityError::HOUR_TOO_BIG;
        } else if (dt.minute < 0 || dt.minute > 59) {
            error = (dt.minute < 0) ? TimeValidityError::MINUTES_TOO_SMALL : TimeValidityError::MINUTES_TOO_BIG;
        } else if (dt.second < 0 || dt.second > 59) {
            error = (dt.second < 0) ? TimeValidityError::SECONDS_TOO_SMALL : TimeValidityError::SECONDS_TOO_BIG;
        }
    }

    return error;  // Возврат типа ошибки
}

// Функция для проверки валидности даты и времени без исключений и текстовой ошибки
void CheckDateTimeValidity(const DateTime& dt) {
    TimeValidityError error = CheckDateTimeValidityNoExcept(dt);

    if (IsError(error)) {
        throw std::domain_error(ErrorMessage(error));  // Генерация исключения с текстовым сообщением
    }
}
