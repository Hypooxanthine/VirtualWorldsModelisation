#include "Graphs/FieldGraph.h"

#include <algorithm>

std::vector<int> FieldGraph::shortestPath(int x1, int y1, int x2, int y2) const
{
    int start = x1 + y1 * m_Width;
    int end = x2 + y2 * m_Width;

    std::vector<float> dist(m_Width * m_Height, std::numeric_limits<float>::infinity());
    dist[start] = 0;
    std::vector<int> prev(m_Width * m_Height, -1);
    std::vector<bool> visited(m_Width * m_Height, false);

    while (!visited[end])
    {
        int current = -1;
        float minDist = std::numeric_limits<float>::infinity();
        for (int i = 0; i < m_Width * m_Height; i++)
        {
            if (!visited[i] && dist[i] < minDist)
            {
                current = i;
                minDist = dist[i];
            }
        }

        if (current == -1)
        {
            break;
        }

        visited[current] = true;

        for (int i = 0; i < 8; i++)
        {
            int next = getNeighbour(current, i);
            if (next != -1 && !visited[next])
            {
                float weight = m_Weights[current][i];
                if (dist[current] + weight < dist[next])
                {
                    dist[next] = dist[current] + weight;
                    prev[next] = current;
                }
            }
        }
    }

    std::vector<int> path;
    int current = end;
    while (current != -1)
    {
        path.push_back(current);
        current = prev[current];
    }

    std::reverse(path.begin(), path.end());

    return path;
}