#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

class dictionary
{
private:
	class edge;

	class vertex{
	public:
		string word;
		bool isThisWord;
		int weight;
		vertex* pred;
		bool visited;
		vector<edge*> edges;
		vector<vertex*> neighbors;
		list<int> edgeIndex;

		vertex(string s)
		{
			word = s;
			isThisWord = false;
			weight = 0;
			visited = false;
			edges.resize(27, NULL);
			pred = NULL;
		}
	};

	class edge{
	public:
		string spell;
		vertex* connectedVertex;

		edge(string s)
		{
			spell = s;
			connectedVertex = NULL;
		}
	};

	vertex* root;
	list<vertex*> printQueue;
	//******Timer*********
	clock_t start, end, dif;
	//********************

public:
	dictionary(){
		root = new vertex("");
	}

	//**************************************************************************//

	void fileReader()   //read words from txt file.
	{
		string line;
		ifstream words("words.txt");
		if (words.is_open()){
			while (getline(words, line)){
				addNewWord(root, line);
			}
			words.close();
		}
		else
			cout << "Unable to open file. Sorry wth.." << endl;
	}

	//**************************************************************************//

	void addNewWord(vertex* vertexPtr, string word)  //add new word into data structure.
	{
		int AtoZ = word[0] - 97;
		if (AtoZ < 0 || 25 < AtoZ)
			AtoZ = 26;

		string letter = word.substr(0, 1);   //a first letter of word.

		if (vertexPtr->edges[AtoZ] == NULL){		//for the new path, make a new edge and vertex.
			vertexPtr->edges[AtoZ] = new edge(letter);
			vertexPtr->edgeIndex.push_back(AtoZ);
			vertexPtr->edges[AtoZ]->connectedVertex = new vertex(vertexPtr->word + letter);
		}
		vertex* nextVertex = vertexPtr->edges[AtoZ]->connectedVertex;

		if (word.size() == 1){			//when it finish finding path, mark that vertex as "ThisIsWord" , also add its nieghbors
			nextVertex->isThisWord = true;
			searchNeighbor(nextVertex, root, nextVertex->word, 0);
		}

		else{
			addNewWord(nextVertex, word.substr(1, word.size()-1));  //follow word's letter, find a path 
		}
	}

	//**************************************************************************//

	void searchNeighbor(vertex* main, vertex* vrt, string letter, int dismatch)
	{
		int AtoZ = letter[0] - 97;
		if (AtoZ < 0 || 25 < AtoZ)
			AtoZ = 26;

		if (letter.size() == 0){					    //finished finding path, add in neighbor list each others
			if (main->word != vrt->word && vrt->isThisWord == true){
				main->neighbors.push_back(vrt);
				vrt->neighbors.push_back(main);
			}
		}

		else if (letter.size() == 1){					//seperated case since we can't letter.substr(1, size-1) for last letter.
			if (dismatch == 0){							//seperated case for path which has no different letter.
				for each(int index in vrt->edgeIndex){	//since dismatch is 0, every letter should be added into neighbor list.
					if (vrt->edges[index]->connectedVertex->word != main->word){
						vrt->edges[index]->connectedVertex->neighbors.push_back(main);
						main->neighbors.push_back(vrt->edges[index]->connectedVertex);
					}
				}
			}
			else{										//case for path which has 1 different letter already.
				int lastLetter = letter[0] - 97;		//adding last letter, with 1 dismatch, we have to follow same letter path only.
				if (lastLetter < 0 || 25 < lastLetter)
					lastLetter = 26;
				if (vrt->edges[lastLetter] != NULL){
					vrt->edges[lastLetter]->connectedVertex->neighbors.push_back(main);
					main->neighbors.push_back(vrt->edges[lastLetter]->connectedVertex);
				}
			}
		}
		else{											//case for letter.size >  1
			if (dismatch == 0){						
				for each(int index in vrt->edgeIndex){  //since dismatch is 0, every letter should be checked.
					if (vrt->edges[index]->spell == letter.substr(0, 1)) // spell is same with next letter
						searchNeighbor(main, vrt->edges[index]->connectedVertex, letter.substr(1, letter.size() - 1), 0);
					else {// spell is different with next letter
						searchNeighbor(main, vrt->edges[index]->connectedVertex, letter.substr(1, letter.size() - 1), 1); // dismatch++ 
					}
				}
			}
			else{									  //dismatch is already 1, only same letter should be checked.
				if (vrt->edges[AtoZ] != NULL){
					searchNeighbor(main, vrt->edges[AtoZ]->connectedVertex, letter.substr(1, letter.size() - 1), 1);
				}
			}
		}
	}

