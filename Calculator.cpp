#include "Calculator.hpp"

void Calculator::Run(int world_rank, int world_size)
{
	try
	{
		double start_time = 0;
		start_time = MPI_Wtime();
		
		Dialog();

		printf("MASTER >> TIME:%f\n", getTime(start_time));
	}
	catch (const exception &exc)
	{
		print(exc.what());
	}
	catch (...)
	{
		print("Error - default\n");
	}
}

void Calculator::Dialog()
{
	bool flag = true;
	while (flag)
	{
		update();
		print("Enter function");
		getline(cin, str);
		if (str == "exit")
		{
			flag = false;
			break;
		}
		str.append("\0");
		double start_time = MPI_Wtime();
		if (!parse() || str[place] != '\0')
		{
			print("try again");
		}
		else
			printf("Master >> Answer:%f, time:%f\n", answer, getTime(start_time));
	}
}

bool Calculator::parse()
{
	try
	{
		answer = GetP();
		return true;
	}
	catch (const SystemException &exc)
	{
		check();
		print(exc.what());
		return false;
	}
}

double Calculator::GetP()
{
	if (str[place] == '(')
	{
		place++;
		double val = GetE();
		if (str[place] == ')')
		{
			place++;
			skip();
			return val;
		}
		else
			throw SystemException("Error - GetP - ()");
	}
	else
		return GetE();
}

double Calculator::GetE()
{
	double val = GetT();
	if (str[place] == '+' || str[place] == '-')
	{
		double op = str[place];
		place++;
		double val2 = GetE();
		if (op == '+')
			val += val2;
		else
			val -= val2;
	}
	return val;
}

double Calculator::GetT()
{
	double val = GetD();
	if (str[place] == '*' || str[place] == '/')
	{
		double op = str[place];
		place++;
		double val2 = GetT();
		if (op == '*')
		{
			val *= val2;
		}
		else
		{
			if (val2 != 0)
			{
				val /= val2;
			}
			else
				throw SystemException("Error - div 0");
		}
	}
	return val;
}

double Calculator::GetD()
{
	double val = GetN();
	if (str[place] == '^')
	{
		place++;
		skip();
		double val2 = 0;
		if (str[place] == '(')
		{
			place++;
			val2 = GetE();
			if (str[place] == ')')
			{
				place++;
				return pow(val, val2);
			}
			else
				throw SystemException("Error - GetD - ()");
		}
		else
		{
			val2 = GetN();
			return pow(val, val2);
		}
	}
	else
		return val;
}

double Calculator::GetN()
{
	double val = NAN;
	double point = 1;
	bool flag_sign = false;
	bool flag_read = false;
	bool flag = false;
	skip();
	if (str[place] == '(')
	{
		return GetP();
	}
	else if ('a' <= str[place] && str[place] <= 'z')
	{
		val = GetI();
		skip();
		return val;
	}
	else
	{
		if (str[place] == '-')
		{
			flag_sign = true;
			place++;
		}
		if (str[place] == '+')
		{
			place++;
		}
		while (!flag)
		{
			if ('0' <= str[place] && str[place] <= '9')
			{
				if (point == 1)
				{
					if (flag_read == false)
					{
						val = 0;
						flag_read = true;
					}
					val = val * 10 + str[place] - '0';
					place++;
				}
				else
				{
					val = val + (str[place] - '0') * point;
					point /= 10;
					place++;
				}
			}
			else if (str[place] == '.' && point == 1)
			{
				point /= 10;
				place++;
			}
			else if (str[place] == ' ' && flag_read == true)
			{
				place++;
				flag = true;
			}
			else
			{
				flag = true;
			}
		}
		if ((point == 1 || point < 0.2) && flag_read == true)
		{
			if (flag_sign == true)
			{
				return -1 * val;
			}
			else
				return val;
		}
		else
			throw SystemException("Error - GetN - wrong num");
	}
}

double Calculator::GetI()
{
	double val = NAN;
	if (str[place] == 'e')
	{
		place++;
		skip();
		return E;
	}
	if (str[place] == 'p' && str[place + 1] == 'i')
	{
		place += 2;
		skip();
		return PI;
	}
	if (str[place] == 'l' && str[place + 1] == 'n')
	{
		val = GetFunc(2);
		if (val < 0)
		{
			return log(val);
		}
		else
			throw SystemException("Error - GetI - < 0 - log");
	}
	else if (str[place] == 's' && str[place + 1] == 'i' && str[place + 2] == 'n')
	{
		val = GetFunc(3);
		return sin(val);
	}
	else if (str[place] == 'c' && str[place + 1] == 'o' && str[place + 2] == 's')
	{
		val = GetFunc(3);
		return cos(val);
	}
	else if (str[place] == 's' && str[place + 1] == 'q' && str[place + 2] == 'r' && str[place + 3] == 't')
	{
		val = GetFunc(4);
		if (val >= 0)
		{
			return sqrt(val);
		}
		else
			throw SystemException("Error - GetI - < 0 - sqrt");
	}
}

double Calculator::GetFunc(uint n)
{
	place += n;
	skip();
	if (str[place] == '(')
	{
		place++;
		double val = GetE();
		if (str[place] == ')')
		{
			place++;
			skip();
			return val;
		}
		else
			throw SystemException("Error - GetFunc - )");
	}
	else
		throw SystemException("Error - GetFunc - (");
}

inline void Calculator::skip()
{
	while (str[place] == ' ')
	{
		place++;
	}
}

inline void Calculator::update()
{
	place = 0;
	answer = 0;
	str = "";
}

inline void Calculator::check()
{
	printf("string:%s\nchar:%c\nplace:%d\n", str.c_str(), str[place], place);
}

inline double Calculator::getTime(double start)
{
	return MPI_Wtime() - start;
}