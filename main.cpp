#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <Windows.h>




import VectorModule;
import Employee;

std::chrono::year_month_day read_ymd_input(const std::string& prompt) {
    // Получаем текущую дату
    std::cin.clear();
    std::cin.ignore();
    std::chrono::year_month_day today = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
    std::chrono::year_month_day current_ymd = std::chrono::year_month_day{ today };

    while (true) 
    {
        std::cout << "\n" << prompt;
        std::string input;
        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::chrono::year_month_day ymd;


        iss >> std::chrono::parse("%F", ymd);

        if (!iss.fail() && ymd.ok())
        {
            if (ymd <= current_ymd) return ymd;
            else std::cout << "Ошибка: дата не может быть в будущем.\n";
        }
        else std::cout << "Ошибка: неверный формат даты. Используйте формат ГГГГ-ММ-ДД (например, 2023-12-31)\n";

    }
}

template<typename T, typename Predicat>
void validation(T& x, Predicat condition, std::string message)
{
    std::cout << message << "\n>>> ";
    while (!(std::cin >> x and condition(x)))
    {
        std::cout << "Input error!" << '\n';
        std::cin.clear();
        std::cin.ignore(std::cin.rdbuf()->in_avail());
        std::cout << message << "\n>>> ";
    }
}

void print_to_file(Vector<Employee>& arr)
{
    std::string filename;
    std::cout << "Введите имя файла "; std::cin >> filename;
    std::ofstream file(filename);
    arr.print_list(file);
    std::cout << "\nИнформация записана в файл " << filename << "\n";
    file.close();
}

void print_selection(Vector<Employee>& arr)
{
    std::string c;
    std::cout << "Куда распечатать выборку ? f - файл / any key - в консоль "; std::cin >> c;
    c == "f" ? print_to_file(arr) : arr.print_list();
}

short source_menu()
{
    std::cout << "<======= ЗАПОЛНИТЬ КОНТЕЙНЕР =======>\n"
        << "1. Из файла\n"
        << "2. Вручную\n"
        << "0. Выход\n";
    short res{};
    validation(res, [](short x) {return x >= 0 && x <= 2; }, "");
    return res;
}

short task_menu()
{
    std::cout
        << "\n1. Ввести в консоль\n"
        << "2. Вывести в файл\n"
        << "3. Добавить новую запись\n"
        << "4. Удалить запись по индексу\n"
        << "5. Удалить запись по значению\n"
        << "6. Изменить запись\n"
        << "7. Выборка по ЗП\n"
        << "8. Выборка по фамилии\n"
        << "9. Выборка мужчин\n"
        << "10. Выборка немужчин(женщин)\n"
        << "11. Выборка по заданному дню\n"
        << "12. Отсортировать \n"
        << "13. Отсортировать в обратном порядке\n"
        << "0. Выход";
    short res{};
    validation(res, [](short x) {return x >= 0 && x <= 13; }, "");
    return res;
}

void sample()
{
    std::cout << "Введите данные сотрудников в формате:\n"
        << "Имя\nФамилия\nОтчество\nДата_приёма (ГГГГ-ММ-ДД)\n"
        << "Дата_увольнения (- если нет)\nДолжность (Cleaner, Cook, Manager, Developer, HR, Analyst)\nПол (Male, Female, Frog)\nЗарплата\n"
        << "В конце ввода нажмите Ctrl+Z:\n";
}

