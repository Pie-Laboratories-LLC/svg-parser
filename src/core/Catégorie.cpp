#include "Catégorie.hpp"

#include <algorithm>
#include <mutex>
#include <set>

namespace Core
{
    namespace
    {
        // le registre: un seul pour tout le programme -- ici, dans le code compilé de
        // Core, pas dans un en-tête: sous Windows, chaque DLL aurait le sien
        struct Registre
        {
            std::mutex verrou;
            std::set<std::string, std::less<>> noms;
        };

        Registre &LeRegistre()
        {
            static Registre registre;
            return registre;
        }

        void Inscrire(const std::string &nom)
        {
            Registre &registre = LeRegistre();
            std::lock_guard verrou(registre.verrou);
            registre.noms.insert(nom);
        }

        // la distance d'édition: insertions, suppressions, substitutions -- octet par
        // octet, assez bon pour des noms de catégories
        size_t Distance(std::string_view a, std::string_view b)
        {
            std::vector<size_t> ligne(b.size() + 1);
            for (size_t j = 0; j <= b.size(); j++) ligne[j] = j;
            for (size_t i = 1; i <= a.size(); i++)
            {
                size_t diagonale = ligne[0];
                ligne[0] = i;
                for (size_t j = 1; j <= b.size(); j++)
                {
                    const size_t dessus = ligne[j];
                    ligne[j] = std::min({ dessus + 1, ligne[j - 1] + 1, diagonale + (a[i - 1] == b[j - 1] ? 0u : 1u) });
                    diagonale = dessus;
                }
            }
            return ligne[b.size()];
        }
    }

    Catégorie::Catégorie(const char *nom)
        : m_nom(nom)
    {
        Inscrire(m_nom);
    }

    Catégorie::Catégorie(const Catégorie &parent, const char *suffixe)
        : m_nom(parent.nom() + "." + suffixe)
    {
        Inscrire(m_nom);
    }

    std::vector<std::string> CatégoriesDéclarées()
    {
        Registre &registre = LeRegistre();
        std::lock_guard verrou(registre.verrou);
        return { registre.noms.begin(), registre.noms.end() };
    }

    bool CatégorieConnue(std::string_view nom)
    {
        if (nom.empty()) return false;
        Registre &registre = LeRegistre();
        std::lock_guard verrou(registre.verrou);
        if (registre.noms.contains(nom)) return true;
        // un parent: le premier nom à partir de « nom. » commence-t-il par lui?
        const std::string préfixe = std::string(nom) + ".";
        const auto it = registre.noms.lower_bound(préfixe);
        return it != registre.noms.end() && it->starts_with(préfixe);
    }

    std::optional<std::string> CatégorieProche(std::string_view nom)
    {
        // les candidats: chaque nom déclaré, et chacun de ses parents
        std::set<std::string> candidats;
        for (const std::string &déclaré : CatégoriesDéclarées())
        {
            std::string_view n = déclaré;
            while (true)
            {
                candidats.emplace(n);
                const size_t point = n.rfind('.');
                if (point == std::string_view::npos) break;
                n = n.substr(0, point);
            }
        }

        // assez proche: deux fautes, ou une par quatre caractères pour un long nom
        const size_t seuil = std::max<size_t>(2, nom.size() / 4);
        std::optional<std::string> meilleur;
        size_t distanceMeilleure = seuil + 1;
        for (const std::string &candidat : candidats)
        {
            const size_t d = Distance(nom, candidat);
            if (d < distanceMeilleure)
            {
                distanceMeilleure = d;
                meilleur = candidat;
            }
        }
        return meilleur;
    }
}
