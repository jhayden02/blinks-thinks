/***********************************************************************************************
*
*   button.cpp - The library for creating, drawing, and interacting with buttons.
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

// Source.
#include "button.hpp"
#include "game.hpp"

// Standard library.
#include <cmath>

using engine::button;
using engine::text;
using engine::game;

button::button(
    text* text_obj,
    Color bg_color,
    Rectangle rec,
    int layer,
    Color outline_color,
    float outline_size)
    :
    entity({rec.x + rec.width / 2.0f, rec.y + rec.height / 2.0f}, layer),
    m_text_obj(text_obj),
    m_rec(rec),
    m_scaled_rec(rec),
    m_default_text_color(text_obj->get_text_color()),
    m_current_text_color(m_default_text_color),
    m_default_bg_color(bg_color),
    m_current_bg_color(m_default_bg_color),
    m_outline_color(outline_color),
    m_outline_size(outline_size),
    m_scale(1.0f)
{
    m_text_obj->set_position(m_position);
    m_rec.x = m_position.x;
    m_rec.y = m_position.y;
}

button::~button()
{
    delete m_text_obj;

    for (auto* trait : m_traits) {
        delete trait;
    }
    m_traits.clear();
}

bool button::is_hovered()
{
    return CheckCollisionPointRec(GetMousePosition(), m_scaled_rec);
}

bool button::is_pressed()
{
    return is_hovered() && IsMouseButtonPressed(0);
}

void button::update()
{ 
    entity::update();

    for (auto& trait : m_traits) {
        trait->update(*this);
    }

    m_scaled_rec = {
        m_position.x - ((m_rec.width * m_scale) / 2.0f),
        m_position.y - ((m_rec.height * m_scale) / 2.0f),
        m_rec.width * m_scale,
        m_rec.height * m_scale
    };

    m_text_obj->set_scale(m_scale);
    m_text_obj->set_position(m_position);

    m_current_text_color = is_hovered()
        ? brighten_color(m_default_text_color)
        : m_default_text_color;

    m_current_bg_color = (is_hovered() && m_default_bg_color.a != 0)
        ? brighten_color(m_default_bg_color)
        : m_default_bg_color;

    if (is_pressed() && m_sfx_press.has_value()) {
        PlaySound(*m_sfx_press);
    }

    m_text_obj->set_text_color(m_current_text_color); 

    m_text_obj->update(); 
}

void button::draw()
{
    DrawRectangleRec(m_scaled_rec, m_current_bg_color);
    DrawRectangleLinesEx(m_scaled_rec, m_outline_size, m_outline_color);
    m_text_obj->draw();
}

Color button::brighten_color(Color color)
{
    return { 
        static_cast<unsigned char>(fmin(color.r * m_brighten_factor, 255)),
        static_cast<unsigned char>(fmin(color.g * m_brighten_factor, 255)),
        static_cast<unsigned char>(fmin(color.b * m_brighten_factor, 255)),
        color.a
    };
}
