#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

// 연산자의 우선순위를 리턴해준다. 번호가 클수록 우선순위가 높다. 
int priority(int op)
{
	switch (op)
	{
	case '(' : 
	case '{' :
	case '[' : return 0;

	case '+':
	case '-': return 1;

	case '*':
	case '/': return 2;


	}
}

// 두 연산자를 비교해서 앞쪽이 크거나 같으면 참을 리턴 
bool preOp(char op1, char op2)
{
	return priority(op1) >= priority(op2);
}


// 연산자를 Stack에 push하기 전 Stack 안에 있는 연산자들과 비교하여
// 우선순위가 높거나 같은 게 있으면 Pop해서 버퍼에 출력시키고 나서 
// 현재 새로운 연산자를 Stack에 push 
void checkS(stack<char>* sc, vector<char>& v, char c)
{
	// Stack이 비어있지 않으면, Stack 안의 연산자가 크거나 같으면 Pop한다.  
	while (!sc->empty() && preOp(sc->top(), c))
	{
		v.push_back(sc->top()); sc->pop();
		v.push_back(' ');
	}
	sc->push(c);
}

//닫힌 괄호일 때 Stack안에 있는 연산자들을 열린 괄호가 나올 때까지 Pop한다.
// 이 때 Stack이 비었다면 오류이다. 
void checkP1(stack<char>* sc, vector<char>& v)
{
	char c;

	for (c = sc->top(), sc->pop(); c != '{'; c = sc->top(), sc->pop())
	{
		v.push_back(' ');
		v.push_back(c);
	}
}
void checkP2(stack<char>* sc, vector<char>& v)
{
	char c;

	for (c = sc->top(), sc->pop(); c != '['; c = sc->top(), sc->pop())
	{
		v.push_back(' ');
		v.push_back(c);
	}
}
void checkP3(stack<char>* sc, vector<char>& v)
{
	char c;

	for (c = sc->top(), sc->pop(); c != '('; c = sc->top(), sc->pop())
	{
		v.push_back(' ');
		v.push_back(c);
	}
}



bool InfixToPostfix(vector<char>& Postfix, vector<char>& Infix)
{
	const char* op = "+-*/";

	stack<char> Stack;
	stack<char>* sc = &Stack;
	vector<char>* p = &Postfix;
	vector<char>::iterator n = Infix.begin();

	int  count = 1;//연산자가 하나만 있을때를 구분하기 위해 처음 시작이 1이다
	int  parN1 = 0;
	int  parN2 = 0;
	int  parN3 = 0;

	while (n != Infix.end())
	{
		if (isdigit(*n) || *n == '.') {
			p->push_back(*n++); //숫자이면 
			count = 0;
			continue;//숫자가 여러 자리 일 수 있으니 숫자가 끝날 때까지 연속으로 숫자를 읽는다.
		}
		else if (strchr(op, *n)) { //만약 연산자이면
			if (++count == 2) { //count를 1증가시키고 2라면 
				if (*n == '+' || *n == '-') { // 그 부호가 +인지 -인지 확인 
					p->push_back(' '); //참이라면 
				}
				else return false;
			}
			else if (count == 3) return false;
			p->push_back(' ');
			checkS(sc, Postfix, *n);
		}
		//숫자나 부호가 아니라면 아래를 진행
		switch (*n)
		{
		case '{':
			
			sc->push(*n);//열린괄호를 스택에 넣어주고
			++parN1; //parN1를 1증가
			break;

		case '}':
			checkP1(sc, Postfix); //checkP1확인
			--parN1; // parN1를1감소
			break;
		
		case '[':

			sc->push(*n);
			++parN2;
			break;

		case ']':
			checkP2(sc, Postfix);
			--parN2;
			break;
		
		case '(':

			sc->push(*n);
			++parN3;
			break;

		case ')':
			checkP3(sc, Postfix);
			--parN3;
			break;
		}

		if (parN1 < 0) {//loop를 한 바퀴 도는 동안 닫힌 괄호가 0보다 작다면 수식이 잘못된 것이므로 false를 리턴하고 함수를 종료한다.
			cout << "Error!: unbalanced parentheses" << endl;
			return false;
		}
		if (parN2 < 0) {
			cout << "Error!: unbalanced parentheses" << endl;
			return false;
		}
		if (parN3 < 0) {
			cout << "Error!: unbalanced parentheses" << endl;
			return false;
		}
		n++;
	}
	

	while (!sc->empty())
	{
		Postfix.push_back(' ');
		Postfix.push_back(sc->top()); sc->pop();
	}

	return true;
}

