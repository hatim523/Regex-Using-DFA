#include<iostream>
#include<string>
#include <vector> 
#include<string.h>

using namespace std;

int DetermineRegex(string regex);
char characters[26] = { 'a', 'b', 'c','d','e','f','g','h', 'i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
int state_number = 0;		//For debugging-only

class Node
{
private:
	int state_name;				//For debugging purpose-only
	bool isFinalState;
	int num_input_characters = 26;		//Considering only lower case letters
	int current_transition_number;		//keeps track of transition_node array number
	Node **transition_node;
	char *copy_characters;
	int *transition_map;		//helps in keeping track of transition node at a certain character
public:
	Node()
	{
		isFinalState = false;
		transition_node = 0;
		transition_node = new Node*[26];

		copy_characters = new char[26];
		memcpy(copy_characters, characters, sizeof(char) * 26);
		transition_map = new int[26];
		memset(transition_map, -1, sizeof(int) * 26);
		current_transition_number = 0;
		//For debugging only
		state_name = state_number++;
	}
	void changeToFinalState()	//Changes the state status to final
	{
		isFinalState = true;
	}
	bool addTransition(char read_input, Node *transition_address)
	{
		bool transition_added = false;
		//removing the read_input from copy_characters so that transition is not duplicated
		for (int i = 0; i < 26; i++)
		{
			if (read_input == copy_characters[i])
			{
				copy_characters[i] = '\0';
				transition_added = true;
				transition_map[i] = current_transition_number;
				break;
			}
		}

		if (transition_added)	//if the transition of read_input has not been added yet
		{
			transition_node[current_transition_number++] = transition_address;
			return true;
		}
		else
			return false;
	}
	void addDefaultTransition(Node *to_node)
	{
		if (current_transition_number == 26)	//If transitions are already added
			return;
		for (int i = 0; i < 26; i++)
		{
			if (copy_characters[i] != '\0')
			{
				transition_map[i] = current_transition_number;
				transition_node[current_transition_number++] = to_node;
			}
		}
	}
	void PrintTransitionNodes()
	{
		for (int i = 0; i < 26; i++)
		{
			cout << "Node: " << state_name << " transition at " << characters[i] << " Node: " << transition_node[transition_map[i]]->state_name << endl;
		}
	}
	Node* getTransitionNode(int character_map)
	{
		int transition_number = transition_map[character_map];
		return transition_node[transition_number];
	}
	bool isFinal()
	{
		if (isFinalState)
			return true;
		return false;
	}
	~Node()
	{
		
	}
};

class DFA
{
public:
	bool startTraversal(Node *head, string input_string)
	{
		Node *temp = head;
		for (int i = 0; i < input_string.length(); i++)
		{
			temp = temp->getTransitionNode(getCharacterInt(input_string[i]));
		}
		if (temp->isFinal())
			return true;
		return false;
	}
	int getCharacterInt(char c)
	{
		for (int i = 0; i < 26; i++)
		{
			if (c == characters[i])
				return i;
		}
	}
};

int main()
{
	int filename_size;
	cin >> filename_size;

	/*
	*	Taking input files
	*/
	vector<string> files;
	for (int i = 0; i < filename_size; i++)
	{
		string a;
		cin >> a;
		files.push_back(a);
	}

	/* 
	* Regex Input
	*/
	cout << "Enter Regex: ";
	string regex;
	cin >> regex;

	/*
	* Determining which regex was typed
	*/
	int regex_DFA = DetermineRegex(regex);
	int regex_length = regex.length();
	DFA string_check;
	Node **DFA_Nodes;
	cout << "\t\tMatches found\n\n";
	if (regex_DFA == 1)
	{
		int total_nodes = regex_length + 1;
		DFA_Nodes = new Node*[total_nodes];
		
		//Initializing nodes
		for (int i = 0; i < total_nodes; i++)
			DFA_Nodes[i] = new Node();

		//adding transitions
		for (int i = 0; i < total_nodes - 2; i++)
		{
			DFA_Nodes[i]->addTransition(regex[i], DFA_Nodes[i + 1]);
		}

//		DFA_Nodes[0]->addTransition('a', DFA_Nodes[1]);
//		DFA_Nodes[1]->addTransition('b', DFA_Nodes[2]);
//		DFA_Nodes[2]->addTransition('c', DFA_Nodes[3]);
		
		//adding transitions to dead state
		for (int i = 0; i < total_nodes - 2; i++)
		{
			DFA_Nodes[i]->addDefaultTransition(DFA_Nodes[total_nodes - 1]);
		}

//		DFA_Nodes[0]->addDefaultTransition(DFA_Nodes[4]);
//		DFA_Nodes[1]->addDefaultTransition(DFA_Nodes[4]);
//		DFA_Nodes[2]->addDefaultTransition(DFA_Nodes[4]);

		//Final state preparation
		DFA_Nodes[total_nodes - 2]->addDefaultTransition(DFA_Nodes[total_nodes - 2]);
		DFA_Nodes[total_nodes - 2]->changeToFinalState();

		//Dead State transitions
		DFA_Nodes[total_nodes - 1]->addDefaultTransition(DFA_Nodes[total_nodes - 1]);

//		for (int i = 0; i < total_nodes; i++)
//			DFA_Nodes[i]->PrintTransitionNodes();

		for (int i = 0; i < filename_size; i++)
		{
			if (string_check.startTraversal(DFA_Nodes[0], files[i]))
				cout << files[i] << endl;
		}
	}
	else if (regex_DFA == 2)		//Guaranteed working only for unique characters...might not work for regex such as *aaab or *aaa
	{
		int total_nodes = regex_length;
		DFA_Nodes = new Node*[total_nodes];

		//initializing nodes
		for (int i = 0; i < total_nodes; i++)
			DFA_Nodes[i] = new Node();

		//Adding transition condition to nodes
		for (int i = 0; i < total_nodes - 1; i++)
			DFA_Nodes[i]->addTransition(regex[i+1], DFA_Nodes[i + 1]);

//		DFA_Nodes[0]->addTransition('a', DFA_Nodes[1]);
//		DFA_Nodes[1]->addTransition('b', DFA_Nodes[2]);
//		DFA_Nodes[2]->addTransition('c', DFA_Nodes[3]);

		//Adding unique transitions
		for (int i = 1; i < total_nodes; i++)
			DFA_Nodes[i]->addTransition(regex[1], DFA_Nodes[1]);

//		DFA_Nodes[1]->addTransition('a', DFA_Nodes[1]);
	//	DFA_Nodes[2]->addTransition('a', DFA_Nodes[1]);
	//	DFA_Nodes[3]->addTransition('a', DFA_Nodes[1]);

		//Adding default transitions for every node
		for (int i=0; i<total_nodes; i++)
			DFA_Nodes[i]->addDefaultTransition(DFA_Nodes[0]);

//		DFA_Nodes[0]->addDefaultTransition(DFA_Nodes[0]);
//		DFA_Nodes[1]->addDefaultTransition(DFA_Nodes[0]);
//		DFA_Nodes[2]->addDefaultTransition(DFA_Nodes[0]);
//		DFA_Nodes[3]->addDefaultTransition(DFA_Nodes[0]);

		//Preparing final state
		DFA_Nodes[total_nodes - 1]->changeToFinalState();

		//Now checking for strings that matches the regex
		for (int i = 0; i < filename_size; i++)
		{
			if (string_check.startTraversal(DFA_Nodes[0], files[i]))
				cout << files[i] << endl;
		}
	}
	else if (regex_DFA == 3)	//Characters after * must be unique...might not work if characters are duplicated
	{
		int total_nodes = regex_length + 2;	//+1 for dead state
		DFA_Nodes = new Node*[total_nodes];

		//Initializing all states
		for (int i = 0; i < total_nodes; i++)
			DFA_Nodes[i] = new Node();

		//Adding transitions for first part before *
		int asterik_position = regex.find("*");
		for (int i = 0; i < asterik_position; i++)
		{
			DFA_Nodes[i]->addTransition(regex[i], DFA_Nodes[i + 1]);
			DFA_Nodes[i]->addDefaultTransition(DFA_Nodes[total_nodes - 1]);
		}

		//Adding dead state transition
		DFA_Nodes[total_nodes - 1]->addDefaultTransition(DFA_Nodes[total_nodes - 1]);

		//Adding transitions for second part after *
		DFA_Nodes[asterik_position]->addTransition(regex[asterik_position + 1], DFA_Nodes[asterik_position + 2]);
		DFA_Nodes[asterik_position]->addDefaultTransition(DFA_Nodes[asterik_position + 1]);

		for (int i = asterik_position + 1; i < total_nodes - 2; i++)
		{
			DFA_Nodes[i]->addTransition(regex[i], DFA_Nodes[i + 1]);
		}

		for (int i = asterik_position + 2; i < total_nodes - 1; i++)
		{
			DFA_Nodes[i]->addTransition(regex[asterik_position + 1], DFA_Nodes[asterik_position + 2]);
		}

		//Adding transition to node which handles *
		DFA_Nodes[asterik_position + 1]->addDefaultTransition(DFA_Nodes[asterik_position + 1]);

		//Adding default transitions to left over Nodes
		for (int i = asterik_position + 2; i < total_nodes - 1; i++)
			DFA_Nodes[i]->addDefaultTransition(DFA_Nodes[asterik_position + 1]);

		//Preparing final state
		DFA_Nodes[total_nodes - 2]->changeToFinalState();

		//Now checking for strings that matches the regex
		for (int i = 0; i < filename_size; i++)
		{
			if (string_check.startTraversal(DFA_Nodes[0], files[i]))
				cout << files[i] << endl;
		}
	}
	else if (regex_DFA == 4)
	{
		int total_nodes = regex_length + 2;	//+1 for dead state
		DFA_Nodes = new Node*[total_nodes];

		//Initializing all states
		for (int i = 0; i < total_nodes; i++)
			DFA_Nodes[i] = new Node();

		//Adding transitions
		for (int i = 0; i < total_nodes - 1; i++)
		{
			if (regex[i] != '.')
				DFA_Nodes[i]->addTransition(regex[i], DFA_Nodes[i + 1]);
			else if (regex[i] == '.')
				DFA_Nodes[i]->addDefaultTransition(DFA_Nodes[i + 1]);
		}

		//Adding dead state transitions
		for (int i = 0; i < total_nodes; i++)
			DFA_Nodes[i]->addDefaultTransition(DFA_Nodes[total_nodes - 1]);

		//Preparing final state
		DFA_Nodes[total_nodes - 2]->changeToFinalState();

		//Now checking for strings that matches the regex
		for (int i = 0; i < filename_size; i++)
		{
			if (string_check.startTraversal(DFA_Nodes[0], files[i]))
				cout << files[i] << endl;
		}
	}
	else if (regex_DFA == 5)
	{

	}
	else
		cout << "Please enter a valid regex" << endl;
	

	return 0;
}

int DetermineRegex(string regex)
{
	/*
	* 1. abc*
	* 2. *abc
	* 3. abc*abc
	* 4. abc..def
	* 5. abc*abc*abc
	*/

	if (regex[regex.length() - 1] == '*')
		return 1;
	else if (regex[0] == '*')
		return 2;
	else if (regex.find(".") < regex.length())
		return 4;
	else
	{
		int num_asteriks = 0;
		for (int i = 1; i < regex.length(); i++)
		{
			if (regex[i] == '*')
				num_asteriks++;
		}
		if (num_asteriks == 1)
			return 3;
		else if (num_asteriks == 2)
			return 5;
	}
	return -1;
}
