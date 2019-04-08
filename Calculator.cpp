#include "Calculator.hpp"

void Calculator::Run()
{
	world_rank == 0 ? master() : slave();
}

void Calculator::master()
{
	try
	{
		double start_time = 0;
		start_time = MPI_Wtime();

		init();

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
	halt();
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
		else if (str.substr(0, 4) == "DSYS")
		{
			changeDebugSys();
		}
		else if (str == "SUM" || str == "LIB")
		{
			changeSum();
		}
		else if (str.substr(0, 4) == "DSUM")
		{
			changeDebugSum();
		}
		else if (str.substr(0, 4) == "DALL")
		{
			changeDebugAll();
		}
		else
		{
			str.append("\0");
			double start_time = MPI_Wtime();
			if (!parse() || str[place] != '\0')
			{
				print("try again");
			}
			else
			{
				printf("Master >> Answer:%.16lf, time:%.16lf\n", answer, getTime(start_time));
			}
		}
	}
}

bool Calculator::parse()
{
	try
	{
		answer = GetP();
		return true;
	}
	catch (const ParseException &exc)
	{
		check();
		print(exc.what());
		return false;
	}
	catch (const SystemException &exc)
	{
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
		{
			throw ParseException("Error - GetP - ()");
		}
	}
	else
	{
		return GetE();
	}
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
			{
				throw ParseException("Error - div 0");
			}
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
			{
				throw ParseException("Error - GetD - ()");
			}
		}
		else
		{
			val2 = GetN();
			return pow(val, val2);
		}
	}
	else
	{
		return val;
	}
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
			{
				return val;
			}
		}
		else
		{
			throw ParseException("Error - GetN - wrong num");
		}
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
	else if (str.substr(0, 3) == "pi")
	{
		place += 2;
		skip();
		return PI;
	}
	else if (str.substr(0, 3) == "ln")
	{
		val = GetFunc(2);
		if (val < 0)
		{
			return log(val);
		}
		else
		{
			throw ParseException("Error - GetI - < 0 - log");
		}
	}
	else if (str.substr(0, 3) == "sin")
	{
		val = GetFunc(3);
		if (SUM)
		{
			return sinus(val);
		}
		else
		{
			return sin(val);
		}
	}
	else if (str.substr(0, 3) == "cos")
	{
		val = GetFunc(3);
		if (SUM)
		{
			cosinus(val);
		}
		else
		{
			return cos(val);
		}
	}
	else if (str.substr(0, 4) == "sqrt")
	{
		val = GetFunc(4);
		if (val >= 0)
		{
			return sqrt(val);
		}
		else
		{
			throw ParseException("Error - GetI - < 0 - sqrt");
		}
	}
	else
	{
		throw ParseException("Error - GetI - wrong func");
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
		{
			throw ParseException("Error - GetFunc - )");
		}
	}
	else
	{
		throw ParseException("Error - GetFunc - (");
	}
}

void Calculator::halt()
{
	if (world_rank != 0)
	{
		throw SystemException(__LINE__, __func__, "Error - wrong thread");
	}

	Message msg(HALT, HALT);
	send(msg, world_rank, world_size);
}

void Calculator::init()
{
	if (world_rank == 0)
	{
		Message msg(INIT, level);
		send(msg, world_rank, world_size);
	}
	factorial(level);
	if (DEBUG_FACT)
	{
		printFact();
	}

	if (level >= world_size)
	{
		int cell = level / (world_size);
		start = world_rank * cell;
		end = (world_rank == world_size - 1) ? (level) : ((world_rank + 1) * cell - 1);
		if (DEBUG_SYS)
		{
			printf("Cell >> start:%d end:%d to %d\n", start, end, world_rank);
		}
	}
	else
	{
		throw SystemException(__LINE__, __func__, "Error - level < world_size");
	}
}