double value(stack<double>* sd)
{
	double d = sd->top(); sd->pop();
	return d;
}



double cal(vector<char>& Postfix)
{
	const char* op = "+-*/";

	vector<char>::iterator p = Postfix.begin();
	stack<double>  Stack;
	stack<double>* sd = &Stack;

	double n1, n2;
	char   cNumber[32];
	char*  c = cNumber;

	while (p != Postfix.end())
	{
		if (isdigit(*p) || *p == '.') {
			*c++ = *p++;
			continue;
		}


		else if (isdigit(cNumber[0])) {
			*c = ' ';
			sd->push(atof(cNumber)); //버퍼로부터 읽어서 숫자는 atof() 함수로 더블형으로 바꾸어 스택에 넣는다.
			c = cNumber; //atof() 함수가 공백을 만날 때까지 읽어서 그 안의 숫자를 double형으로 바꿔주기 때문에 읽고(pd->Push(atof(p));),
			cNumber[0] = ' ';
		}
		//문자로 된 숫자들을 스택에 넣다가 연산자를 만나면
		//스택으로부터 먼저 가져온 게 n1이라 했으니 n1 먼저 대입을 받고,
		//다음으로 n2을 대입 받는다.아주 중요하다. + 나 * 는 상관없지만 - 나 / 는 n2과 n1이 바뀌면 값이 달라지기 때문이다.
		else if (strchr(op, *p)) { //스택으로부터 두 개의 숫자를 꺼내서 먼저 꺼낸 건 n1 나중에 꺼낸 건 n2으로 해서 계산을 한다.

			n1 = value(sd);
			n2 = value(sd);
			if (n1 == 0 && *p == '/') {
				cout << "Error!:divide by zero" << endl;
			}
			switch (*p)
			{
			case '+': sd->push(n2 + n1); break;
			case '-': sd->push(n2 - n1); break;
			case '*': sd->push(n2 * n1); break;
			case '/':sd->push(n2 / n1);break;
			}
		}
		p++; 
	}
	return value(sd);
}



// 중위식을 후위식으로 바꾸기 전 사전검사를 수행한다. 
bool funCheck(vector<char>& Infix)
{
	vector<char>::iterator p = Infix.begin();

	int n1 = 0;
	int n2 = 0;
	int n3 = 0;

	while (p != Infix.end())
	{
		if (*p == '{') {
			n1++;
		}
		else if (*p == '}') { // 열린 괄호와 닫힌 괄호를 카운트 한다.  
			n1--;
		}
		else if (*p == '[') {
			n2++;
		}
		else if (*p == ']') {
			n2--;
		}
		else if (*p == '(') {
			n3++;
		}
		else if (*p == ')') { 
			n3--;
		}
		p++;
	}
	if (n1 != 0) {
		cout << "Error!: unbalanced parentheses" << endl;
		return false;
	}
	if (n2 != 0) {
		cout << "Error!: unbalanced parentheses" << endl;
		return false;
	}
	if (n3 != 0) {
		cout << "Error!: unbalanced parentheses" << endl;
		return false;
	}// 수식을 다 읽었는데 괄호 카운트가 0이 아니라면 수식 오류다. 
	return true;
}

void result(vector<char>& Infix)
{
	vector<char> buffer;
	vector<char>::iterator it;

	if (funCheck(Infix)) {
		if (InfixToPostfix(buffer, Infix)) {
			cout << cal(buffer)<<endl;
		}
	}
	buffer.erase(buffer.begin(), buffer.end());
}

void getInfix(vector<char>& vc)
{
	int ENTER = 10;
	int BACK_SPACE = 8;

	for (int ch = getc(stdin); ch != ENTER; ch = getc(stdin))
	{
		vc.push_back(ch);
		if (ch == BACK_SPACE)
			vc.pop_back();
	}
}

int main()
{
	vector<char> vc;
	cout << fixed; //소수점 셋째자리까지 나옴
	cout.precision(3);

	while (cin.peek()!= 'E') 
	{
		getInfix(vc);
		result(vc);
		vc.erase(vc.begin(), vc.end());
	}
	return 0;
}

