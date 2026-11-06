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

#include "svg/TextParts.hpp"

namespace Draw2d::Svg {

    namespace
    {
        void Concaténer(const RawText &rawText, std::string &brut)
        {
            for (const auto &part : rawText.getTexts())
            {
                if (const auto *s = std::get_if<Core::String>(&part)) brut += *s;
                else Concaténer(std::get<std::unique_ptr<TSpan>>(part)->getText(), brut);
            }
        }
    }

    Core::String concatenateText(const RawText &rawText)
    {
        std::string brut;
        Concaténer(rawText, brut);

        std::string texte;
        texte.reserve(brut.size());
        bool bBlancEnAttente = false;
        for (char c : brut)
        {
            if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
            {
                // un blanc n'est écrit qu'avant le caractère suivant: ceux du
                // début et de la fin ne le sont jamais
                if (!texte.empty()) bBlancEnAttente = true;
                continue;
            }
            if (bBlancEnAttente) { texte += ' '; bBlancEnAttente = false; }
            texte += c;
        }
        return Core::String(texte);
    }
}
