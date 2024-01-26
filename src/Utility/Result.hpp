#pragma once
#include <QString>
#include <optional>
#include <type_traits>
#include <utility>
#include <variant>

enum class ErrorKind {
    NetworkError,
    JsonParseError,
    EncryptionError,
    ApiError,
};
class ErrorInfo {
public:
    ErrorKind kind;
    QString message;
    ErrorInfo(ErrorKind kind, const QString& message) : kind(kind), message(message) {}
    ErrorInfo(ErrorKind kind, QString&& message) : kind(kind), message(std::move(message)) {}
    ErrorInfo(const ErrorInfo& other) : kind(other.kind), message(other.message) {}
    ErrorInfo(ErrorInfo&& other) noexcept : kind(other.kind), message(std::move(other.message)) {}
    ErrorInfo& operator=(const ErrorInfo& other) {
        kind = other.kind;
        message = other.message;
        return *this;
    }
    ErrorInfo& operator=(ErrorInfo&& other) noexcept {
        kind = other.kind;
        message = std::move(other.message);
        return *this;
    }
    ~ErrorInfo() = default;
};
template <typename T>
class Result {
private:
    std::variant<T, ErrorInfo> data;

public:
    Result(T&& data) : data(std::move(data)) {}
    Result(const T& data) : data(data) {}
    Result(ErrorInfo&& data) : data(std::move(data)) {}
    Result(const ErrorInfo& data) : data(data) {}
    Result(const Result& other) : data(other.data) {}
    Result(Result&& other) noexcept : data(std::move(other.data)) {}
    Result& operator=(const Result& other) {
        data = other.data;
        return *this;
    }
    Result& operator=(Result&& other) noexcept {
        data = std::move(other.data);
        return *this;
    }
    ~Result() = default;
    bool isOk() const {
        return std::holds_alternative<T>(data);
    }
    bool isErr() const {
        return std::holds_alternative<ErrorInfo>(data);
    }
    const T& unwrap() const {
        return std::get<T>(data);
    }
    const ErrorInfo& unwrapErr() const {
        return std::get<ErrorInfo>(data);
    }
    T&& take() {
        return std::move(std::get<T>(data));
    }
    ErrorInfo&& takeErr() {
        return std::move(std::get<ErrorInfo>(data));
    }
    T unwrapOr(T&& def) const {
        if (isOk()) {
            return std::get<T>(data);
        } else {
            return std::forward<T>(def);
        }
    }
    template <typename F>
    T unwrapOrElse(F&& def) const {
        if (isOk()) {
            return std::get<T>(data);
        } else {
            return std::forward<F>(def)();
        }
    }
};

template <>
class Result<void> {
private:
    std::optional<ErrorInfo> data;

public:
    Result() : data(std::nullopt) {}
    Result(ErrorInfo&& data) : data(std::move(data)) {}
    Result(const ErrorInfo& data) : data(data) {}
    Result(const Result& other) : data(other.data) {}
    Result(Result&& other) noexcept : data(std::move(other.data)) {}
    Result& operator=(const Result& other) {
        data = other.data;
        return *this;
    }
    Result& operator=(Result&& other) noexcept {
        data = std::move(other.data);
        return *this;
    }
    ~Result() = default;
    bool isOk() const {
        return !data.has_value();
    }
    bool isErr() const {
        return data.has_value();
    }
    const ErrorInfo& unwrapErr() const {
        return data.value();
    }
    ErrorInfo&& takeErr() {
        return std::move(data.value());
    }
    template <typename F>
    void unwrapOrElse(F&& def) const {
        if (isOk()) {
            return;
        } else {
            return std::forward<F>(def)();
        }
    }
};
