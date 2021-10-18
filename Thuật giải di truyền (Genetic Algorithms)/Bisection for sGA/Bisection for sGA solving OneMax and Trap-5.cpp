#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

int problem_size; /*kích thước vấn đề*/
long long number_of_evaluations; /*hàm fitness evaluation*/
int k_bisection;
bool crossover_type;
bool problem_type;
int random_seed;

/*cá thể*/
struct individual
{
	bool* value = new bool[problem_size];
	int* fitness = new int; /*độ thích nghi fitness*/
};

individual* population; /*quần thể*/
individual* parent; /*cha mẹ*/
individual* offspring; /*con cái*/
individual* pool; /*gom nhóm 2 thế hệ cha mẹ và con cái*/

void freeupspace(individual* a, int n)
{
	for (int i = 0; i < n; i++)
	{
		delete[] a[i].value;
		delete a[i].fitness;
	}
	delete[] a;
}

/*hàm đánh giá oneMax*/
int oneMax(bool* v)
{
	int c = 0;
	for (int i = 0; i < problem_size; i++)
	{
		if (v[i]) c++;
	}
	return c;
}

int onemaxFitness(individual temp)
{
	number_of_evaluations++;
	return oneMax(temp.value);
}

/*hàm bẫy liên tiếp trap-5*/
int concatenated_trap_k(individual temp, int b, int e)
{
	int f_trap = 0;
	for (int i = b; i < e; i++)
	{
		if (temp.value[i]) f_trap++;
	}
	if (f_trap == 5) return 5;
	return 4 - f_trap;
}

int trap5Fitness(individual temp)
{
	number_of_evaluations++;
	int f = 0;
	for (int i = 0; i < problem_size; i += 5)
	{
		f += concatenated_trap_k(temp, i, i + 5);
	}
	return f;
}

/*hàm khởi tạo quần thể*/
void initialize_population(int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < problem_size; j++)
		{
			population[i].value[j] = rand() % 2;
		}
		*population[i].fitness = (!problem_type) ? onemaxFitness(population[i]) : trap5Fitness(population[i]);
	}
}

void assign_value(bool* a, bool* b)
{
	for (int i = 0; i < problem_size; i++)
	{
		a[i] = b[i];
	}
}

void assign_population(individual* a, individual* b, int n)
{
	for (int i = 0; i < n; i++)
	{
		assign_value(a[i].value, b[i].value);
		*a[i].fitness = *b[i].fitness;
	}
}

/*Hàm chọn lọc cạnh tranh*/
individual tournament_selection(int b, int e)
{
	int max_fitness = *pool[b].fitness;
	int max_index = b; /*best*/
	for (int i = b + 1; i < e; i++)
	{
		if (*pool[i].fitness > max_fitness)
		{
			max_fitness = *pool[i].fitness;
			max_index = i;
		}
	}
	return pool[max_index];
}

/*Thực hiện phép lai ghép bên trong quần thể*/
/**Lai ghép 1 điểm 1X (one point crossover)**/
void one_point_crossover(bool* a, bool* b)
{
	int index = rand() % problem_size;
	for (int i = index; i < problem_size; i++)
	{
		swap(a[i], b[i]);
	}
}

void one_point_Offspring(int n)
{
	for (int i = 0; i < n; i += 2)
	{
		one_point_crossover(offspring[i].value, offspring[i + 1].value);
		*offspring[i].fitness = (!problem_type) ? onemaxFitness(offspring[i]) : trap5Fitness(offspring[i]);
		*offspring[i + 1].fitness = (!problem_type) ? onemaxFitness(offspring[i + 1]) : trap5Fitness(offspring[i + 1]);
	}
}

/**Lai ghép đồng nhất UX (uniform crossover)**/
void uniform_crossover(bool* a, bool* b)
{
	for (int i = 0; i < problem_size; i++)
	{
		double t = (double)rand() / (RAND_MAX);
		if ((t) < 0.5)
			swap(a[i], b[i]);
	}
}

void uniform_Offspring(int n)
{
	for (int i = 0; i < n; i += 2)
	{
		uniform_crossover(offspring[i].value, offspring[i + 1].value);
		*offspring[i].fitness = (!problem_type) ? onemaxFitness(offspring[i]) : trap5Fitness(offspring[i]);
		*offspring[i + 1].fitness = (!problem_type) ? onemaxFitness(offspring[i + 1]) : trap5Fitness(offspring[i + 1]);
	}
}

void generateOffspring(int n)
{
	assign_population(parent, population, n);
	assign_population(offspring, population, n);
	if (!crossover_type) one_point_Offspring(n);
	else uniform_Offspring(n);
}

/*Gom nhóm hai thế hệ Cha mẹ + Con cái*/
void generate_pool(int n)
{
	for (int i = 0; i < n; i++)
	{
		assign_value(pool[i].value, parent[i].value);
		*pool[i].fitness = *parent[i].fitness;
		assign_value(pool[n + i].value, offspring[i].value);
		*pool[n + i].fitness = *offspring[i].fitness;
	}
}

