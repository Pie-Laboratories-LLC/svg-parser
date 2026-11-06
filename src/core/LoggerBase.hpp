/*
 * Copyright 2026 Pie Laboratories
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CORE_LOGGERBASE_DOT_HPP
#define CORE_LOGGERBASE_DOT_HPP

#include <cstdarg>
#include <cstring>
#include <format>
#include <mutex>
#include <shared_mutex>
#include <unordered_set>
#include <unordered_map>

#ifndef CORE_LOGLEVEL_DOT_HPP
    #include "LogLevel.hpp"
#endif
#ifndef CORE_CATGORIE_DOT_HPP
    #include "core/Catégorie.hpp"
#endif
#ifndef CORE_STDSTRINGEXTENSIONS_DOT_HPP
    #include "core/StdStringExtensions.hpp"
#endif

namespace Core {

    // une clé de texte qu'on cherche sans la copier: par const char *, string_view ou string
    struct HachageTexte
    {
        using is_transparent = void;
        size_t operator()(std::string_view texte) const { return std::hash<std::string_view> {}(texte); }
    };

    class Journal;

    class LoggerBase {
    public:
        // écrire sous une catégorie déclarée -- d'ordinaire par les fonctions libres de
        // Core, Info(LOG, "…"), plutôt que par le singleton
        template <typename... Args>
        void Trace(const Catégorie &catégorie, const char *format, Args&&... args) { _Log(LogLevel::Trace, catégorie.c_str(), format, std::forward<Args>(args)...); }
        template <typename... Args>
        void Debug(const Catégorie &catégorie, const char *format, Args&&... args) { _Log(LogLevel::Debug, catégorie.c_str(), format, std::forward<Args>(args)...); }
        template <typename... Args>
        void Info(const Catégorie &catégorie, const char *format, Args&&... args) { _Log(LogLevel::Info, catégorie.c_str(), format, std::forward<Args>(args)...); }
        template <typename... Args>
        void Warning(const Catégorie &catégorie, const char *format, Args&&... args) { _Log(LogLevel::Warn, catégorie.c_str(), format, std::forward<Args>(args)...); }
        template <typename... Args>
        void Error(const Catégorie &catégorie, const char *format, Args&&... args) { _Log(LogLevel::Error, catégorie.c_str(), format, std::forward<Args>(args)...); }
        template <typename... Args>
        void Fatal(const Catégorie &catégorie, const char *format, Args&&... args) { _Log(LogLevel::Fatal, catégorie.c_str(), format, std::forward<Args>(args)...); }

        // un texte déjà fait, écrit tel quel -- jamais pris pour un format. Un littéral
        // va toujours au gabarit au-dessus, qui le formate
        void Trace(const Catégorie &catégorie, std::string_view texte) { Écrire(LogLevel::Trace, catégorie, texte); }
        void Debug(const Catégorie &catégorie, std::string_view texte) { Écrire(LogLevel::Debug, catégorie, texte); }
        void Info(const Catégorie &catégorie, std::string_view texte) { Écrire(LogLevel::Info, catégorie, texte); }
        void Warning(const Catégorie &catégorie, std::string_view texte) { Écrire(LogLevel::Warn, catégorie, texte); }
        void Error(const Catégorie &catégorie, std::string_view texte) { Écrire(LogLevel::Error, catégorie, texte); }
        void Fatal(const Catégorie &catégorie, std::string_view texte) { Écrire(LogLevel::Fatal, catégorie, texte); }

        void Écrire(LogLevel niveau, const Catégorie &catégorie, std::string_view texte);

        // l'ancienne manière, par un nom: le temps de la migration. Un nom jamais
        // déclaré est signalé une fois au journal
        template <typename... Args>
        [[deprecated("une Core::Catégorie déclarée")]] void Trace(const char *subject, const char *format, Args&&... args) { _LogNom(LogLevel::Trace, subject, format, std::forward<Args>(args)...); }
        template <typename... Args>
        [[deprecated("une Core::Catégorie déclarée")]] void Debug(const char *subject, const char *format, Args&&... args) { _LogNom(LogLevel::Debug, subject, format, std::forward<Args>(args)...); }
        template <typename... Args>
        [[deprecated("une Core::Catégorie déclarée")]] void Info(const char *subject, const char *format, Args&&... args) { _LogNom(LogLevel::Info, subject, format, std::forward<Args>(args)...); }
        template <typename... Args>
        [[deprecated("une Core::Catégorie déclarée")]] void Warning(const char *subject, const char *format, Args&&... args) { _LogNom(LogLevel::Warn, subject, format, std::forward<Args>(args)...); }
        template <typename... Args>
        [[deprecated("une Core::Catégorie déclarée")]] void Error(const char *subject, const char *format, Args&&... args) { _LogNom(LogLevel::Error, subject, format, std::forward<Args>(args)...); }
        template <typename... Args>
        [[deprecated("une Core::Catégorie déclarée")]] void Fatal(const char *subject, const char *format, Args&&... args) { _LogNom(LogLevel::Fatal, subject, format, std::forward<Args>(args)...); }

        // ce qu'on sait d'une catégorie, pour la lister: allumée ou non et son niveau,
        // tels qu'elle les voit; si chacun lui est propre ou vient d'un parent; si elle
        // est déclarée -- sinon, un parent réglé, ou un nom vu par l'ancienne manière
        struct ÉtatCatégorie
        {
            std::string nom;
            bool bActive = false;
            LogLevel niveau = LogLevel::Info;
            bool bÉtatPropre = false;
            bool bNiveauPropre = false;
            bool bDéclarée = false;
        };

        // Les catégories forment un arbre sur leurs points. Un réglage vaut pour toute la
        // branche, sauf là où un réglage plus proche le contredit -- l'allumage et le
        // niveau, chacun pour soi. Sans réglage sur sa branche: éteinte, au niveau par
        // défaut. À chaud, de n'importe quel fil

        // depuis le code, par la catégorie: sans vérification, elle a compilé
        void activerCatégorie(const Catégorie &catégorie, bool bActive);
        void oublierÉtat(const Catégorie &catégorie);
        // le contraire de ce qu'elle est maintenant, posé sur elle; son nouvel état
        bool basculerCatégorie(const Catégorie &catégorie);
        bool catégorieActive(const Catégorie &catégorie) const;
        // son niveau, pour elle et sa branche; lui en donner un l'allume
        void définirNiveau(const Catégorie &catégorie, LogLevel niveau);
        void oublierNiveau(const Catégorie &catégorie);
        // le niveau qui lui est propre, s'il y en a un
        std::optional<LogLevel> niveauCatégorie(const Catégorie &catégorie) const;

        // depuis l'extérieur, par un nom -- le tube, un fichier: seulement un nom connu,
        // déclaré ou parent d'un déclaré; sinon rien n'est fait, et faux
        [[nodiscard]] bool activerCatégorie(std::string_view nom, bool bActive);
        [[nodiscard]] bool oublierÉtat(std::string_view nom);
        [[nodiscard]] std::optional<bool> basculerCatégorie(std::string_view nom);
        bool catégorieActive(std::string_view nom) const;
        [[nodiscard]] bool définirNiveau(std::string_view nom, LogLevel niveau);
        [[nodiscard]] bool oublierNiveau(std::string_view nom);
        std::optional<LogLevel> niveauCatégorie(std::string_view nom) const;

        void définirNiveauParDéfaut(LogLevel niveau);
        LogLevel niveauParDéfaut() const;

        // toutes les catégories: déclarées, réglées, ou vues par l'ancienne manière -- triées
        std::vector<ÉtatCatégorie> catégories() const;

        // ce message serait-il écrit? Les avertissements et les erreurs, toujours; le
        // reste, si sa catégorie est allumée et qu'il atteint son niveau
        bool actif(LogLevel niveau, const Catégorie &catégorie) const;

    protected:
        LoggerBase(std::unordered_set<const char *, CStrHash, CStrEq> categories, LogLevel enumLogLevel);

        template <typename... Args>
        void _Log(LogLevel niveau, const char *nom, const char *format, Args&&... args)
        {
            if (!__actif(niveau, nom)) return;
            __écrire(niveau, nom, std::vformat(format, std::make_format_args(args...)));
        }

        template <typename... Args>
        void _LogNom(LogLevel niveau, const char *nom, const char *format, Args&&... args)
        {
            if (!__actifNonDéclaré(niveau, nom)) return;
            __écrire(niveau, nom, std::vformat(format, std::make_format_args(args...)));
        }

    private:
        friend class Journal;

        bool __actif(LogLevel niveau, std::string_view nom) const;
        // l'ancienne manière: un nom jamais déclaré est signalé, une fois
        bool __actifNonDéclaré(LogLevel niveau, const char *nom) const;
        // déclaré, parent d'un déclaré, ou vu par l'ancienne manière
        bool __connu(std::string_view nom) const;

        // les réglages, sans vérification
        void __activer(std::string_view nom, bool bActive);
        void __oublierÉtat(std::string_view nom);
        bool __basculer(std::string_view nom);
        void __définirNiveau(std::string_view nom, LogLevel niveau);
        void __oublierNiveau(std::string_view nom);

        // sous m_verrouCatégories, tenu par l'appelant: tels que la catégorie les voit --
        // le réglage le plus proche
        bool __actifSousVerrou(LogLevel niveau, std::string_view nom) const;
        bool __activeSousVerrou(std::string_view nom) const;
        LogLevel __niveauSousVerrou(std::string_view nom) const;

        // la ligne entière, construite et écrite sous le verrou de sortie
        void __écrire(LogLevel niveau, const char *catégorie, std::string_view texte) const;

        // lus à chaque message, de tous les fils; écrits à chaud, rarement
        mutable std::shared_mutex m_verrouCatégories;
        // les réglages propres: allumée ou éteinte; un niveau
        std::unordered_map<std::string, bool, HachageTexte, std::equal_to<>> m_états;
        std::unordered_map<std::string, LogLevel, HachageTexte, std::equal_to<>> m_niveaux;
        // les noms vus par l'ancienne manière -- chacun signalé une fois s'il n'est pas déclaré
        mutable std::unordered_set<std::string, HachageTexte, std::equal_to<>> m_vues;
        LogLevel m_enumLogLevel;

        // la sortie: une ligne à la fois
        mutable std::mutex _mutex {};
    }; // class LoggerBase

} // namespace Core

#endif  /* CORE_LOGGERBASE_DOT_HPP */
