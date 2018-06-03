#include <iostream>
#include <string>
#include <vector>

enum class Kind
{
Push, Pop, Number
};

struct Token
{
Kind kind;
int value;
};

class Scanner
{
public:
Scanner(std::istream& inputStream) : stream_{inputStream} {};

bool fetch(Token& t)
{
std::string token;

if (!(stream_ >> token))
return false;

if (token == "+")
{
t.kind = Kind::Push;
}
else if (token == "-")
{
t.kind = Kind::Pop;
}
else
{
t.kind = Kind::Number;
t.value = std::atoi(token.c_str());
}

return true;
}
private:
std::istream& stream_;
};

class Stack
{
public:
Stack(std::size_t maxSize) : maxSize_{maxSize} {};

bool isEmpty() const { return index_ == 0; }
bool isFull() const { return index_ == maxSize_; }
void push(int value) { items_[index_++] = value; }
int pop() { return items_[--index_]; }

private:
std::size_t maxSize_;
std::size_t index_ = 0;
std::vector<int> items_;
};

int main()
{
Scanner scanner{std::cin};
Stack stack{10};

Token token;
while (scanner.fetch(token))
{
switch(token.kind)
{
case Kind::Pop:
if (stack.isEmpty())
{
std::cout << ":(\n";
}
else
{
std::cout << stack.pop() << "\n";
}
break;
case Kind::Number:
if (stack.isFull())
{
std::cout << ":(\n";
}
else
{
stack.push(token.value);
std::cout << ":)\n";
}

break;
}
}

return 0;
}
