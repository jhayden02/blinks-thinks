/***********************************************************************************************
* 
*   level.hpp - The library for creating, loading, and unloading levels. 
*
*   Copyright (c) 2026 Josh Hayden (@jhayden02)
*
*   Blink's Thinks is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License v3.0 as published
*   by the Free Software Foundation.
*  
*   Blink's Thinks is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*  
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
***********************************************************************************************/

#pragma once

// Source.
#include "entity.hpp"
#include "animation.hpp"
#include "background.hpp"
#include "overlay.hpp"
#include "text.hpp"
#include "label.hpp"
#include "button.hpp"

using engine::text;

// Standard library.
#include <string>
#include <vector>
#include <type_traits>

using std::string;
using std::vector;
using std::is_base_of;

namespace engine
{

class game;

class level
{
    public:
        level();

        virtual ~level();

        virtual void update();

        virtual void draw();

        vector<button*> get_buttons() { return m_buttons; }

        template <typename T>
        T* add_entity(T* ent)
        {
            static_assert(is_base_of<entity, T>::value, "T must derive from entity.");

            auto it = m_entities.begin();
            while (it != m_entities.end() && (*it)->get_layer() <= ent->get_layer()) {
                ++it;
            }
            m_entities.insert(it, ent);

            if constexpr (is_base_of<button, T>::value) {
                m_buttons.push_back(static_cast<button*>(ent));
            }

            return ent;
        }

        text* add_simple_text(string text, float font_size, Color text_color, Vector2 position,
                            int layer);

        button* add_ui_button(string text);

        button* add_text_button(string text, int font_size, Color text_color, Vector2 position);

    protected:
        game& m_game;

    private:
        vector<entity*> m_entities;

        vector<button*> m_buttons;
};

} // NAMESPACE ENGINE.
