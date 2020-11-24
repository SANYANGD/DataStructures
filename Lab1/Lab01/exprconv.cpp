#include "exprconv.h"

#include <QDebug>

std::string readAsNumber(std::string str, int& index)
{
    std::string number = "";
    bool dot = false;
    for (; index < (int)str.length(); ++index)
    {
        if (str[index] >= '0' && str[index] <= '9')
        {
            number += str[index];
        }
        else if (str[index] == '.')
        {
            if (!dot)
            {
                number += str[index];
                dot = true;
            }
            else
            {
                throw QString("Invalid Number Format");
            }
        }
        else
        {
            break;
        }
    }

    return number;
}

std::string getNextToken(std::string& str)
{
    static bool lastIsOperator = false;
    std::string tk;

    if (str[0] >= '0' && str[0] <= '9')
    {
        int x = 0;
        std::string number = readAsNumber(str, x);
        str.erase(str.begin(), str.begin()+x);
        return number;
    }

    switch (str[0])
    {
    case '-':
        if (lastIsOperator)
        {
            std::string number = "-";
            str.erase(str.begin());
            int i = 0;
            number += readAsNumber(str, i);
            str.erase(str.begin(), str.begin()+i);
            return number;
        }
    case '+':
    case '*':
    case '/':
    case '(':
    case ')':
        lastIsOperator = true;
        std::string temp = str.substr(0,1);
        str.erase(str.begin());
        return temp;
    }
    throw QString("ERROR");
}

ExprConv::ExprConv()
{

}

Stack<Token*>* ExprConv::toSuffix(std::string str)
{
    Stack<Token*> temp;                       //用于存储运算符的临时栈
    Stack<Token*> *back = new Stack<Token*>(); // 用于储存后缀表达式

    while (!str.empty())
    {
        Token *tk = new Token(getNextToken(str));
        if (tk->isBracket())
        {
            // 处理括号
            if (tk->toToken() == '(')
            {
                // ( 直接入栈
                temp.push(tk);
            }
            else if (tk->toToken() == ')')
            {
                // 出栈到 (
                while (!temp.empty() && temp.top()->toToken() != '(')
                {
                    back->push(temp.pop());
                }
            }
        }
        else if (tk->isOperator())
        {
            if (temp.empty() || temp.top()->toToken() == '(' ||
                    (temp.top()->toToken() != '(' && level(tk->toToken() > level(tk->toToken()))))
            {
                // 如果是运算符，并且栈顶优先级高，则直接进栈
                temp.push(tk);
            }
            else
            {
                // 否则出栈直到遇到低优先级运算符
                while (!temp.empty() && level(tk->toToken()) < level(temp.top()->toToken()))
                {
                    back->push(temp.pop());
                }
                temp.push(tk);
            }
        }
        else if (tk->isNumber())
        {
            // 如果是数字, 直接进栈
            back->push(tk);
        }
    }

    // 将剩余运算符出栈
    while (!temp.empty())
    {
        back->push(temp.pop());
    }

    return back;
}

double ExprConv::calculateSuffix(Stack<Token*>* back)
{
    double tempNum, num1, num2;
    Stack<double> tempStack; //用于存储数字的临时栈

    back->reverse();

    while (!back->empty())
    {
        auto token = back->pop();
        if (token->isNumber())
        {
            // 数字直接入栈
            tempNum = token->toNumber();
            tempStack.push(tempNum);
        }
        else if (token->isOperator())
        {
            // 遇到运算符就取出两个数字
            num1 = tempStack.pop();
            num2 = tempStack.pop();

            switch (token->toToken())
            {
            case '+':
                tempNum = num1 + num2;
                tempStack.push(tempNum);
                break;
            case '-':
                tempNum = num1 - num2;
                tempStack.push(tempNum);
                break;
            case '*':
                tempNum = num1 * num2;
                tempStack.push(tempNum);
                break;
            case '/':
                tempNum = num1 / num2;
                tempStack.push(tempNum);
                break;
            }
        }
        qDebug() << "";
    }

    delete back;
    return tempStack.pop();
}

int ExprConv::level(char ch)
{
    switch (ch)
    {
    case '+':
        return 1;
    case '-':
        return 1;
    case '*':
        return 2;
    case '/':
        return 2;
    default:
        return 0;
    }
}

Token::Token(std::string std)
{
    data = QString::fromStdString(std);
    m_number = data.toDouble(&m_isNumber);
    m_isOperator = data[0] == '+' || data[0] == '-' || data[0] == '*' || data[0] == '/';
    m_isBracket = data[0] =='(' || data[0] == ')';
}

double Token::toNumber()
{
    return m_isNumber ? data.toDouble() : 0.0f;
}

char Token::toToken()
{
    return m_isOperator || m_isBracket ? data.toStdString()[0] : ' ';
}
