#pragma once
#include "AccountInfoEntity.h"
#include "ProfileInfoEntity.h"
#include <optional>
namespace NeteaseCloudMusic {
struct LoginStatusEntity {
    std::optional<AccountInfoEntity> account;
    std::optional<ProfileInfoEntity> profile;
};
}; // namespace NeteaseCloudMusic