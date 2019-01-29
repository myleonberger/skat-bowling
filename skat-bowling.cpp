// skat-bowling.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>

#include "json-develop\include\nlohmann\json.hpp"

const int max_score = 10;

std::ifstream fi;
std::ofstream fo;
nlohmann::json j_scores;
nlohmann::json j_totals;

void calculate_points()
{
	nlohmann::json frames = j_scores["points"];
	int score_1, score_2, frame_score, bonus = 0;
	std::vector<int> results;
	std::vector<int> totals;
	std::vector<bool> spare;
	std::vector<bool> strike;

	for (size_t i = 0; i < frames.size(); i++)
	{
		score_1 = frames[i][0];
		score_2 = frames[i][1];
		frame_score = score_1 + score_2;

		strike.push_back(score_1 == max_score);
		spare.push_back(((frame_score) == max_score) && (score_2 > 0)); // score_1 < 10

		if (i > 0)
		{
			// last frame bonus
			if (i == 9) { if (strike[i] || spare[i]) { bonus = frames[i][2]; } }

			results.push_back(frame_score + bonus);
			// update previous point in case of a striek or spare
			if (strike[i - 1])
			{
				results[i - 1] += frame_score;
				totals[i - 1] += frame_score;
			}
			if (spare[i - 1])
			{
				results[i - 1] += score_1;
				totals[i - 1] += score_1;
			}
			totals.push_back(frame_score + bonus + totals[i - 1]);
		}
		else
		{
			results.push_back(frame_score);
			totals.push_back(frame_score);
		}
	}
	j_totals["token"] = j_scores["token"];
	j_totals["totals"] = totals;

#if 1
	std::cout << j_scores << std::endl << "Frames\t" << frames.size() << "\nScore\t";
	for (size_t i = 0; i < frames.size(); i++) { std::cout << frames[i] << "\t"; }
	std::cout << std::endl << "result\t";
	for (size_t i = 0; i < results.size(); i++) { std::cout << results[i] << "\t"; }
	std::cout << std::endl << "Spare\t";
	for (size_t i = 0; i < spare.size(); i++) { std::cout << spare[i] << "\t"; }
	std::cout << std::endl << "Strike\t";
	for (size_t i = 0; i < strike.size(); i++) { std::cout << strike[i] << "\t"; }
	std::cout << std::endl << "total\t";
	for (size_t i = 0; i < totals.size(); i++) { std::cout << totals[i] << "\t"; }
	std::cout << std::endl << j_totals << std::endl << std::endl;
#endif
}

int main(int arg_c, char* arg_v[])
{
	if (arg_c > 1)
	{
		std::string fname = arg_v[1], dir = "";
		fi.open(fname);
		if (fi.is_open())
		{
			fi >> j_scores;
			fi.close();
			calculate_points();
			if (fname.find('\\', 1) < fname.length()) { dir = fname.substr(0, fname.find('\\', 1)) + "\\"; }
			fname = dir + "results" + fname.substr(fname.find('_', 1));
			fo.open(fname);
			if (fo.is_open())
			{
				fo << j_totals;
				fo.close();
			}
		}
		else
		{
			std::cout << "Can't open file: " << arg_v[1] << std::endl;
		}
	}
	else
	{
		std::cout << "No file specified" << std::endl;
	}
}