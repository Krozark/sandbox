/**
 *
 * http://deepnight.net/bresenham-magic-raycasting-line-of-sight-pathfinding/#more-499
 *
 */


#include <iostream>
#include "time.h"
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
using namespace std;
 
void CreateMap(sf::Vector2i size, sf::VertexArray& array, vector<vector<int> >& tiles)
{
    tiles = vector<vector<int> >(size.x, vector<int>(size.y, 1));
    for(int r=0 ; r<250 ; r++)
    {
        int w = rand()%20+42 + (rand()%100==0?160:0);
        int h = rand()%16+40 + (rand()%100==0?120:0);
        int x = rand()%(size.x-8-w) + 4;
        int y = rand()%(size.y-8-h) + 4;
        for(int i=0 ; i<w ; i++)
        {
            for(int j=0 ; j<h ; j++)
            {
                tiles[i+x][j+y] = 0;
            }
        }
    }

    for(int i=0 ; i<size.x ; i++)
    {
        for(int j=0 ; j<size.y ; j++)
        {
            if(tiles[i][j]==1)
            {
                int b = rand()%32+70;
                array.append(sf::Vertex(sf::Vector2f(i, j), sf::Color(b, b, b)));
            }
        }
    }
}
 
void DrawRay(int x0, int y0, int dirx, int diry, sf::VertexArray& array, vector<vector<int> >& tiles)
{
    static const sf::Color col(255, 128, 0);
    static const float powaaaa = 16000.f;
    bool swapXY = abs(diry) > abs(dirx);
    if(swapXY)
    {
        x0 ^= y0; y0 ^= x0; x0 ^= y0;
        dirx ^= diry; diry ^= dirx; dirx ^= diry;
    }
    bool invY = (dirx < abs(diry));
    if(invY)
    {
        dirx *= -1;
        diry *= -1;
    }
    int y = y0;
    int err = int(dirx/2.f);
    int ystep = (diry < 0 ? -1 : 1);
    diry = int(abs(diry));
    int x = x0;
    if(invY)
    {
        if(swapXY)
        {
            while(tiles[y][x]!=1)
            {
                if(tiles[y][x]==0)
                {
                    float b = 1.f-(float(((x-x0)*(x-x0)+(y-y0)*(y-y0))/powaaaa));
                    if(b < 0)
                    {
                        break;
                    }
                    array.append(sf::Vertex(sf::Vector2f(y, x), sf::Color(int(float(col.r)*b), int(float(col.g)*b), int(float(col.b)*b))));
                    tiles[y][x] = 2; //mark as "rayed" so a pixel don't get displayed twice
                }
                err -= diry;
                if(err < 0) {
                    y -= ystep;
                    err += dirx;
                }
                x--;
            }
        }
        else
        {
            while(tiles[x][y]!=1)
            {
                if(tiles[x][y]==0)
                {
                    float b = 1.f-(float(((x-x0)*(x-x0)+(y-y0)*(y-y0))/powaaaa));
                    if(b < 0)
                    {
                        break;
                    }
                    array.append(sf::Vertex(sf::Vector2f(x, y), sf::Color(int(float(col.r)*b), int(float(col.g)*b), int(float(col.b)*b))));
                    tiles[x][y] = 2; //mark as "rayed"
                }
                err -= diry;
                if(err < 0)
                {
                    y -= ystep;
                    err += dirx;
                }
                x--;
            }
        }
    }
    else
    {
        if(swapXY)
        {
            while(tiles[y][x]!=1)
            {
                if(tiles[y][x]==0)
                {
                    float b = 1.f-(float(((x-x0)*(x-x0)+(y-y0)*(y-y0))/powaaaa));
                    if(b < 0)
                    {
                        break;
                    }
                    array.append(sf::Vertex(sf::Vector2f(y, x), sf::Color(int(float(col.r)*b), int(float(col.g)*b), int(float(col.b)*b))));
                    tiles[y][x] = 2; //mark as "rayed"
                }
                err -= diry;
                if(err < 0)
                {
                    y += ystep;
                    err += dirx;
                }
                x++;
            }
        }
        else 
        {
            while(tiles[x][y]!=1) 
            {
                if(tiles[x][y]==0)
                {
                    float b = 1.f-(float(((x-x0)*(x-x0)+(y-y0)*(y-y0))/powaaaa));
                    if(b < 0)
                    {
                        break;
                    }
                    array.append(sf::Vertex(sf::Vector2f(x, y), sf::Color(int(float(col.r)*b), int(float(col.g)*b), int(float(col.b)*b))));
                    tiles[x][y] = 2; //mark as "rayed"
                }
                err -= diry;
                if(err < 0)
                {
                    y += ystep;
                    err += dirx;
                }
                x++;
            }
        }
    }
}
 
int main()
{
    srand((unsigned)(time(NULL)));
    sf::RenderWindow window(sf::VideoMode(1600, 900, 32), "Tests...");
    sf::RenderTexture texture;
    texture.create(800, 450);
    sf::VertexArray array;
    sf::VertexArray map;
    array.setPrimitiveType(sf::Points);
    map.setPrimitiveType(sf::Points);
    vector<vector<int> > tiles;
    CreateMap(sf::Vector2i(800, 450), map, tiles);
    while(window.isOpen())
    {
        sf::Event e;
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed) 
            {
                window.close();
            }
            if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
        }
 
 
        window.clear();
        texture.clear();
        array.clear();
        //clear ray map
        for(int i=0 ; i<800 ; i++)
        {
            for(int j=0 ; j<450 ; j++)
            {
                if(tiles[i][j] == 2)
                {
                    tiles[i][j] = 0;
                }
            }
        }
        sf::Vector2f mp(sf::Vector2f(sf::Mouse::getPosition(window))/2.f);
        if(mp.x >= 0 && mp.y >= 0 && mp.x < 800 && mp.y < 450)
        {
            const static int nbrays = 256;
            for(int i=0 ; i<nbrays ; i++) 
            {
                float a = float(i)*2.f*3.1415926535897932384626433f/float(nbrays);
                DrawRay(mp.x, mp.y, int(cos(a)*128.f), int(sin(a)*128.f), array, tiles);
            }
        }
        texture.draw(map);
        texture.draw(array);
        texture.display();
        sf::Sprite screen(texture.getTexture());
        screen.setScale(2, 2);
        window.draw(screen);
        window.display();
    }
    return 0;
}
