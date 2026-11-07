/*
 * Copyright 2026 Pie Laboratories LLC
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

#include <iostream>
#include <iomanip>
#include <set>
#include <sstream>
#include "core/LoggerBase.hpp"

#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif

namespace Core {

    namespace
    {
        // le réglage le plus proche d'une catégorie: le sien, sinon celui de son parent,
        // et ainsi de suite jusqu'à la racine; rien si aucun sur toute la branche
        template <typename Carte>
        const typename Carte::mapped_type *PlusProche(const Carte &carte, std::string_view nom)
        {
            while (true)
            {
                if (const auto it = carte.find(nom); it != carte.end()) return &it->second;
                const size_t point = nom.rfind('.');
                if (point == std::string_view::npos) return nullptr;
                nom = nom.substr(0, point);
            }
        }
    }

    LoggerBase::LoggerBase(std::unordered_set<const char *, CStrHash, CStrEq> categories, LogLevel enumLogLevel)
        : m_enumLogLevel(enumLogLevel)
    {
        for (const char *catégorie : categories) m_états.insert_or_assign(std::string(catégorie), true);
    }

    void LoggerBase::Écrire(LogLevel niveau, const Catégorie &catégorie, std::string_view texte)
    {
        if (!__actif(niveau, catégorie.nom())) return;
        __écrire(niveau, catégorie.c_str(), texte);
    }

    bool LoggerBase::actif(LogLevel niveau, const Catégorie &catégorie) const
    {
        return __actif(niveau, catégorie.nom());
    }

    // depuis le code
    void LoggerBase::activerCatégorie(const Catégorie &catégorie, bool bActive) { __activer(catégorie.nom(), bActive); }
    void LoggerBase::oublierÉtat(const Catégorie &catégorie) { __oublierÉtat(catégorie.nom()); }
    bool LoggerBase::basculerCatégorie(const Catégorie &catégorie) { return __basculer(catégorie.nom()); }
    bool LoggerBase::catégorieActive(const Catégorie &catégorie) const { return catégorieActive(std::string_view(catégorie.nom())); }
    void LoggerBase::définirNiveau(const Catégorie &catégorie, LogLevel niveau) { __définirNiveau(catégorie.nom(), niveau); }
    void LoggerBase::oublierNiveau(const Catégorie &catégorie) { __oublierNiveau(catégorie.nom()); }
    std::optional<LogLevel> LoggerBase::niveauCatégorie(const Catégorie &catégorie) const { return niveauCatégorie(std::string_view(catégorie.nom())); }

    // depuis l'extérieur
    bool LoggerBase::activerCatégorie(std::string_view nom, bool bActive)
    {
        if (!__connu(nom)) return false;
        __activer(nom, bActive);
        return true;
    }

    bool LoggerBase::oublierÉtat(std::string_view nom)
    {
        if (!__connu(nom)) return false;
        __oublierÉtat(nom);
        return true;
    }

    std::optional<bool> LoggerBase::basculerCatégorie(std::string_view nom)
    {
        if (!__connu(nom)) return std::nullopt;
        return __basculer(nom);
    }

    bool LoggerBase::catégorieActive(std::string_view nom) const
    {
        std::shared_lock verrou(m_verrouCatégories);
        return __activeSousVerrou(nom);
    }

    bool LoggerBase::définirNiveau(std::string_view nom, LogLevel niveau)
    {
        if (!__connu(nom)) return false;
        __définirNiveau(nom, niveau);
        return true;
    }

    bool LoggerBase::oublierNiveau(std::string_view nom)
    {
        if (!__connu(nom)) return false;
        __oublierNiveau(nom);
        return true;
    }

    std::optional<LogLevel> LoggerBase::niveauCatégorie(std::string_view nom) const
    {
        std::shared_lock verrou(m_verrouCatégories);
        if (const auto it = m_niveaux.find(nom); it != m_niveaux.end()) return it->second;
        return std::nullopt;
    }

    void LoggerBase::définirNiveauParDéfaut(LogLevel niveau)
    {
        std::unique_lock verrou(m_verrouCatégories);
        m_enumLogLevel = niveau;
    }

    LogLevel LoggerBase::niveauParDéfaut() const
    {
        std::shared_lock verrou(m_verrouCatégories);
        return m_enumLogLevel;
    }

    std::vector<LoggerBase::ÉtatCatégorie> LoggerBase::catégories() const
    {
        // hors du verrou du journal: le registre a le sien
        const std::vector<std::string> déclarées = CatégoriesDéclarées();
        const std::set<std::string> ensembleDéclarées(déclarées.begin(), déclarées.end());

        std::shared_lock verrou(m_verrouCatégories);
        std::set<std::string> noms(ensembleDéclarées);
        for (const std::string &nom : m_vues) noms.insert(nom);
        for (const auto &[nom, _] : m_états) noms.insert(nom);
        for (const auto &[nom, _] : m_niveaux) noms.insert(nom);

        std::vector<ÉtatCatégorie> liste;
        liste.reserve(noms.size());
        for (const std::string &nom : noms)
            liste.push_back({ nom, __activeSousVerrou(nom), __niveauSousVerrou(nom),
                              m_états.contains(nom), m_niveaux.contains(nom), ensembleDéclarées.contains(nom) });
        return liste;
    }

    bool LoggerBase::__actif(LogLevel niveau, std::string_view nom) const
    {
        std::shared_lock verrou(m_verrouCatégories);
        return __actifSousVerrou(niveau, nom);
    }

    bool LoggerBase::__actifNonDéclaré(LogLevel niveau, const char *catégorie) const
    {
        const std::string_view nom(catégorie);

        // « all »: le niveau par défaut
        if (nom == "all")
        {
            std::shared_lock verrou(m_verrouCatégories);
            return niveau >= LogLevel::Warn || niveau >= m_enumLogLevel;
        }

        // le cas courant: un nom déjà vu
        {
            std::shared_lock verrou(m_verrouCatégories);
            if (m_vues.contains(nom)) return __actifSousVerrou(niveau, nom);
        }

        // la première fois: noté, et signalé s'il n'est pas déclaré -- le signal écrit
        // hors du verrou des catégories
        bool bSignaler = false;
        {
            std::unique_lock verrou(m_verrouCatégories);
            if (m_vues.emplace(nom).second) bSignaler = !CatégorieConnue(nom);
        }
        if (bSignaler)
            __écrire(LogLevel::Warn, "journal",
                     std::format("catégorie non déclarée « {} »: à déclarer comme Core::Catégorie", nom));

        std::shared_lock verrou(m_verrouCatégories);
        return __actifSousVerrou(niveau, nom);
    }

    bool LoggerBase::__connu(std::string_view nom) const
    {
        if (CatégorieConnue(nom)) return true;
        std::shared_lock verrou(m_verrouCatégories);
        return m_vues.contains(nom);
    }

    void LoggerBase::__activer(std::string_view nom, bool bActive)
    {
        std::unique_lock verrou(m_verrouCatégories);
        m_états.insert_or_assign(std::string(nom), bActive);
    }

    void LoggerBase::__oublierÉtat(std::string_view nom)
    {
        std::unique_lock verrou(m_verrouCatégories);
        if (const auto it = m_états.find(nom); it != m_états.end()) m_états.erase(it);
    }

    bool LoggerBase::__basculer(std::string_view nom)
    {
        std::unique_lock verrou(m_verrouCatégories);
        const bool bActive = !__activeSousVerrou(nom);
        m_états.insert_or_assign(std::string(nom), bActive);
        return bActive;
    }

    void LoggerBase::__définirNiveau(std::string_view nom, LogLevel niveau)
    {
        std::unique_lock verrou(m_verrouCatégories);
        m_niveaux.insert_or_assign(std::string(nom), niveau);
        m_états.insert_or_assign(std::string(nom), true);
    }

    void LoggerBase::__oublierNiveau(std::string_view nom)
    {
        std::unique_lock verrou(m_verrouCatégories);
        if (const auto it = m_niveaux.find(nom); it != m_niveaux.end()) m_niveaux.erase(it);
    }

    bool LoggerBase::__actifSousVerrou(LogLevel niveau, std::string_view nom) const
    {
        // les avertissements et les erreurs passent toujours
        if (niveau >= LogLevel::Warn) return true;
        if (!__activeSousVerrou(nom)) return false;
        return niveau >= __niveauSousVerrou(nom);
    }

    bool LoggerBase::__activeSousVerrou(std::string_view nom) const
    {
        const bool *bActive = PlusProche(m_états, nom);
        return bActive && *bActive;
    }

    LogLevel LoggerBase::__niveauSousVerrou(std::string_view nom) const
    {
        const LogLevel *niveau = PlusProche(m_niveaux, nom);
        return niveau ? *niveau : m_enumLogLevel;
    }

    void LoggerBase::__écrire(LogLevel niveau, const char *catégorie, std::string_view texte) const
    {
        const char *couleur = "\033[0m";
        switch (niveau)
        {
        case LogLevel::Error: couleur = "\033[31m"; break;   // rouge
        case LogLevel::Warn:  couleur = "\033[33m"; break;   // jaune
        case LogLevel::Info:  couleur = "\033[97m"; break;   // blanc
        case LogLevel::Debug: couleur = "\033[36m"; break;   // cyan
        case LogLevel::Trace: couleur = "\033[32m"; break;   // vert
        default: break;
        }

        // sous le verrou: gmtime n'est pas sûr entre fils, et la ligne part d'un bloc
        std::lock_guard verrou(_mutex);
        const std::time_t maintenant = std::time(nullptr);
        const std::tm *gmtm = std::gmtime(&maintenant);
        std::ostringstream ligne;
        ligne << couleur << std::format("[{}] [{}] ", catégorie, LogLevelToString(niveau))
              << '[' << std::put_time(gmtm, "%Y %m %d %H:%M:%S") << "]: " << texte << "\033[0m\n";
        std::cout << ligne.str() << std::flush;
    }
}
