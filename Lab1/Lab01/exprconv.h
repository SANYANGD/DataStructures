#ifndef EXPRCONV_H
#define EXPRCONV_H

#include <QObject>

template <class T>
class Stack {
  private:
    T       elems[1024];// 元素
    size_t  m_top;

  public:
    Stack() : m_top(0) { /*elems = new T[1024];*/ }
    ~Stack() { /*delete elems;*/ }

    inline void push(T const& v) { elems[m_top++] = v; }

    inline T    pop()            { if (m_top > 0) return elems[--m_top]; else throw QString("NullPointer"); }

    inline T    top()   const    { if (m_top > 0) return elems[m_top-1]; else throw QString("NullPointer"); }

    inline bool empty() const    { return m_top == 0; }

    inline T&   at(int index)    {
        if (index < m_top && index >= 0) {
            return elems[index];
        } else {
            throw QString("NullPointer");
        }
    }

    inline size_t size() const   { return m_top; }

    inline void reverse() {
        for (size_t i = 0; i < m_top / 2; ++i) {
            T temp = elems[i];
            elems[i] = elems[m_top-i-1];
            elems[m_top-i-1] = temp;
        }
    }
};


class Token
{
public:
    Token(std::string token);

    inline bool isOperator() const { return m_isOperator; }
    inline bool isBracket()  const { return m_isBracket; }
    inline bool isNumber()   const { return m_isNumber; }

    char    toToken();
    double  toNumber();
    inline QString getData() { return data; }

private:
    bool m_isNumber;
    bool m_isOperator;
    bool m_isBracket;
    double m_number;
    QString data;
};


// Expression Convertor
class ExprConv
{
public:
    ExprConv();
    ~ExprConv();

public:
    static Stack<Token*>* toSuffix(std::string str);
    static double         calculateSuffix(Stack<Token*>* back);

private:
    static int level(char ch);
};

#endif // EXPRCONV_H
