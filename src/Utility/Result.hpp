#pragma once
#include <QDebug>
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
    JsonDeserializeError,
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
    const T& unwrap() const& {
        return std::get<T>(data);
    }
    T& unwrap() & {
        return std::get<T>(data);
    }
    T&& unwrap() && {
        return std::get<T>(std::move(data));
    }
    const ErrorInfo& unwrapErr() const& {
        return std::get<ErrorInfo>(data);
    }
    ErrorInfo& unwrapErr() & {
        return std::get<ErrorInfo>(data);
    }
    ErrorInfo&& unwrapErr() && {
        return std::get<ErrorInfo>(std::move(data));
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
    template <typename F>
    auto map(F&& f) const -> Result<decltype(f(std::declval<T>()))> {
        if (isOk()) {
            if constexpr (std::is_void_v<decltype(f(std::declval<T>()))>) {
                f(std::get<T>(data));
                return {};
            } else {
                return {f(std::get<T>(data))};
            }
        } else {
            return {std::get<ErrorInfo>(data)};
        }
    }
    template <typename F>
    auto andThen(F&& f) const -> decltype(f(std::declval<T>())) {
        if (isOk()) {
            return f(std::get<T>(data));
        } else {
            return {std::get<ErrorInfo>(data)};
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
    const ErrorInfo& unwrapErr() const& {
        return data.value();
    }
    ErrorInfo& unwrapErr() & {
        return data.value();
    }
    ErrorInfo&& unwrapErr() && {
        return std::move(data).value();
    }
    template <typename F>
    void unwrapOrElse(F&& def) const {
        if (isOk()) {
            return;
        } else {
            return std::forward<F>(def)();
        }
    }
    template <typename F>
    auto map(F&& f) const -> Result<decltype(f())> {
        if (isOk()) {
            return {f()};
        } else {
            return {data.value()};
        }
    }
    template <typename F>
    auto andThen(F&& f) const -> decltype(f()) {
        if (isOk()) {
            return f();
        } else {
            return {data.value()};
        }
    }
};

inline QDebug operator<<(QDebug debug, const ErrorKind& it) {
    QDebugStateSaver saver(debug);
    switch (it) {
    case ErrorKind::NetworkError:
        debug.noquote() << "NetworkError";
        break;
    case ErrorKind::JsonParseError:
        debug.noquote() << "JsonParseError";
        break;
    case ErrorKind::EncryptionError:
        debug.noquote() << "EncryptionError";
        break;
    case ErrorKind::ApiError:
        debug.noquote() << "ApiError";
        break;
    case ErrorKind::JsonDeserializeError:
        debug.noquote() << "JsonDeserializeError";
        break;
    default:
        debug.noquote() << "Error" << (int)it;
        break;
    }
    return debug;
}

inline QDebug operator<<(QDebug debug, const ErrorInfo& it) {
    QDebugStateSaver saver(debug);
    debug.noquote().nospace() << '<' << it.kind << QStringLiteral(": ") << it.message << '>';
    return debug;
}

template <
    typename T,
    std::enable_if_t<!std::is_void_v<decltype(*static_cast<QDebug*>(nullptr) << *static_cast<T*>(nullptr))>, int> = 0>
QDebug operator<<(QDebug debug, const Result<T>& it) {
    QDebugStateSaver saver(debug);
    if (it.isOk()) {
        debug.noquote().nospace() << QStringLiteral("Ok(") << it.unwrap() << ')';
    } else {
        debug.noquote().nospace() << QStringLiteral("Err(") << it.unwrapErr() << ')';
    }
    return debug;
}

inline QDebug operator<<(QDebug debug, const Result<void>& it) {
    QDebugStateSaver saver(debug);
    if (it.isOk()) {
        debug.noquote().nospace() << QStringLiteral("Ok()");
    } else {
        debug.noquote().nospace() << QStringLiteral("Err(") << it.unwrapErr() << ')';
    }
    return debug;
}
