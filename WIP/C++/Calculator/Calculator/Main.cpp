#include <iostream>
#include <string>
#include <iostream>

using namespace std;

string input;
float answer = 0;

float string2float(string num_str);

float extract(float firstNum);

void updateInput(int size);

float trigFloat(int nameLength);

int main()
{
	//get first input and adds " 0+" to the beginning to prevent errors when starting with a bracket
	getline(cin, input);
	input = " 0+" + input;

	//calculates answer
	float firstNum = string2float(input);
	answer = extract(firstNum);
	cout << answer;
	
	//continue next input with the last answer as the first number
	bool cleared = false;
	while (true)
	{
		getline(cin, input);

		//allows for clearing answer with an empty line
		if (input.length() == 0)
		{
			cleared = true;
		}
		else
		{
			if (cleared)
			{
				cleared = false;
				input = "+" + input;
				answer = extract(0);
			}
			else
			{
				answer = extract(answer);
			}
			cout << answer;
		}
	}
}

float string2float(string num_str)
{
	if (num_str[0] == '(')
	{
		return(NAN);
	}
	else if (num_str[0] == 'a')
	{
		if (num_str[1] == 's')
		{
			return(asin(trigFloat(4)));
		}
		else if (num_str[1] == 'c')
		{
			return(acos(trigFloat(4)));
		}
		else if (num_str[1] == 't')
		{
			return(atan(trigFloat(4)));
		}
		else if (num_str[1] == 'n')
		{
			updateInput(3);
			return(answer);
		}
	}
	else if (num_str[0] == 's')
	{
		return(sin(trigFloat(3)));
	}
	else if (num_str[0] == 'c')
	{
		return(cos(trigFloat(3)));
	}
	else if (num_str[0] == 't')
	{
		return(tan(trigFloat(3)));
	}
	else if (num_str[0] == 'p')
	{
		updateInput(2);
		return(3.1415927410125732421875);
	}

	int baseCounter = 1;
	float num_float = 0;
	string currentNum = "";

	//store the number as a seperate string so it can be calculated from the end
	for (int i = 0; num_str[i] > 47 && num_str[i] < 58 || num_str[i] == 46; i++)
	{
		currentNum += num_str[i];
	}

	//removes the processed number from the input
	updateInput(currentNum.length());

	//converts currentNum to a float
	for (int i = currentNum.length(); i > 0; i--)
	{
		if (num_str[i - 1] != '.')
		{
			num_float += (currentNum[i - 1] - 48) * baseCounter;
			baseCounter *= 10;
		}
		else
		{
			baseCounter = 1;
			num_float /= powf(10, currentNum.length() - i);
		}
	}

	return(num_float);
}

float extract(float firstNum)
{
	float output{};

	//gets the function, stores it and removes it from the input
	char function = input[0];
	updateInput(1);

	//returns early if its the end of a bracket
	if (function == ')') return(firstNum);

	//gets the next number after the function
	float secondNum = string2float(input);

	//error correction for if there is a bracket after a function
	if (isnan(secondNum))
	{
		updateInput(1);
		secondNum = extract(string2float(input));
	}

	//depending on the function works out the output
	     if (function == '+')	output =	 firstNum + secondNum;
	else if (function == '-')	output =	 firstNum - secondNum;
	else if (function == '*')	output =	 firstNum * secondNum;
	else if (function == '/')	output =     firstNum / secondNum;
	else if (function == '^')	output = pow(firstNum , secondNum);
	else if (function == '(')	output = firstNum * extract(secondNum);

	//continue if theres more or return if its the end
	//cout << firstNum << function << secondNum << "=" << output << endl;
	return((input.length() > 0)? extract(output) : output );
}

void updateInput(int start)
{
	input = input.substr(start, input.length());
}

float trigFloat(int nameLength)
{
	updateInput(nameLength);
	float tempNum = string2float(input);
	if (isnan(tempNum))
	{
		updateInput(1);
		tempNum = extract(string2float(input));
	}
	return(tempNum);
}