int main()
{
    SetConsoleOutputCP(1251);
    Vector<Employee> arr;
    short source_choice{}, action_choice{};
    Employee emp;
    do
    {
        source_choice = source_menu();
        switch (source_choice)
        {
        case 1:
        {
            std::ifstream file("test.txt");
            arr.fill_list(file);
            file.close();
            break;
        }
        case 2:
        {
            sample();
            arr.fill_list(std::cin);
            std::cout << "\nВведено сотрудников: " << arr.size() << "\n";
            break;
        }
        default:
            break;
        }
        if (source_choice != 0)
        {
            do
            {
                action_choice = task_menu();
                switch (action_choice)
                {
                case 1: // распечатать в консоль
                {
                    arr.print_list();
                    break;
                }
                case 2: //распечатать в файл
                {
                    print_to_file(arr);
                    break;
                }
                case 3: //добавить запись
                {
                    sample();
                    Employee newEmp;
                    std::cin >> newEmp;
                    arr.add(newEmp);
                    std::cout << "Новый работник" << newEmp.get_name() + " " + newEmp.get_surname() << "успешно добавлен\n";
                    break;
                }
                case 4://удалить по индексу
                {
                    size_t ind;
                    std::cout << "Введите индекс: ";
                    std::cin >> ind;
                    arr.remove_by_ind(ind) ? std::cout << "\nэлемент удалён\n" :
                        std::cout << "\nэлемента с таким индексом не существует\n";
                    break;
                }
                case 5://удалдить по значению 
                {
                    Employee newEmp;
                    sample();
                    std::cin >> newEmp;
                    arr.remove_by_value(newEmp) ? std::cout << "\nэлемент удалён\n" :
                        std::cout << "\nтакого элемента не существует\n";
                    break;
                }
                case 6://Изменить запись
                {
                    int ind; validation(ind, [arr](int x) {return x > 0 and x < arr.size(); }, "\nКакой элемент поменять?\n");
                    sample();
                    Employee newEmp;
                    std::cin >> newEmp;
                    arr.modify(ind, newEmp);
                    std::cout << "\nЭлемент успещно изменён\n";
                    break;
                }
                case 7://выборка по зп
                {
                    int salary1, salary2;
                    Vector<Employee> a;
                    validation(salary1, [](int x) {return x; }, "\nВведите начальное значение ");
                    validation(salary2, [](int x) {return x; }, "\nВведите конечное значение ");
                    if (arr.select([salary1, salary2](const Employee& emp)
                        {return emp.get_salary() < salary2 and
                        emp.get_salary() > salary1; }, a)) print_selection(a);
                    else std::cout << "\nПодходящих значений не найдено\n";

                    break;
                }
                case 8://Выборка по фамилии
                {
                    std::string surname1, surname2;
                    Vector<Employee> a;
                    std::cout << "\nВведите начальное значение "; std::cin >> surname1;
                    std::cout << "\nВведите конечное значение "; std::cin >> surname2;
                    if (arr.select([surname1, surname2](const Employee& emp)
                        {return emp.get_surname() < surname2
                        and emp.get_surname() > surname1; }, a)) print_selection(a);
                    else std::cout << "\nПодходящих значений не найдено\n";
                    print_selection(a);
                    break;
                }
                case 9://Выборка мужчин
                {
                    Vector<Employee> a;
                    if (arr.select([](const Employee& emp) {return emp.get_sex() == Sex::Male; }, a))
                        print_selection(a);
                    else std::cout << "\nМужиков нет.\n";
                    break;
                }
                case 10://Выборка women
                {
                    Vector<Employee> a;
                    if (arr.select([](const Employee& emp) {return emp.get_sex() == Sex::Female; }, a))
                        print_selection(a);
                    else std::cout << "\nЖеньщен нет.\n";
                    break;
                }
                case 11:
                {
                    Vector<Employee> a;
                    std::chrono::year_month_day date = read_ymd_input("Введите дату для поиска:");
                    if (arr.select([date](const Employee& emp) {return emp.get_applyment_date()
                        < date and emp.get_fire_date() > date; }, a)) print_selection(a);
                    else std::cout << "\nНикто не работал в этот день.\n";
                    break;
                }
                case 12://Отсортировать
                {
                    if (arr.size() != 0)
                    {
                        arr.sort_by([](const Employee& a, const Employee& b)
                            { if (a.get_surname() != b.get_surname()) return a.get_surname() < b.get_surname();
                        return a.get_name() < b.get_name(); }, true);
                        std::cout << "\nСписок отсортирован\n";
                    }
                    else
                        std::cout << "\nСписок пуст\n";
                    break;
                }
                case 13://Отсортировать в обратном порядке
                {
                    if (arr.size() != 0)
                    {
                        arr.sort_by([](const Employee& a, const Employee& b)
                            { if (a.get_surname() != b.get_surname()) return a.get_surname() < b.get_surname();
                        return a.get_name() < b.get_name(); }, false);
                        std::cout << "\nСписок отсортирован\n";
                    }
                    else 
                        std::cout << "\nСписок пуст\n";
                    break;
                }
                default:
                    break;
                }


            } while (action_choice != 0);
        }
        break;
    } while (source_choice != 0);

   






    //SALARY SELECTION
    //int salary1 = 10000, salary2 = 20001;
    //arr.select([salary1, salary2](const Employee& emp) {return emp.get_salary() < salary2 and emp.get_salary() > salary1; }, a);
    //SURNAME SELECTION
    //std::string surname1 = "Bebra", surname2 = "Qewish";
    //arr.select([surname1, surname2](const Employee& emp) {return emp.get_surname() < surname2 and emp.get_surname() > surname1; }, a);
    //SEX SELECTION
    //arr.select([](const Employee& emp) {return emp.get_sex() == Sex::Male; }, a);
    //arr.select([](const Employee& emp) {return emp.get_sex() == Sex::Female; }, a);
    //arr.select([](const Employee& emp) {return emp.get_sex() == Sex::Frog; }, a);
    //DAY SELECTION
    //std::chrono::year_month_day date = read_ymd_input("Введите дату для поиска:");
    //bool sel_flag = arr.select([date](const Employee& emp) {return emp.get_applyment_date() < date and emp.get_fire_date()> date; }, a);
    // Sort
    //arr.print_list();
    //arr.sort_by([](const Employee& a, const Employee& b) { if (a.get_surname() != b.get_surname()) return a.get_surname() < b.get_surname(); return a.get_name() < b.get_name(); },true);
    //std::cout << "\n--------------------------------------\n";
    //arr.print_list();

        //std::cin >> emp;
        //std::cout << emp << std::endl;

        //arr.add(emp);
    //arr.print_list();
    //std::cout << "-----------------------------------------------------\n";
    //std::cout << "Введите сотрудника\n";
    //std::cin >> emp;
    //arr.remove_by_value(emp);
    
    //std::vector<int> a = { 1,2,3,4,5 };



    //(file >> emp);
    //std::cout << emp;

    //Employee emp;
    //while (file >> emp)
    //    std::cout << emp << std::endl;
}