#include "./../headers/Particle.hpp"
#include <iostream>

Particle::Particle()
    : x(200), y(200), fov(40)
{
    n_rays = (int)(fov / 1);
    rays = new Ray[n_rays];
    int c = 0;

    angleLow = -fov / 2;
    angleHigh = -angleLow;

    for (float i = -fov/2; i < fov/2; i += 1)
        rays[c++] = Ray(x, y, i);
}

void Particle::Look(Boundary &wall, Renderer *r)
{
    for (int i = 0; i < n_rays; i++)
    {
        Point pt = rays[i].Cast(wall);
        if (PointExist(pt))
            r->Line(x, y, pt.x, pt.y, {1, 1, 1, 1});
    }
}


void Particle::Show(Renderer * r)
{
    r->DrawCircle(x, y, 4, {1, 1, 1, 1});

    for (int i = 0; i < n_rays; i++)
        rays[i].Show(r);
}

void Particle::Update(float x_, float y_)
{
    this->x = x_;
    this->y = y_;

    for (int i = 0; i < n_rays; i++)
    {
        rays[i].posX = x_;
        rays[i].posY = y_;
    }

}

std::vector<float> Particle::Look(std::vector<Boundary> &walls, Renderer *r)
{
    std::vector<float> rv(n_rays);
    for (int i = 0; i < n_rays; i++)
    {
        float record = INFINITY;
        Point winner = {-1, -1};

        for (int j = 0; j < walls.size(); j++)
        {
            Point pt = rays[i].Cast(walls[j]);
            float d;
            if (PointExist(pt))
            {
                d = dist(x, y, pt.x, pt.y);
                if (d < record)
                {
                    record = d;
                    winner = pt;
                }
            }
        }
        
        if (PointExist(winner))
            r->Line(x, y, winner.x, winner.y, {1, 1, 1, 1});
        rv[i] = record;    
    }
    return rv;
}

void Particle::Rotate(float amount)
{
    angleLow += amount;
    angleHigh += amount;

    int c = 0;
    for (float i = angleLow; i < angleHigh; i += 1)
        rays[c++] = Ray(x, y, i);
}