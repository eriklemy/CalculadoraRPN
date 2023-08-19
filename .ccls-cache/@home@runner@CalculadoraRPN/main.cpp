/** Copyright 2023
 * Linguagens Formais e Compiladores - LFC
 * ALUNO: Erick Lemmy dos Santos Oliveira
 * PROF: Frank Alcantara
 * GIT: https://github.com/eriklemy/CalculadoraRPN
// --------------------------------------------------------------------------------------------------------
ENUNCIADO:
	Seu objetivo será desenvolver um programa, no ambiente online Repl.it, usando Python, C, ou C++, capaz 
 	de abrir um arquivo de texto, contendo expressões aritméticas simples, com uma expressão aritmética por
  	linha, escritas em notação RPN, segundo o formato a seguir:
	a) Adição:+, no formato (+ A B);
	b) Subtração: - no formato (- A B) ;
	c) Multiplicação: * no formato (* A B);
	d) Divisão Real: | no formato (| A B);
	e) Divisão de inteiros: / no formato (/ A B);
	f) Resto da Divisão de Inteiros: % no formato (% A B);
	g) Potenciação:^ no formato (^ A B);
	h) Radiciação (Raiz Quadrada) (? A);
	i) Inversão de Sinal (@A).
	Onde A e B representam números reais, ou inteiros, e não variáveis. Use o ponto, (.), para indicar a 
 	vírgula decimal. O formato definido entre as alíneas a e i define as operações binárias que poderão 
  	estar nas linhas do arquivo de texto que seu programa lerá.
	Todas as operações serão executadas sobre números reais de precisão dupla. As únicas exceções são a 
 	divisão de inteiros e a operação Resto da Divisão de Inteiros. Além disso, O expoente da operação de 
   	potenciação será sempre um inteiro positivo.
	As expressões indicadas nas alíneas a até i podem ser aninhadas para a criação de novas expressões. 
 	Isto implica na possibilidade de que o texto lido contenha linhas com expressões como as apresentadas 
  	a seguir:
	j) (+ A (* C D))
	k) (/ (% A B) (* D (@ E)))
	Onde A, B, C, D e E representam números reais, ou inteiros, e não variáveis. Use o ponto, ., para
	indicar a vírgula decimal. Além das expressões existem três comandos especiais (N RES), (V MEM) e
	(MEM), de tal forma que:
	a) (N RES): devolve o resultado da expressão que está N linhas antes, onde N é um número
	inteiro não negativo;
	b) (V MEM): armazena um valor, V, em um espaço de memória chamado de MEM, capaz de
	armazenar um valor real;
	c) (MEM): devolve o valor armazenado na memória. Se a memória não tiver sido usada
	devolve o número real zero.
// --------------------------------------------------------------------------------------------------------
**/

/* 
FONTES:
https://wagnergaspar.com/como-resolver-expressao-em-notacao-pos-fixa-notacao-polonesa-reversa/
https://pt.stackoverflow.com/questions/490417/criando-uma-calculadora-por-rota%C3%A7%C3%A3o-polonesa-inversa-em-c
https://hpmuseum.org/rpn.htm
*/

#include <cctype>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <vector>

#define DEBUG_TOKENS(tokens) \
    do { \
        std::cout << "Tokens: "; \
        for (const std::string &tok : tokens) { \
            std::cout << tok << " "; \
        } \
        std::cout << std::endl; \
    } while (false)


struct Memory {
	bool hasValue = false;
	double value = 0.0;
};

bool isOperator(char c) {
  	return c == '+' || c == '-' || c == '*' || c == '|' || c == '/' || c == '%' ||
		   c == '^' || c == '?' || c == '@';
}