	//**************************************************************************//

	vertex* vertexFinder(vertex* v, string word, string letter)  //find a vertex with a word.
	{
		int AtoZ = letter[0] - 97;
		if (AtoZ < 0 || 25 < AtoZ)
			AtoZ = 26;

		if (v->word == word && v->isThisWord == true)
			return v;   //I found it
		else{
			if (v->edges[AtoZ] != NULL){
				if (letter.size() != 1)
					return vertexFinder(v->edges[AtoZ]->connectedVertex, word, letter.substr(1, letter.size() - 1));
				else
					return vertexFinder(v->edges[AtoZ]->connectedVertex, word, letter.substr(0, letter.size()));
			}
			else
				return NULL;
		}
	}

	//**************************************************************************//

	void pathFinder()
	{
		cout << "enter first word : ";
		string first;
		getline(cin, first);
		cout << "enter second word : ";
		string second;
		getline(cin, second);
		
		start = clock();  //clock start

		if (first.size() != second.size())
			cout << "Please put same size of words" << endl;
		else{
			vertex* start = NULL;
			start = vertexFinder(root, first, first);
			if (start == NULL)
				cout << "could not find a first word" << endl;
			else{
				searchPath(start);
				vertex* destination = NULL;
				destination = vertexFinder(root, second, second);
				if (destination == NULL)
					cout << "could not find a second word" << endl;
				else{
					if (destination->pred == NULL)
						cout << "sorry, no path exist" << endl;
					else
						printPath(first, destination);
				}
			}
		}
		end = clock();
		dif = end - start;
		cout << "Takes *Only* :" << dif << " milliseconds." << endl;
		resetVertex();
	}

	//**************************************************************************//

	void searchPath(vertex* main)
	{
		list<vertex*> searchQ;
		searchQ.push_back(main);
		while (!searchQ.empty()){
			vertex* check = searchQ.front();
			searchQ.pop_front();
			for each(vertex* v in check->neighbors){
				if (v->visited == false){
					v->visited = true;
					v->pred = check;
					v->weight = check->weight + 1;
					searchQ.push_back(v);
				}
				else{
					if (check->weight + 1 < v->weight){
						v->weight = check->weight + 1;
						v->pred = check;
						searchQ.push_back(v);
					}
				}
			}
		}

	}

	//**************************************************************************//

	void printPath(string s, vertex* v)
	{
		if (v->word == s){
			printQueue.push_front(v);
			while (!printQueue.empty()){
				vertex* print = printQueue.front();
				printQueue.pop_front();
				cout << print->word << endl;
			}
			printQueue.clear();
		}
		else{
			printQueue.push_front(v);
			printPath(s, v->pred);
		}
	}

	//**************************************************************************//

	void resetVertex()		//for search path, reset vertex's weight, visited, pred
	{
		list<vertex*> Queue;
		Queue.push_back(root);
		while (!Queue.empty())
		{
			vertex* print = Queue.front();
			Queue.pop_front();
			for each(int index in print->edgeIndex){
				vertex* thisVertex = print->edges[index]->connectedVertex;
				Queue.push_back(thisVertex);
				if (thisVertex->isThisWord == true){
					thisVertex->pred = NULL;
					thisVertex->weight = 0;
					thisVertex->visited = false;
				}
			}
		}
	}
};