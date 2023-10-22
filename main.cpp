#include <bits/stdc++.h>
using namespace  std;

const string FILE_NAME = "js_code.txt";

const string operators = "+-*/%=<>{}[]()&|?!:,;^";
vector<string> keywords = {
        "abstract", "arguments", "await", "boolean", "break", "byte", "case", "catch",
        "char", "class", "const", "continue", "debugger", "default", "delete", "do",
        "double", "else", "enum", "eval", "export", "extends", "false", "final",
        "finally", "float", "for", "function", "goto", "if", "implements", "import",
        "in", "instanceof", "int", "interface", "let", "long", "native", "new",
        "null", "package", "private", "protected", "public", "return", "short", "static",
        "super", "switch", "synchronized", "this", "throw", "throws", "transient", "true",
        "try", "typeof", "var", "void", "volatile", "while", "with", "yield" };

struct Lexeme
{
    string type;
    string value;
};

bool IsDec(string token)
{
    regex decRegex(R"(-?\d+(\.\d*)?([eE][-+]?\d+)?)");
    return regex_match(token, decRegex);
}

bool IsFloat(string token)
{
    regex floatRegex("[-+]?[0-9]*\\.[0-9]+([eE][-+]?[0-9]+)?");
    return regex_match(token, floatRegex);
}

bool IsHex(string token)
{
    regex hexRegex("0[xX][0-9a-fA-F]+");
    return regex_match(token, hexRegex);
}

bool IsIdentifier(string token)
{
    regex identifierRegex(R"(\b[a-zA-Z_$][a-zA-Z0-9_$]*(\.[a-zA-Z_$][a-zA-Z0-9_$]*)*\b)");
    return regex_match(token, identifierRegex);
}

bool IsStringConstant(string token) {
    regex strConstRegex(R"("(\\"|[^"])*")");
    return regex_match(token, strConstRegex);
}

bool IsCharConstant(string token) {
    regex charConstRegex(R"('(?:\\.|[^\\'])')");
    return regex_match(token, charConstRegex);
}

void ProcessJSFile(const string fileName, vector<Lexeme> &LexemeList)
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        cerr << "Failed to open file." << endl;
        return;
    }

    bool inStrConst = false;
    string bufferToken = "";
    string line;
    while(getline(file, line))
    {
        size_t commentPos = line.find("//");
        if (commentPos != string::npos)
        {
            string comment = line.substr(commentPos);
            line.erase(commentPos);                             
            line = regex_replace(line, regex(R"(\s+$)"), "");  
            LexemeList.push_back({"comment", comment});         
        }


        stringstream lineStream(line);
        string token;
        while (lineStream >> token)
        {
            for (int pos=0; ; pos++)
            {
                if (find(operators.begin(), operators.end(), token[pos]) != operators.end() || pos == token.size())
                {

                    if (find(keywords.begin(), keywords.end(), bufferToken) != keywords.end())
                    {
                        LexemeList.push_back({"keyword", bufferToken});
                        bufferToken.clear();
                    }
                    else if (IsHex(bufferToken))
                    {
                        LexemeList.push_back({"hexadecimal number", bufferToken});
                        bufferToken.clear();
                    }
                    else if (IsFloat(bufferToken))
                    {
                        LexemeList.push_back({"floating-point number", bufferToken});
                        bufferToken.clear();
                    }
                    else if (IsDec(bufferToken))
                    {
                        LexemeList.push_back({"decimal number", bufferToken});
                        bufferToken.clear();
                    }
                    else if (IsIdentifier(bufferToken))
                    {
                        LexemeList.push_back({"identifier", bufferToken});
                        bufferToken.clear();
                    }
                    else if (IsStringConstant(bufferToken))
                    {
                        LexemeList.push_back({"string constant", bufferToken});
                        bufferToken.clear();
                    }
                    else if (IsCharConstant(bufferToken))
                    {
                        LexemeList.push_back({"char constant", bufferToken});
                        bufferToken.clear();
                    }

                    if (pos != token.size())
                    {
                        Lexeme l;
                        l.value = token[pos];
                        l.type = "operator";
                        LexemeList.push_back(l);
                        bufferToken.clear();
                    }
                }
                else bufferToken += token[pos];

                if (pos == token.size()) break;
            }
        }
    }

    file.close();
}

int main()
{
    vector<Lexeme> JSLexems;

    ProcessJSFile(FILE_NAME, JSLexems);
    for (auto l : JSLexems)
    {
        cout << "<" << l.value << ", " << l.type << ">\n";
    }

    return 0;
}