bool isOpeningParenthesis(char c) { return c == '('; }
bool isClosingParenthesis(char c) { return c == ')'; }
bool isNumber(char c) { return std::isdigit(c) || c == '.'; }
double applyOperator(char op, double a, double b);
double stof_with_sign(const std::string &str);
std::vector<std::string> splitTokens(const std::string &line);
double evaluateExpression(const std::vector<std::string> &tokens);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "ERRO: É NECESSARIO FORNECER COMO PARAMETRO O NOME DO ARQUIVO COM AS EXPRESSOES!!\n";
        std::cerr << "USO: " << argv[0] << " teste1.txt\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        std::cout << "Error opening file." << std::endl;
        return 1;
    }

    Memory MEM;
    std::vector<double> resultMemory; // Armazenamento dos resultados
    std::string line;
    int lineNumber = 1;

    while (getline(inputFile, line))
    {
        std::stack<std::string> expressionStack;
        std::vector<std::string> tokens = splitTokens(line);

        DEBUG_TOKENS(tokens);

        for (size_t i = 0; i < tokens.size(); ++i)
        {
            const std::string &tok = tokens[i];
            if (isOpeningParenthesis(tok[0]))
            {
                expressionStack.push(tok);
            }
            else if (isClosingParenthesis(tok[0]))
            {
                std::vector<std::string> subExpression;
                while (!expressionStack.empty() && !isOpeningParenthesis(expressionStack.top()[0]))
                {   
                    subExpression.push_back(expressionStack.top());
                    expressionStack.pop();
                }
            
                if (!expressionStack.empty())
                {
                    expressionStack.pop(); // Remove "("
                }
                double result = evaluateExpression(subExpression);
                expressionStack.push(std::to_string(result));
            }
            else if (isNumber(tok[0]) || (tok[0] == '-' && isNumber(tok[1])))
            {
                expressionStack.push(tok);
            }
            else if (isOperator(tok[0]))
            {
                expressionStack.push(tok);
            }
            else
            {
                const std::string &token = tokens[i - 1];
                if (tok == "R" && i + 2 < tokens.size() && tokens[i + 1] == "E" && tokens[i + 2] == "S")
                {
                    if (expressionStack.size() < 1)
                    {
                        throw std::runtime_error("Invalid expression: not enough operands");
                    }
                    int n = static_cast<int>(stof_with_sign(tokens[i - 1]));
                    if (n <= 0 || n > resultMemory.size())
                    {
                        throw std::runtime_error("Invalid expression: invalid line number");
                    }
                    expressionStack.pop(); // Remove ")"
                    expressionStack.push({std::to_string(resultMemory[lineNumber - n - 1])});
                    i += 2;
                }
                else if (tok == "M" && i + 2 < tokens.size() &&
                        tokens[i + 1] == "E" && tokens[i + 2] == "M" &&
                        !isOperator(token[0]))
                {
                    double v = stof_with_sign(std::to_string(resultMemory[lineNumber - 2]));
                    MEM.value = v; // Armazenamento em memória
                    MEM.hasValue = true;
                    i += 2;
                }
                else if (tok == "M" && i + 2 < tokens.size() && tokens[i + 1] == "E" && tokens[i + 2] == "M" && isOperator(token[0]))
                {
                    if (MEM.hasValue)
                    {
                        expressionStack.push({std::to_string(MEM.value)});
                    }
                    else
                    {
                        expressionStack.push({"0.0"});
                    }
                }
            }
        }

        if (!expressionStack.empty())
        {
            double result = evaluateExpression(std::vector<std::string>{expressionStack.top()});
            std::ostringstream formattedResult;
            formattedResult << std::fixed << std::setprecision(1) << result;
            std::cout << "L" << lineNumber << ": " << formattedResult.str()
                        << std::endl;

            resultMemory.push_back(result);
            MEM.hasValue = true;
        }
        lineNumber++;
    }

    inputFile.close();
    return 0;
}

double applyOperator(char op, double a, double b) {
	switch (op) {
		case '+': return a + b;
		case '-': return a - b;
		case '*': return a * b;
		case '|': return a / b;
		case '/': return static_cast<double>(a / b);
		case '%': return static_cast<int>(a) % static_cast<int>(b);
		case '^': return std::pow(a, b);
		case '?': return std::sqrt(a);
		case '@': return -a;
		default: throw std::runtime_error("Invalid operator");
  	}
}

double stof_with_sign(const std::string &str) {
	size_t pos = 0;
	double number = std::stod(str, &pos);
	if (pos == 0) {
		throw std::invalid_argument("Invalid number format: " + str);
	}
	return number;
}

std::vector<std::string> splitTokens(const std::string &line) {
	std::vector<std::string> tokens;
	std::string token;
	for (char c : line) {
		if (isspace(c)) {
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
		} else if (isOpeningParenthesis(c) || isClosingParenthesis(c) || isOperator(c)) {
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
		  	}
		  	tokens.push_back(std::string(1, c));
		} else {
			token += c;
		  	if (token == "." || isNumber(c)) {
				continue;
		  	} else {
				tokens.push_back(token);
				token.clear();
			}
		}
	}
	
	if (!token.empty()) {
		tokens.push_back(token);
	}
	
	return tokens;
}

double evaluateExpression(const std::vector<std::string> &tokens) {
	std::stack<double> values;

	for (const std::string &token : tokens) {
    	if (isNumber(token[0]) || (token[0] == '-' && isNumber(token[1]))) {
      		values.push(stof_with_sign(token));
    	} else if (isOperator(token[0])) {
	    	if (token[0] == '?' || token[0] == '@') {
	        	if (values.empty()) {
	          		throw std::runtime_error("Invalid expression: not enough operands");
	        	}
		        double a = values.top();
		        values.pop();
		        values.push(applyOperator(token[0], a, 0));
	      	} else {
				if (values.size() < 2) {
		  			throw std::runtime_error("Invalid expression: not enough operands");
	        	}
		        double b = values.top();
		        values.pop();
		        double a = values.top();
		        values.pop();
		        values.push(applyOperator(token[0], b, a));
			}
		}
	}

  	if (values.size() != 1) {
    	throw std::runtime_error("Invalid expression: too many operands");
  	}

  	return values.top();
}