void Calculator::slave()
{
	try
	{
		double start_time = 0;
		start_time = MPI_Wtime();

		wait();

		printf("SLAVE-%d >>> TIME:%f\n", world_rank, getTime(start_time));
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

void Calculator::wait()
{
	bool flag = true;
	while (flag)
	{
		Message msg;
		send(msg, world_rank, world_size);
		switch (msg.getFirst())
		{
		case HALT:
			flag = false;
			break;
		case INIT:
			level = msg.getSecond();
			init();
			break;
		case CHANGE_DEBUG_SUM:
			changeDebugSlave(msg.getSecond(), DEBUG_SUM);
			break;
		case CHANGE_DEBUG_SYS:
			changeDebugSlave(msg.getSecond(), DEBUG_SUM);
			break;
		case SIN:
			sinus(msg.getSecond());
			break;
		case COS:
			cosinus(msg.getSecond());
			break;
		default:
			flag = false;
			break;
		}
	}
}

double Calculator::sinus(double x)
{
	if (world_rank == 0)
	{
		Message msg(SIN, x);
		send(msg, world_rank, world_size);
	}
	double sum = 0;
	for (int i = start; i <= end; i++)
	{
		if (DEBUG_SUM)
		{
			printf("components: %lf %lf %ld for %d   ",
				   pow(-1, i),
				   pow(x, 2 * i + 1),
				   factorials[2 * i + 1], i);
		}
		sum += pow(-1, i) * pow(x, 2 * i + 1) / factorials[2 * i];
		if (DEBUG_SUM)
		{
			printf("n=%d sum=%.16lf rank=%d\n", i, sum, world_rank);
		}
	}
	Message msg(SIN, sum);
	get(msg, world_rank, world_size);
	if (world_rank == 0)
	{
		return msg.returnSum(world_size);
	}
	else
	{
		return 0.0;
	}
}

double Calculator::cosinus(double x)
{
	if (world_rank == 0)
	{
		Message msg(COS, x);
		send(msg, world_rank, world_size);
	}
	double sum = 0;
	for (int i = start; i <= end; i++)
	{
		if (DEBUG_SUM)
		{
			printf("components: %lf %lf %ld for %d   ",
				   pow(-1, i),
				   pow(x, 2 * i),
				   factorials[2 * i], i);
		}
		sum += pow(-1, i) * pow(x, 2 * i) / factorials[2 * i];
		if (DEBUG_SUM)
		{
			printf("n=%d sum=%.16lf rank=%d\n", i, sum, world_rank);
		}
	}
	Message msg(COS, sum);
	get(msg, world_rank, world_size);
	if (world_rank == 0)
	{
		return msg.returnSum(world_size);
	}
	else
	{
		return 0.0;
	}
}

long Calculator::getFactorial(long n)
{
	long i = 0;
	if (factorials == nullptr)
	{
		throw SystemException(__LINE__, __func__, "Error - nullptr");
	}
	if (n <= 1)
	{
		factorials[n - 1] = i = 1;
	}
	else
	{
		factorials[n - 1] = i = n * getFactorial(n - 1);
	}
	return i;
}

void Calculator::factorial(int n)
{
	n = n * 2 + 2;
	if (n < 1)
	{
		throw SystemException(__LINE__, __func__, "Error - n < 1");
	}
	factorials = (ulong *)calloc(n, sizeof(ulong));
	if (factorials == nullptr)
	{
		throw SystemException(__LINE__, __func__, "Error - n < 1");
	}

	getFactorial(n);
}

void Calculator::printFact()
{
	if (factorials != nullptr)
	{
		for (int i = 0; i < level * 2; i++)
		{
			printf("%ld ", factorials[i]);
		}
		printf("\n");
	}
	else
	{
		print("nullptr - factorials");
	}
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
	printf("Entered string:\n%s\n", str.c_str());
	for (int i = 0; i < place; i++)
	{
		printf(" ");
	}
	print("^");
	printf("Error on char:'%c'\n", str[place]);
}

inline double Calculator::getTime(double start)
{
	return MPI_Wtime() - start;
}

void Calculator::changeDebugSys()
{
	changeDebugFlag(DEBUG_SYS);

	Message msg(CHANGE_DEBUG_SYS, DEBUG_SYS);
	send(msg, world_rank, world_size);
	if (DEBUG_SYS)
	{
		printf("Debug flag changed to:true\n");
	}
}

void Calculator::changeDebugSlave(int state, bool &flag)
{
	if (state == true)
	{
		flag = true;
	}
	else if (state == false)
	{
		flag = false;
	}
	else
	{
		throw SystemException("Error - bad state - stop programm");
	}
}

void Calculator::changeDebugSum()
{
	changeDebugFlag(DEBUG_SUM);

	Message msg(CHANGE_DEBUG_SUM, DEBUG_SUM);
	send(msg, world_rank, world_size);
	if (DEBUG_SYS)
	{
		printf("DEBUG SUM:%s\n", DEBUG_SUM == true ? "true" : "false");
	}
}

void Calculator::changeDebugFlag(bool &flag)
{
	if (world_rank != 0)
	{
		throw SystemException(__LINE__, __func__, "Error - Wrong thread");
	}

	if (str.substr(4, 3) == ":on")
	{
		flag = true;
	}
	else if (str.substr(4, 4) == ":off")
	{
		flag = false;
	}
	else
	{
		throw SystemException(__LINE__, __func__, "Error - wrong command");
	}
}

void Calculator::changeSum()
{
	if (str == "SUM")
	{
		SUM = true;
	}
	else
	{
		SUM = false;
	}
	if (DEBUG_SYS)
	{
		printf("SUM:%s\n", SUM == true ? "true" : "false");
	}
}

void Calculator::changeDebugAll()
{
	changeDebugSum();
	changeDebugSys();
}