#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

#define EXIT_SUCCESS 0
#define EXIT_FAILIURE 1
// If true, does few additional prints and doesn't remove additional cpp or object files
bool DEBUG = false;

enum class TokenType {
    variable,
    number,
    add,
    sub,
    multi,
    div,
    equal,
    dot,
    print,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};


std::vector<Token> tokenize(const std::string &str) {
    std::vector<Token> tokens;
    std::string buffer;

    for(int i = 0; i < str.length(); i++){
        char c = str.at(i);

        // ----- Letters & Words
        if(std::isalpha(c)){
            buffer.push_back(c);
            if(i < str.length()){ // Check for overflow
                i++;
                while (i < str.length() && std::isalpha(str.at(i))){
                    if(i < str.length()){
                        buffer.push_back(str.at(i));
                        i++;
                    }
                }
                i--;
                // check for print function
                // Otherwise mark as a variable
                if(buffer == "print"){
                    tokens.push_back({.type = TokenType::print});
                    buffer.clear();
                } else {
                    tokens.push_back({.type = TokenType::variable, .value = buffer});
                    buffer.clear();
                }
            }

        }
        // ----- Numbers
        else if(std::isdigit(c)){
            buffer.push_back(c);
            if(i < str.length()) {
                i++;
                // Check for decimal "dots" in numbers
                if(str.at(i) == '.'){
                    buffer.push_back(str.at(i));
                    i++;
                }
                while(i < str.length() && std::isdigit(str.at(i)) || str.at(i) == '.'){
                    if(i < str.length()) {
                        buffer.push_back(str.at(i));
                        i++;
                    }
                }
                i--;
                tokens.push_back({.type = TokenType::number, .value = buffer});
                buffer.clear();
            }
        }
        // ----- Math symbols
        else if(c == '+'){
            tokens.push_back({.type = TokenType::add});
        }
        else if(c == '-'){
            tokens.push_back({.type = TokenType::sub});
        }
        else if(c == '*'){
            tokens.push_back({.type = TokenType::multi});
        }
        else if(c == '/'){
            tokens.push_back({.type = TokenType::div});
        }
        else if(c == '='){
            tokens.push_back({.type = TokenType::equal});
        }
        // ----- Other symbols
        else if(c == ';'){
            tokens.push_back({.type = TokenType::semi});
        }
        else if(std::isspace(c)){
            continue;
        }
        // Check for line ending (\n)
        else if(str.at(i) == '\\' && str.at(i+1) == 'n'){
            continue;
        }
        else if(str.at(i-1) == '\\' && str.at(i) == 'n'){
            continue;
        }
        else {
            std::cerr << "--!Syntax Error!--"<< std::endl;
            std::cerr << c << std::endl;
            std::cerr << "This is not correct syntax" << std::endl;
        }
    }

    if(DEBUG == true){
        std::cout << "[+] File tokenized Successfully" << std::endl;
    }

    return tokens;
}

std::string assembler(const std::vector<Token> &tokens){
    std::stringstream output;
    // As the name implies, stores variable names.
    std::vector<std::string> usedVar;

    // Header
    output << "#include <iostream>\n";
    output << "int main(){\n";

    for(int i = 0; i < tokens.size(); i++){
        
        
        switch (tokens.at(i).type) {
            case TokenType::variable: // Check for variable
                if(usedVar.size() < 1){
                    // if variable has not been used yet, initialize it
                    output << "double " + tokens.at(i).value.value();
                    usedVar.push_back(tokens.at(i).value.value());
                } else {
                    bool isUsed = false;
                    for(int k = 0; k < usedVar.size(); k++){
                        if(tokens.at(i).value.value() == usedVar[k]){
                            isUsed = true; // Turns true if variable is in the used variable list
                        }
                    }
                    if(isUsed == true){
                        output << " " + tokens.at(i).value.value() + " ";
                    } else {
                        output << "double " + tokens.at(i).value.value();
                    }
                    usedVar.push_back(tokens.at(i).value.value());
                }

                
            break;
            
            case TokenType::number:
                output << " " + tokens.at(i).value.value();
            break;

            case TokenType::add:
                output << " + ";
            break;

            case TokenType::sub:
                output << " - ";
            break;

            case TokenType::multi:
                output << " * ";
            break;

            case TokenType::div:
                output << " / ";
            break;

            case TokenType::equal:
                output << " = ";
            break;

            case TokenType::print:
                output << "std::cout << ";
                for(int j = i; j < tokens.size() - 1; j++){
                    if(tokens.at(j).type == TokenType::semi){
                        break;
                    } else {
                        switch (tokens.at(j).type) {
                            case TokenType::variable:
                                output << tokens.at(j).value.value();
                                i++;
                            break;
                            
                            case TokenType::number:
                                output << " " + tokens.at(j).value.value();
                                i++;
                            break;
                            
                            case TokenType::add:
                                output << " + ";
                                i++;
                            break;

                            case TokenType::sub:
                                output << " - ";
                                i++;
                            break;

                            case TokenType::multi:
                                output << " * ";
                                i++;
                            break;

                            case TokenType::div:
                                output << " / ";
                                i++;
                            break;

                            default:
                                break;
                        }
                    }
                }
                output << " << std::endl;";
            break;
            
            case TokenType::semi:
                output << ";\n";
            break;

            default:
                break;
        }
    }

    output << "return 0;";
    // Main end bracket
    output << "}";

    return output.str();
}

int main(int argc ,char* argv[]) {

    if(argc != 2){
        std::cerr << "Incorrect usage!" << std::endl;
        std::cerr << "Correct usage is math filename.mh" << std::endl;
        return EXIT_FAILURE;
    }

    std::string ext = argv[1];
    if(ext.substr(ext.find_last_of(".") + 1) != "mh"){
        std::cerr << "Incorrect file extension!" << std::endl;
        std::cerr << "Extension should be .mh and you used " << "." << ext.substr(ext.find_last_of(".") + 1) << std::endl;
        return EXIT_FAILURE;
    }
    
    // TODO: Error check for correct file type

    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in); // Use file as an input
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }
    
    std::vector<Token> tokens = tokenize(contents);

    { // Automatic destructor ?
        std::fstream file("out.cpp", std::ios::out);
        file << assembler(tokens);
    }

    // Compile with g++
    system("g++ -c out.cpp");
    system("g++ out.o -o out");
    
    if(DEBUG == false){
        system("rm out.o && rm out.cpp");
    }
    // Run the file
    system("out");

    return EXIT_SUCCESS;
}