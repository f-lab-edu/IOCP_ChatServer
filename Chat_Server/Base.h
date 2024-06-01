#pragma once

struct Pos
{
    int x;
    int y;

    Pos operator+(Pos& pos)
    {
        return {x + pos.x, y + pos.y};
    }
    Pos operator-(Pos& pos)
    {
        return {x - pos.x, y - pos.y};
    }
    Pos operator*(Pos& pos)
    {
        return {x * pos.x, y * pos.y};
    }
    Pos operator/(Pos& pos)
    {
        return {x / pos.x, y / pos.y};
    }
    Pos operator%(Pos& pos)
    {
        return {x % pos.x, y % pos.y};
    }
};
