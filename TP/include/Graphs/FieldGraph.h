#pragma once

#include <vector>
#include <array>
#include <limits>

/**
 * @brief Classe de graph particulière : chaque noeud peut être voisin avec seulement 8 autres noeuds
 * 
 *  0 1 2
 *  7 n 3
 *  6 5 4
 * 
 * Il n'y a pas d'adjacence car on est en 8-connexité
 * Par contre on a besoin d'avoir les poids des arêtes
 */
class FieldGraph
{
public:

    template <typename Fn>
    inline FieldGraph(int width, int height, Fn f)
        : m_Width(width), m_Height(height), m_Size(width * height)
    {
        di = { 
            -width - 1, -width, -width + 1,
            -1,                  1,
             width - 1,  width,  width + 1
        };

        m_Weights.resize(width * height);
        for (int i = 0; i < width * height; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                setWeight(i, j, f(i, i + di[j]));
            }
        }

        // On met les poids des arêtes à l'extérieur du terrain à l'infini
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                m_Weights[i][j] = std::numeric_limits<float>::infinity();
            }
        }

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                m_Weights[i + (height - 1) * width][4 + j] = std::numeric_limits<float>::infinity();
            }
        }

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                m_Weights[i * width][(6 + j) % 8] = std::numeric_limits<float>::infinity();
            }
        }

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                m_Weights[(i + 1) * width - 1][2 + j] = std::numeric_limits<float>::infinity();
            }
        }
    }
    
    inline FieldGraph(int width, int height, float defaultWeight)
        : FieldGraph(width, height, [defaultWeight](int, int) { return defaultWeight; })
    {
    }

    inline FieldGraph(int width, int height)
        : FieldGraph(width, height, 1.0f)
    {}

    inline float getWeight(int x, int y, int direction) const
    {
        return m_Weights[x + y * 8][direction];
    }

    inline void setWeight(int i, int direction, float weight)
    {
        m_Weights[i][direction] = weight;
    }

    inline void setWeight(int x, int y, int direction, float weight)
    {
        m_Weights[x + y * 8][direction] = weight;
    }

    inline int getNeighbour(int from, int direction) const
    {
        if (from < 0 || from >= m_Size)
        {
            return -1;
        }

        int x = from % m_Width;
        int y = from / m_Width;

        if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
        {
            return -1;
        }

        int n = from + di[direction];
        if (n < 0 || n >= m_Width * m_Height)
        {
            return -1;
        }

        return n;
    }

    std::vector<int> shortestPath(int x1, int y1, int x2, int y2) const;

private:
    std::vector<std::array<float, 8>> m_Weights;
    int m_Width, m_Height, m_Size;

    std::array<int, 8> di;
};