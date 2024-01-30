#pragma once
#include "AccoutInfoEntity.h"
#include "ProfileInfoEntity.h"
#include <optional>
namespace NeteaseCloudMusic {
struct LoginStatusEntity {
    std::optional<AccoutInfoEntity> account;
    std::optional<ProfileInfoEntity> profile;
};
}; // namespace NeteaseCloudMusic