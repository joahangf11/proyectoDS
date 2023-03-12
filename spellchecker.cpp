/*----------------------------------------------------------
 * Project: Spell Checker
 *
 * Date: 30-Nov-2022
 * Authors:
 *           A01748077 Melisa Saucedo Sanchez
 *           A01748222 Joahan Javier Garcia Fernandez
 ----------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <set>
#include <vector>
#include <unordered_set>
#include <unordered_map>
using namespace std;


void toLower(string& cadena) {
    for (auto& ch : cadena){
        ch = tolower(ch);
    }
}

unordered_map<char,char> codifica = {
        {'b', '1'}, {'f', '1'}, {'p', '1'}, {'v', '1'},
        {'c','2'}, {'g','2'}, {'j','2'}, {'k','2'}, {'q','2'}, {'s','2'}, {'x',2}, {'z',2},
        {'d','3'}, {'t','3'},
        {'l','4'},
        {'m','5'}, {'n','5'},
        {'r','6'}
    };

unordered_set<string> setDictionary;

unordered_map<string,vector<string>> soundexValues;

string soundex(string palabra)
{
    //STEP 1
    string palabraCod= palabra.substr(0,1);
    tolower(palabraCod[0]) ;
    for(int i=1; i < palabra.length(); i++)
    {
        if (codifica.count(palabra[i]))
        {
            palabraCod += codifica[palabra[i]];
        }
    }

    //STEP 2

      if(palabraCod.length()<7)
    {
        palabraCod.append(7-palabraCod.length(),'0');
    } else {
        palabraCod = palabraCod.substr(0, 7);
    }

    return palabraCod;
}

struct word
{
    std::string text;
    int line;
    int column;
};

bool read_words(const std::string input_file_name,std::vector<word>& words)
{
    std::ifstream input_file(input_file_name);
    if (input_file.fail()) {
        return false;
    }
    std::regex reg_exp("[a-zA-Z]+");
    std::smatch match;
    std::string text;
    int line = 0;
    int column = 0;
    while (std::getline(input_file, text)) {
        ++line;
        column = 1;
        while (std::regex_search(text, match, reg_exp)) {
            column += match.position();
            words.push_back({match.str(), line, column});
            column += match.length();
            text = match.suffix().str();
        }
    }

    return true;
}

bool read_onlyWords(unordered_map<string, vector<string>>& soundexValues,unordered_set<string>& setDictionary)
{
    std::ifstream input_file("words.txt");
    if (input_file.fail()) {
        return false;
    }

    std::string text;
    while (std::getline(input_file, text)) {
       setDictionary.insert(text);
       string sndx = soundex(text);

        if (not soundexValues.count(sndx)){
            soundexValues[sndx] = vector<string> {};
        }
        soundexValues[sndx].push_back(text);
    }
    return true;
}

int main(int argc, char* argv[])
{

    // Verificar que este correcto la entrada en la terminal
     if (argc != 2){
        std::cerr << "Please specify the name of the input file.\n";
        exit(1);
    }
    //variable
    vector<word> words;
    std::string file_name = argv[1];

    //ingresamos los valores a las variables
    read_words(file_name,words);

    read_onlyWords(soundexValues, setDictionary);


    //COMPARAR
     unordered_set<string> palabraRepetida;
    for(auto word : words){
        string palabraCompara = word.text;
        toLower(palabraCompara);
        if(not setDictionary.count(palabraCompara) and not palabraRepetida.count(palabraCompara)){ //si no esta en el diccionario y tampoco en el set de palabra repetida
            palabraRepetida.insert(palabraCompara);
            string sndxPC = soundex(palabraCompara);
            cout << "Unrecognized word: "<< '"'<<word.text << '"' <<". First found at line "<< word.line <<", column "<< word.column << ". \n";
            if(soundexValues.count(sndxPC)){
                cout<< "Suggestions: ";
                bool primeraVez = true;
                string resultado;
                for(auto word : soundexValues[sndxPC]){
                    if(primeraVez){
                        primeraVez = false;
                    } else{
                        resultado += ", ";
                    }
                    resultado += word ;
                }
                cout << resultado;
            } else{
                cout<<"No suggestions.";
            }
            cout<<"\n\n";
        }
    }

    return 0;
}