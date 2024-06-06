#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct HighScore
{
    std::string name;
    int score;

    HighScore(const std::string &n, int s) : name(n), score(s) {}
};

class HighScoreManager
{
private:
    std::vector<HighScore> highScores;
    const std::string fileName;

public:
    HighScoreManager(const std::string &file) : fileName(file)
    {
        loadHighScores();
    }

    void loadHighScores()
    {
        std::ifstream file(fileName);
        if (!file.is_open())
        {
            std::cerr << "Failed to open high score file!" << std::endl;
            return;
        }

        highScores.clear();
        std::string name;
        int score;
        while (file >> name >> score)
        {
            highScores.emplace_back(name, score);
        }
        file.close();
    }

    void saveHighScores()
    {
        std::ofstream file(fileName);
        if (!file.is_open())
        {
            std::cerr << "Failed to open high score file for writing!" << std::endl;
            return;
        }

        for (const auto &hs : highScores)
        {
            file << hs.name << " " << hs.score << std::endl;
        }
        file.close();
    }

    void addHighScore(const HighScore &hs)
    {
        highScores.push_back(hs);
        std::sort(highScores.begin(), highScores.end(), [](const HighScore &a, const HighScore &b)
                  { return a.score > b.score; });

        if (highScores.size() > 5)
        {
            highScores.pop_back();
        }

        saveHighScores();
    }

    const std::vector<HighScore> &getHighScores() const
    {
        return highScores;
    }
};