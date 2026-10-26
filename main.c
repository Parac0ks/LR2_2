#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Прототипи функцій (оголошення перед main)
double num_comput_integral_l_re(double left_boundary_a, double right_boundary_b, unsigned int intervals);
double num_comput_integral_r_re(double left_boundary_a, double right_boundary_b, unsigned int intervals);
double num_comput_integral_Simps(double left_boundary_a, double right_boundary_b, unsigned int intervals);
double integrand_expression(double x);

int main()
{
    double left_boundary_a = 0, right_boundary_b = 0;
    double measurement_error = 0, I1 = 0, I2 = 0;
    int intervals, var, i;
    double integral_s = 0;

    while(1)
    {
        // Введення меж інтегрування
        printf("\n\tEnter the left boundary \n X(first)=");
        scanf("%lf", &left_boundary_a);

        printf("\n\tEnter the right boundary \n X(last)=");
        scanf("%lf", &right_boundary_b);

        // Введення кількості інтервалів із валідацією (має бути більше 0)
        do{
            printf("\tEnter the number of partition intervals (N>0)\nN=");
            scanf("%u", &intervals);
        }while(intervals <= 0);

        // Введення похибки вимірювання
        printf("\n\tEnter the measurment error of integration\n Measurment error=");
        scanf("%lf", &measurement_error);

        // Меню вибору методу розрахунку з валідацією варіанту
        do
        {
            printf("\nChoose the method of calculating:\n");
            printf("\t1. By Left Rectangles :\n");
            printf("\t2. By Right Rectangles:\n");
            printf("\t3. By ntegral_Simps's method (parabola method):\n"); // в методичці вказано варіант 4 для Сімпсона
            scanf("%u", &var);

            if (var != 1 && var != 2 && var != 3) // виправлено логіку перевірки під твої скріншоти
                printf("\nYou are mistaken\n");

        }while(var != 1 && var != 2 && var != 3);

        system("cls"); // Очищення екрану перед виведенням результату

        switch(var)
        {
            case 1: // Метод лівих прямокутників
            {
                integral_s = num_comput_integral_l_re(left_boundary_a, right_boundary_b, intervals);
                printf("\n\n\t*Left Rectangles method*\n");
                printf("\n\ta = %.2lf  \tb = %.2lf  \tintegral = %.8lf  \tn = %d",
                       left_boundary_a, right_boundary_b, integral_s, intervals);
            }
            break;

            case 2: // Метод правих прямокутників із циклом уточнення похибки
            {
                printf("\n\n\t======*Right Rectangles method*======\n");
                I1 = num_comput_integral_r_re(left_boundary_a, right_boundary_b, intervals);

                for (i = 0; i < 2; i++) {
                    if (i == 1){
                        intervals += 2;
                        I2 = num_comput_integral_r_re(left_boundary_a, right_boundary_b, intervals);

                        if (fabs(I1 - I2) <= measurement_error) {
                            break;
                        }
                    }
                    else {
                        i = 0; // Скидання ітератора для продовження циклу уточнення кроку
                    }
                }
                printf("\n\ta = %.2lf  \tb = %.2lf  \tintegral = %.8lf  \tn = %d",
                       left_boundary_a, right_boundary_b, I2, intervals);
            }
            break;

            case 3: // Метод Сімпсона (парабол)
            {
                integral_s = num_comput_integral_Simps(left_boundary_a, right_boundary_b, intervals);
                printf("\n\n\t======*num_comput_integral_Simps's method *======\n");
                printf("\n\ta = %.2lf  \tb = %.2lf  \tintegral = %.8lf  \tn = %d",
                       left_boundary_a, right_boundary_b, integral_s, intervals);
            }
            break;
        }

        printf("\n\n---------------------------------------\n");
    }

    return 0;
}

// 1. Метод лівих прямокутників (повністю за кодом із сторінки 79-80)
double num_comput_integral_l_re(double left_boundary_a, double right_boundary_b, unsigned int intervals)
{
    double integral_s = 0, x = 0, h;
    unsigned int i;

    h = (right_boundary_b - left_boundary_a) / intervals;
    x = left_boundary_a;

    for (i = 0; i < intervals; i++){
        integral_s += integrand_expression(x);
        x += h;
    }
    return integral_s * h;
}

// 2. Метод правих прямокутників (дописано самостійно в стилі методички)
double num_comput_integral_r_re(double left_boundary_a, double right_boundary_b, unsigned int n)
{
    double integral_s = 0, x = 0, h;
    unsigned int i;

    h = (right_boundary_b - left_boundary_a) / n;
    x = left_boundary_a + h; // Для правих прямокутників починаємо з x0 + h

    for (i = 0; i < n; i++){
        integral_s += integrand_expression(x);
        x += h;
    }
    return integral_s * h;
}

// 3. Метод Сімпсона / Парабол (дописано самостійно в стилі методички)
double num_comput_integral_Simps(double left_boundary_a, double right_boundary_b, unsigned int n)
{
    // Для методу Сімпсона кількість інтервалів n обов'язково має бути парною
    if (n % 2 != 0) n++;

    double h = (right_boundary_b - left_boundary_a) / n;
    double sum_even = 0.0; // Сума точок із парними індексами
    double sum_odd = 0.0;  // Сума точок із непарними індексами
    unsigned int i;

    for (i = 1; i < n; i++) {
        double x = left_boundary_a + i * h;
        if (i % 2 == 0) {
            sum_even += integrand_expression(x);
        } else {
            sum_odd += integrand_expression(x);
        }
    }

    // Формула Парабол: (h/3) * (f(a) + f(b) + 4*сума_непарних + 2*сума_парних)
    double integral_s = (h / 3.0) * (integrand_expression(left_boundary_a) +
                                     integrand_expression(right_boundary_b) +
                                     4.0 * sum_odd + 2.0 * sum_even);
    return integral_s;
}

// Інтегрована функція (за кодом із сторінки 80)
double integrand_expression(double x) // основна функція
{
    return pow(x, 2) + 2 * x;
}
