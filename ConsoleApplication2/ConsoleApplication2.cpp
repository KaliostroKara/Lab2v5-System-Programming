#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <string>

class Automaton {
public:
    std::set<char> Alphabet;  
    std::set<int> States;  
    int InitialState;  
    std::set<int> FinalStates;  
    std::map<std::pair<int, char>, int> TransitionFunction;
    Automaton() = default;
};

std::string ComputeW1(const Automaton& automaton, const std::string& w0) {
    for (int state : automaton.States) {
        int current_state = state;
        bool is_accepted = true;
        std::string w1;

        for (char symbol : w0) {
   
            auto it = automaton.TransitionFunction.find({ current_state, symbol });
            if (it == automaton.TransitionFunction.end()) {
                is_accepted = false;
                break;
            }
            else {
                current_state = it->second;
                w1.push_back(symbol);
            }
        }
        if (is_accepted && automaton.FinalStates.count(current_state)) {
            return w1;
        }
    }
    return "";
}

bool LoadAutomatonFromFile(const std::string& filename, Automaton& automaton) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        return false;
    }

    int size;
    char a;

    infile >> size;
    for (int i = 0; i < size; ++i) {
        infile >> a;
        automaton.Alphabet.insert(a);
    }

    infile >> size;
    for (int i = 0; i < size; ++i) {
        automaton.States.insert(i);
    }
    infile >> automaton.InitialState;
    infile >> size;
    for (int i = 0; i < size; ++i) {
        int final_state;
        infile >> final_state;
        automaton.FinalStates.insert(final_state);
    }
    int s, s_prime;
    while (infile >> s >> a >> s_prime) {
        automaton.TransitionFunction[{s, a}] = s_prime;
    }
    infile.close();
    return true;
}
bool IsWordAccepted(const Automaton& automaton, const std::string& w0) {
    return !ComputeW1(automaton, w0).empty();
}

int main() {
    Automaton automaton;
    if (!LoadAutomatonFromFile("automaton.txt", automaton)) {
        std::cerr << "Error reading automaton" << std::endl;
        return 1;
    }
    std::string w0;
    char continueAnswer;

    do {
        std::cout << "Enter word w0: ";
        std::cin >> w0;
        std::string w1 = ComputeW1(automaton, w0);
        if (!w1.empty()) {
            std::cout << "A word w1 = " << w1 << " exists such that w = w1w0 is accepted by the automaton" << std::endl;
        }
        else {
            std::cout << "No such word w1 exists that w = w1w0 is accepted by the automaton" << std::endl;
        }
        std::cout << "Would you like to check another word? (y/n): ";
        std::cin >> continueAnswer;

    } while (continueAnswer == 'y' || continueAnswer == 'Y');

    return 0;
}