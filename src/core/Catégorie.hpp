#ifndef CORE_CATÉGORIE_DOT_HPP
#define CORE_CATÉGORIE_DOT_HPP

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace Core
{
    // une catégorie de journal déclarée: un nom en arbre sur ses points --
    // « draw2d.svg.glyphs » a pour parents « draw2d.svg » puis « draw2d ». La
    // déclarer l'inscrit au registre; une fille se construit sur sa mère, le
    // préfixe n'est jamais retapé. Déclarée « inline const » dans l'en-tête de sa
    // bibliothèque: chaque DLL en a sa copie, toutes inscrivent le même nom -- on ne
    // compare jamais les adresses, seulement les noms
    class Catégorie
    {
    public:
        explicit Catégorie(const char *nom);
        Catégorie(const Catégorie &parent, const char *suffixe);

        Catégorie(const Catégorie &) = delete;
        Catégorie &operator=(const Catégorie &) = delete;

        const std::string &nom() const { return m_nom; }
        const char *c_str() const { return m_nom.c_str(); }

    private:
        std::string m_nom;
    };

    // les catégories déclarées, triées
    std::vector<std::string> CatégoriesDéclarées();
    // déclarée, ou parent d'une déclarée: « draw2d » si « draw2d.svg » l'est --
    // « draw2dx » non: un préfixe de texte n'est pas un parent
    bool CatégorieConnue(std::string_view nom);
    // le nom connu le plus proche d'un nom mal tapé -- déclaré ou parent d'un déclaré;
    // rien si aucun n'est assez proche
    std::optional<std::string> CatégorieProche(std::string_view nom);
}

#endif // CORE_CATÉGORIE_DOT_HPP
