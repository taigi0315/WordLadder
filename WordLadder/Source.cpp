#include <iostream>
#include "buildDictionary.h"
#include <ctime>

using namespace std;

int main()
{
	//******Timer*********
	clock_t start, end, dif;
	start = clock();
	end = clock();
	dif = end - start;
	//********************
	dictionary myDictionary;

	cout << "Inserting word into Data Structure..." << endl;

	start = clock();
	myDictionary.fileReader();
	end = clock();
	dif = end - start;
	cout << "Takes *Only* :" << dif << " milliseconds."<< endl;

	//myDictionary.testPrint();

	while (true){
		myDictionary.pathFinder();
	}
	return 0;
}