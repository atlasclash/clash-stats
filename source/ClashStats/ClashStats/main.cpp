//
//  main.cpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include <iostream>
#include <string>
#include "ParserFactory.hpp"
#include "Parser.hpp"
#include "Options.hpp"
#include "WarData.hpp"

const int QUIT_OPTION	= 9;

WarData *g_WarData = NULL;

void ui_LoadWarData()
{
	std::string fileName;
	
	std::cout << "Enter filename to load"					<< std::endl;
	std::cin >> fileName;
	
	Parser *parser = PARSER::GetInstance().getParserForFile(fileName.c_str());
	if (parser == NULL)
	{
		std::cout << "Unable to parse file: " << fileName	<< std::endl;
		return;
	}
	
	if (g_WarData != NULL)
	{
		delete g_WarData;
	}
	
	g_WarData = new WarData();
	parser->ProcessWar(g_WarData);
	g_WarData->CalcCloserStars();
	
	delete parser;
}

void ui_AnalyzeWarData()
{
	int choice = 0;
	
	if (g_WarData == NULL)
	{
		std::cout << "No war data loaded."	<< std::endl;
		return;
	}
	
	while (choice != QUIT_OPTION)
	{
		std::cout << "Analyze War Data"					<< std::endl;
		std::cout << "[1] Final score"					<< std::endl;
		std::cout << "[2] Player summaries"				<< std::endl;
		std::cout << "[3] MIA"							<< std::endl;
		std::cout << "[4] Nukes"						<< std::endl;
		std::cout << "[5] Snipes"						<< std::endl;
		std::cout << "[9] Back"							<< std::endl;
		std::cin >> choice;
		
		switch (choice)
		{
			case 1:
				g_WarData->ReportFinalScore();
				break;
			case 2:
				g_WarData->ReportPlayerStats();
				break;
			case 3:
				g_WarData->ReportWarningMissingInAction();
				break;
			case 4:
				g_WarData->ReportWarningNuke();
				break;
			case 5:
				g_WarData->ReportWarningSnipe();
				break;
			case QUIT_OPTION:
			default:
				break;
		}
		
		std::cout << std::endl;
	}
}

void ui_Options()
{
	int choice = 0;
	
	while (choice != QUIT_OPTION)
	{
		std::string checkDataRanges = (OPTIONS::GetInstance().parser_Check_Data_Ranges) ? "[ON]" : "[off]";
		std::string checkPlayerTotals = (OPTIONS::GetInstance().parser_Check_Player_Totals)	? "[ON]" : "[off]";
		std::string checkMissingAttacks = (OPTIONS::GetInstance().parser_Check_Missing_Attacks) ? "[ON]" : "[off]";
		
		std::cout << "Options"													<< std::endl;
		std::cout << "[1] Check Data Ranges "		<< checkDataRanges			<< std::endl;
		std::cout << "[2] Check Player Totals "		<< checkPlayerTotals		<< std::endl;
		std::cout << "[3] Check Missing Attacks "	<< checkMissingAttacks		<< std::endl;
		std::cout << "[9] Quit"													<< std::endl;
		std::cin >> choice;
		
		switch (choice)
		{
			case 1:
				OPTIONS::GetInstance().parser_Check_Data_Ranges = !OPTIONS::GetInstance().parser_Check_Data_Ranges;
				break;
			case 2:
				OPTIONS::GetInstance().parser_Check_Player_Totals = !OPTIONS::GetInstance().parser_Check_Player_Totals;
				break;
			case 3:
				OPTIONS::GetInstance().parser_Check_Missing_Attacks = !OPTIONS::GetInstance().parser_Check_Missing_Attacks;
				break;
			case 9:
			default:
				return;
				break;
		}
		
		std::cout << std::endl;
	}
}

bool ui_MainMenu()
{
	int choice = 0;
	
	while (choice != QUIT_OPTION)
	{
		std::cout << "Main Menu"						<< std::endl;
		std::cout << "[1] Load war data"				<< std::endl;
		std::cout << "[2] Analyze war data"				<< std::endl;
		std::cout << "[3] Write war data to DB"			<< std::endl;
		std::cout << "[4] Options"						<< std::endl;
		std::cout << "[9] Quit"							<< std::endl;
		std::cin >> choice;

		switch (choice)
		{
			case 1:
				ui_LoadWarData();
				break;
			case 2:
				ui_AnalyzeWarData();
				break;
			case 3:
				std::cout << "Unsupported" << std::endl;
				break;
			case 4:
				ui_Options();
				break;
			case QUIT_OPTION:
			default:
				return false;
		}
		
		std::cout << std::endl;
	}
	
	return true;
}

int main(int argc, const char * argv[])
{
	OPTIONS::Instantiate();
	PARSER::Instantiate();

	// assume second argument is a file to parse, otherwise go into interactive mode
	if (argc != 2)
	{
		while (ui_MainMenu())
			;
	}
	else
	{
		g_WarData = new WarData();
		Parser *parser = PARSER::GetInstance().getParserForFile(argv[1]);
		if (parser == NULL)
		{
			std::cout << "Unable to parse file" << std::endl;
			return -1;
		}
		
		parser->ProcessWar(g_WarData);
		g_WarData->CalcCloserStars();
		g_WarData->RunReports();
	}
	
	if (g_WarData != NULL)
		delete g_WarData;
	
	OPTIONS::Destroy();
	PARSER::Destroy();
	
    return 0;
}
