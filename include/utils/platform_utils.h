#ifndef PLATFORM_UTILS_H
#define PLATFORM_UTILS_H
#include <QString>
namespace mva::utils { class PlatformUtils { public: static QString getPlatformName(); static QString getArchitecture(); }; }
#endif
