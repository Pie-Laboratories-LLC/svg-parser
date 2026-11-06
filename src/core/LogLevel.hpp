#ifndef CORE_LOGLEVEL_DOT_HPP
#define CORE_LOGLEVEL_DOT_HPP
#include <optional>
#include <string_view>
#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
namespace Core {
    enum class LogLevel {
        NotALogLevel = -1,
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Fatal
    };
    // d'après son nom écrit, sans lancer: rien s'il n'en est pas un
    std::optional<LogLevel> ChercherLogLevel(std::string_view texte);
    // de même, mais un nom inconnu lance -- en nommant ceux qui existent
    LogLevel ParseLogLevel(const Core::String &cstrLogLevel);
    // son nom écrit, celui que Chercher et Parse lisent -- sans lancer, même pour une
    // valeur hors de l'énumération
    Core::String LogLevelToString(LogLevel enumLogLevel);
}
#endif  /* CORE_LOGLEVEL_DOT_HPP */