/*Shuffle (xáo trộn) pool*/
void shuffle_pool(int n)
{
	for (int i = 0; i < n; i++)
	{
		swap(pool[rand() % (2 * n)], pool[rand() % (2 * n)]);
	}
}

/*Tạo quần thể mới*/
void generateNewPopulation(int n)
{
	generate_pool(n);
	int index = 0;

	shuffle_pool(n);
	for (int i = 0; i < 2 * n; i += 4)
	{
		individual temp = tournament_selection(i, i + 4);
		assign_value(population[index].value, temp.value);
		*population[index].fitness = *temp.fitness;
		index++;
	}

	shuffle_pool(n);
	for (int i = 0; i < 2 * n; i += 4)
	{
		individual temp = tournament_selection(i, i + 4);
		assign_value(population[index].value, temp.value);
		*population[index].fitness = *temp.fitness;
		index++;
	}
}

/*kiểm tra điều kiện hội tụ của thuật toán*/
bool checking_convergence(int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < problem_size; j++)
		{
			if (*(population[i].value + j) != *(population[i + 1].value + j)) return false;
		}
	}
	return true;
}

bool checking_solution(int n)
{
	for (int i = 0; i < n; i++)
	{
		if (*population[i].fitness == problem_size)
		{
			freeupspace(population, n);
			return true;
		}
	}
	freeupspace(population, n);
	return false;
}

/*cài đặt thuật toán tiến hóa đơn giản (simple Genetic Algorithm)*/
bool sGA(int n, int random_seed)
{
	srand(random_seed);
	population = new individual[n];
	offspring = new individual[n];
	parent = new individual[n];
	pool = new individual[2 * n];
	initialize_population(n);
	while (true)
	{
		generateOffspring(n);
		generateNewPopulation(n);
		if (checking_convergence(n)) break;
	}
	freeupspace(offspring, n);
	freeupspace(parent, n);
	freeupspace(pool, 2 * n);
	return checking_solution(n);
}

/*Chạy bisection*/
/*Chạy 10 lần bisection*/
bool check(int n)
{
	int MSSV = 18520186 /*MSSV*/;
	random_seed = MSSV + k_bisection * 10;
	for (int i = 0; i < 10; i++)
	{
		if (!sGA(n, random_seed + i))
			return false;
	}
	return true;
}

int bisection(int problem_size)
{
	int n_upper = 4;
	bool success = false;
	/**Bước 1: tìm cận trên (upper bound) của MRPS**/
	do
	{
		if (n_upper == 8192)
		{
			return 0;
		}
		n_upper = n_upper * 2;
		success = check(n_upper);
	} while (!success);
	/**Bước 2: Tính kích thước quần thể nhỏ nhất cần thiết (minimally required population size - MRPS)**/
	int n_lower = n_upper / 2;
	while ((n_upper - n_lower) / double(n_upper) > 0.1)
	{
		int n = (n_upper + n_lower) / 2;
		success = check(n);

		if (success)
		{
			n_upper = n;
		}
		else n_lower = n;

		if (n_upper - n_lower <= 2) break;
	}
	number_of_evaluations = 0;
	check(n_upper);
	return n_upper;
}

/*Độ lệch chuẩn*/
double stdDeviation(vector<double> v, double mean)
{
	double deviation = 0;
	for (int i = 0; i < v.size(); i++)
	{
		deviation += pow(v[i] - mean, 2);
	}
	return sqrt(deviation / v.size());
}

int main()
{
	cout << "Nhap vao kich thuoc van de (problem size) [10, 20, 40, 80, 160]: ";
	cin >> problem_size;
	cout << "Chon function Onemax hay trap5: (0: onemax, 1: trap5): ";
	cin >> problem_type;
	cout << "Chon phep lai ghep 1X hay UX (0: 1X, 1: UX): ";
	cin >> crossover_type;
	vector<double> MRPS;
	vector<double> no_of_evaluation;
	vector<int> random_seed_set;
	double mean_of_no_evaluations = 0;
	double mean_of_MRPS = 0;
	for (k_bisection = 0; k_bisection < 10; k_bisection++)
	{
		cout << "Ket qua 10 lan chay bisection: " << k_bisection << endl;
		int n = bisection(problem_size);
		mean_of_MRPS += n;
		mean_of_no_evaluations += ((n == 0) ? 0 : number_of_evaluations / 10.0);
		MRPS.push_back(double(n));
		no_of_evaluation.push_back((n == 0) ? 0 : number_of_evaluations / 10.0);
		random_seed_set.push_back(random_seed);
	}
	for (int i = 0; i < 10; i++)
	{
		cout << MRPS[i] << ' ' << no_of_evaluation[i] << ' ' << random_seed_set[i] << endl;
	}
	cout << "Ket qua MRPS cua sGA cho phep lai va function tuong ung la:" << mean_of_MRPS / (10.0) << ' ' << stdDeviation(MRPS, mean_of_MRPS / 10.0) << endl;
	cout << "So lan goi ham danh gia (number_of_evaluations) cua sGA cho phep lai va function tuong ung la:" << (mean_of_no_evaluations / (10.0)) << ' ' << stdDeviation(no_of_evaluation, mean_of_no_evaluations / 10.0) << endl;
}
