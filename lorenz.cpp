// Для запуска ввести комманду gcc lorenz.c -o lorenz.o && gnuplot lorenz.plot
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

double O = 10, r = 28, b = 8 / 3;
double x_start = 3.051522, y_start = 1.582542, z_start = 15.62388, d_xyz = 0.04;
int seq_count = 400;

// Функция определяющая положение точки относительно двух плоскостей
int plane_position(double x)
{
    return (x - 7.18 > 0) + (x + 7.15 > 0);
}


double f_lorenz_x(double x, double y, double z)
{
    return O * (-x + y);
}

double f_lorenz_y(double x, double y, double z)
{
    return r * x - y - x * z;
}

double f_lorenz_z(double x, double y, double z)
{
    return -b * z + x * y;
}


// Метод Дормана - Принца с порядком ошибки 8
std::string DOPRI8(double x, double y, double z, int steps, double h, bool need_data)
{

    std::ofstream seq_file("sequence.txt", std::ios::app);
    std::ofstream data_file("attractor_data.txt", std::ios::app);
    std::string outstring = "";
    // Таблица Бутчера
    static double a[14][13] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {(double)1 / 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {(double)1 / 48, (double)1 / 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {(double)1 / 32, 0, (double)3 / 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {(double)5 / 16, 0, (double)-75 / 64, (double)75 / 64, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {(double)3 / 80, 0, 0, (double)3 / 16, (double)3 / 20, 0, 0, 0, 0, 0, 0, 0, 0},
        {(double)29443841 / 614563906, 0, 0, (double)77736538 / 692538347, (double)-28693883 / 1125000000, (double)23124283 / 1800000000, 0, 0, 0, 0, 0, 0, 0},
        {(double)16016141 / 946692911, 0, 0, (double)61564180 / 158732637, (double)22789713 / 633445777, (double)545815736 / 2771057229, (double)-180193667 / 1043307555, 0, 0, 0, 0, 0, 0},
        {(double)39632708 / 573591083, 0, 0, (double)-433636366 / 683701615, (double)-421739975 / 2616292301, (double)100302831 / 723423059, (double)790204164 / 839813087, (double)800635310 / 3783071287, 0, 0, 0, 0, 0},
        {(double)246121993 / 1340847787, 0, 0, (double)-37695042795 / 15268766246, (double)-309121744 / 1061227803, (double)-12992083 / 490766935, (double)6005943493 / 2108947869, (double)393006217 / 1396673457, (double)123872331 / 1001029789, 0, 0, 0, 0},
        {(double)-1028468189 / 846180014, 0, 0, (double)8478235783 / 508512852, (double)1311729495 / 1432422823, (double)-10304129995 / 1701304382, (double)-48777925059 / 3047939560, (double)15336726248 / 1032824649, (double)-45442868181 / 3398467696, (double)3065993473 / 597172653, 0, 0, 0},
        {(double)185892177 / 718116043, 0, 0, (double)-3185094517 / 667107341, (double)-477755414 / 1098053517, (double)-703635378 / 230739211, (double)5731566787 / 1027545527, (double)5232866602 / 850066563, (double)-4093664535 / 808688257, (double)3962137247 / 1805957418, (double)65686358 / 487910083, 0, 0},
        {(double)403863854 / 491063109, 0, 0, (double)-5068492393 / 434740067, (double)-411421997 / 543043805, (double)652783627 / 914296604, (double)11173962825 / 925320556, (double)-13158990841 / 6184727034, (double)3936647629 / 1978049680, (double)-160528059 / 685178525, (double)248638103 / 1413531060, 0, 0}
    };

    double b_coefs[] = {(double)14005451 / 335480064, 0, 0, 0, 0, (double)-59238493 / 1068277825, (double)181606767 / 758867731, (double)561292985 / 797845732, (double)-1041891430 / 1371343529, (double)760417239 / 1151165299, (double)118820643 / 751138087, (double)-528747749 / 2220607170, (double)1 / 4};

    double k_x[13] = {0};
    double k_y[13] = {0};
    double k_z[13] = {0};

    int cur_plane_position;
    int prev_plane_posion = -1;
    do
    {
        k_x[0] = f_lorenz_x(x, y, z);
        k_y[0] = f_lorenz_y(x, y, z);
        k_z[0] = f_lorenz_z(x, y, z);

        double sum_x, sum_y, sum_z;

        for (int i = 1; i <= 12; i++)
        {
            sum_x = 0, sum_y = 0, sum_z = 0;

            for (int j = 0; j < i; j++)
            {
                sum_x += a[i][j] * k_x[j];
                sum_y += a[i][j] * k_y[j];
                sum_z += a[i][j] * k_z[j];
            }

            sum_x = x + sum_x * h;
            sum_y = y + sum_y * h;
            sum_z = z + sum_z * h;

            k_x[i] = f_lorenz_x(sum_x, sum_y, sum_z);
            k_y[i] = f_lorenz_y(sum_x, sum_y, sum_z);
            k_z[i] = f_lorenz_z(sum_x, sum_y, sum_z);
        }

        sum_x = 0, sum_y = 0, sum_z = 0;

        for (int i = 0; i <= 12; i++)
        {
            sum_x += b_coefs[i] * k_x[i];
            sum_y += b_coefs[i] * k_y[i];
            sum_z += b_coefs[i] * k_z[i];
        }
        x += sum_x * h;
        y += sum_y * h;
        z += sum_z * h;
 
        if (need_data)
        {
            data_file << x << " " << y << " " << z  << std::endl;
        }

        cur_plane_position = plane_position(x);
        if (cur_plane_position != prev_plane_posion)
        {
            if (cur_plane_position == 0)
                seq_file << "."; 
                outstring += ".";
            if (cur_plane_position == 2)
                seq_file << "#"; 
                outstring += "#";
            if (cur_plane_position == 1)
                seq_file << ((prev_plane_posion == 0) ? "." : "#");
                outstring += ((prev_plane_posion == 0) ? "." : "#");
            prev_plane_posion = cur_plane_position; 
        }

    } while (--steps != 0);
    seq_file << std::endl;
    seq_file.close();
    data_file.close();
    return outstring;
}

// Функция для подсчета расстояния между строками методом посимвольного сравнения
int countDifferentChars(const std::string& str1, const std::string& str2) {
    int minLength = std::min(str1.length(), str2.length());
    int diffCount = 0;

    for (int i = 0; i < minLength; ++i) {
        if (str1[i] != str2[i]) {
            ++diffCount;
        }
    }

    return diffCount + std::abs((int)str1.length() - (int)str2.length());
}


// Функция для подсчета расстояния между строками с использованием расстояния Левенштейна
double levenshtein_distance(const std::string &source, const std::string &target)
{
    if (source.size() > target.size())
    {
        return levenshtein_distance(target, source);
    }

    size_t min_size = source.size(), max_size = target.size();
    std::vector<size_t> lev_dist(min_size + 1);

    for (size_t i = 0; i <= min_size; ++i)
    {
        lev_dist[i] = i;
    }

    for (size_t j = 1; j <= max_size; ++j)
    {
        size_t previous_diagonal = lev_dist[0], previous_diagonal_save;
        ++lev_dist[0];

        for (size_t i = 1; i <= min_size; ++i)
        {
            previous_diagonal_save = lev_dist[i];
            if (source[i - 1] == target[j - 1])
            {
                lev_dist[i] = previous_diagonal;
            }
            else
            {
                lev_dist[i] = std::min(std::min(lev_dist[i - 1], lev_dist[i]), previous_diagonal) + 1;
            }
            previous_diagonal = previous_diagonal_save;
        }
    }

    return static_cast<double>(lev_dist[min_size]);
}


double polynomial_hash(std::string str)
{
    int length_of_seq = str.length();

    double first = 50;
    double last = 5;
    double q = pow(first/last, 1./length_of_seq);

    double ans = 0;
    double powered = first;
    int symbol;
    for(int i = 0; i < str.length(); i++)
    {
        symbol = (str[i] == '#') ? 1 : 0;
        ans += (symbol * powered);
        powered *= q;
    }
    return ans;
}


void sequence_analyse(double x, double y, double z, int seq_count, double d_xyz)
{
    std::string str;
    FILE* frac_file = fopen("fractal.txt", "w");
    std::string first_str = DOPRI8(x, y, z, 38000, 0.001, false);

    for (int i = 0; i < seq_count; i++)
    {
        for(int j = 0; j < seq_count; j++)
        {
            str = DOPRI8(x + i * d_xyz, y + j * d_xyz, z, 38000, 0.001, (i == 0 && j == 0) ? true : false);
            fprintf(frac_file, "%d %d %lf\n", i, j, (double)countDifferentChars(str, first_str));
        }
    }

    std::ifstream file("sequence.txt");
    std::string firstLine;
    std::getline(file, firstLine);
    std::string line;
    std::ofstream outFile("research_data.txt");
    std::vector<int> arr;
    while (std::getline(file, line))
    {
        arr.push_back(countDifferentChars(firstLine, line));
    }
    for(int i = 0; i < arr.size(); i++){
        outFile << d_xyz * i << " " << arr.at(i) << std::endl;
    }
    outFile.close();
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        fprintf(stderr, "Usage: prog <x> <y> <z>\n");
        return -1;
    }
    if (sscanf(argv[1], "%lf", &x_start) < 1 || sscanf(argv[2], "%lf", &y_start) < 1 || sscanf(argv[3], "%lf", &z_start) < 1 || sscanf(argv[4], "%d", &seq_count) < 1 || sscanf(argv[5], "%lf", &d_xyz) < 1)
    {
        fprintf(stderr, "Usage: prog <x> <y> <z> <seq_count> <d_xyz>\n");
        return -1;
    }
    sequence_analyse(x_start, y_start, z_start, seq_count, d_xyz);
    return 0;
}
