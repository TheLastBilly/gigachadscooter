#include "engine/inputs.hpp"

using namespace engine;

#define keyCase(id, key) case key: index = id; break;

keyboardHandler::keyboardHandler(const std::string &name):
    node(name)
{}

void keyboardHandler::updateInputs(const SDL_Event &event)
{
    unsigned int index = 0;

    switch (event.key.keysym.sym)
    {
        keyCase(key::up, SDLK_UP);
        keyCase(key::down, SDLK_DOWN);
        keyCase(key::left, SDLK_LEFT);
        keyCase(key::right, SDLK_RIGHT);

        keyCase(key::q, SDLK_q);
        keyCase(key::w, SDLK_w);
        keyCase(key::e, SDLK_e);
        keyCase(key::r, SDLK_r);
        keyCase(key::t, SDLK_t);
        keyCase(key::y, SDLK_y);
        keyCase(key::u, SDLK_u);
        keyCase(key::i, SDLK_i);
        keyCase(key::o, SDLK_o);
        keyCase(key::p, SDLK_p);
        keyCase(key::a, SDLK_a);
        keyCase(key::s, SDLK_s);
        keyCase(key::d, SDLK_d);
        keyCase(key::f, SDLK_f);
        keyCase(key::g, SDLK_g);
        keyCase(key::h, SDLK_h);
        keyCase(key::j, SDLK_j);
        keyCase(key::k, SDLK_k);
        keyCase(key::l, SDLK_l);
        keyCase(key::z, SDLK_z);
        keyCase(key::x, SDLK_x);
        keyCase(key::c, SDLK_c);
        keyCase(key::v, SDLK_v);
        keyCase(key::b, SDLK_b);
        keyCase(key::n, SDLK_n);
        keyCase(key::m, SDLK_m);

        keyCase(key::num0, SDLK_0);
        keyCase(key::num1, SDLK_1);
        keyCase(key::num2, SDLK_2);
        keyCase(key::num3, SDLK_3);
        keyCase(key::num4, SDLK_4);
        keyCase(key::num5, SDLK_5);
        keyCase(key::num6, SDLK_6);
        keyCase(key::num7, SDLK_7);
        keyCase(key::num9, SDLK_9);

        keyCase(key::space, SDLK_SPACE);
        keyCase(key::enter, SDLK_RETURN);
        keyCase(key::scape, SDLK_ESCAPE);

        keyCase(key::dot, SDLK_PERIOD);
        keyCase(key::comma, SDLK_COMMA);
    
    default:
        index = key::none;
        break;
    }

    if(event.type == SDL_KEYDOWN)
        inputState[index] = true;
    else if(event.type == SDL_KEYUP )
        inputState[index] = false;
}