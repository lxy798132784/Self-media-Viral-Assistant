#include "utils/platform_utils.h"
namespace mva::utils {
QString PlatformUtils::getPlatformName() {
#if defined(Q_OS_WIN)
    return "Windows";
#elif defined(Q_OS_MACOS)
    return "macOS";
#else
    return "Linux";
#endif
}
QString PlatformUtils::getArchitecture() {
#if defined(Q_PROCESSOR_X86_64)
    return "x86_64";
#elif defined(Q_PROCESSOR_ARM_64)
    return "ARM64";
#else
    return "Unknown";
#endif
}
} // namespace mva::utils

cat > /workspace/include/utils/platform_utils.h << 'EOF'
#ifndef PLATFORM_UTILS_H
#define PLATFORM_UTILS_H
#include <QString>
namespace mva::utils {
class PlatformUtils {
public:
    static QString getPlatformName();
    static QString getArchitecture();
};
} // namespace mva::utils
#endif
