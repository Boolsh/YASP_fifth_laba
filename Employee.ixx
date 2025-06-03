export module Employee;

import <iostream>;
import <string>;
import <fstream>;
import <chrono>;



export enum class Posts { Cleaner, Cook, Manager, Developer, HR, Analyst };
export enum class Sex { Male, Female, Frog };

bool read_enum(std::istream& in, Posts& patronymic);
bool read_enum(std::istream& in, Sex& patronymic);

//export bool operator<(const std::chrono::year_month_day& lhs, const std::chrono::year_month_day& rhs) {
//  if (lhs.year() != rhs.year()) return lhs.year() < rhs.year();
//  if (lhs.month() != rhs.month()) return lhs.month() < rhs.month();
//  return lhs.day() < rhs.day();
//}
//
//export bool operator>(const std::chrono::year_month_day& lhs, const std::chrono::year_month_day& rhs) {
//  return rhs < lhs;
//}
//
//export bool operator<=(const std::chrono::year_month_day& lhs, const std::chrono::year_month_day& rhs) {
//  return !(lhs > rhs);
//}
//
//export bool operator>=(const std::chrono::year_month_day& lhs, const std::chrono::year_month_day& rhs) {
//  return !(lhs < rhs);
//}

#pragma once
export class Employee
{
private:
    std::string name, surname, patronymic;
    std::chrono::year_month_day applyment_date;
    std::optional<std::chrono::year_month_day> fire_date;
    Posts post{};
    Sex sex{};
    int salary{};
public:

    Employee() {};

    std::string get_name() const { return name; };
    std::string get_surname() const { return surname; };
    std::string get_patronimic() const { return patronymic; };
    std::chrono::year_month_day get_applyment_date() const { return applyment_date; };
    std::optional<std::chrono::year_month_day> get_fire_date() const { return fire_date; };
    Posts get_post() const { return post; };
    Sex get_sex() const { return sex; };
    int get_salary() const { return salary; };

    void set_name(std::string _name) { name = _name; };
    void set_surname(std::string _surname) { surname = _surname; };
    void set_patronymic(std::string _patronymic) { patronymic = _patronymic; };
    void set_applyment_date(std::chrono::year_month_day y) { applyment_date = y; };
    void set_fire_date(std::optional<std::chrono::year_month_day> y) { fire_date = y; };
    void set_post(std::string y)
    {
        if (y == "Manager")
            post = Posts::Manager;
        if (y == "Analyst")
            post = Posts::Analyst;
        if (y == "HR")
            post = Posts::HR;
        if (y == "Cook")
            post = Posts::Cook;
        if (y == "Cleaner")
            post = Posts::Cleaner;
        if (y == "Developer")
            post = Posts::Developer;
    };
    void set_sex(std::string y)
    {
        if (y == "Male")
            sex = Sex::Male;
        if (y == "Female")
            sex = Sex::Female;
        if (y == "Frog")
            sex = Sex::Frog;
    };
    void set_salary(int y) { salary = y; };



    friend std::ostream& operator<<(std::ostream& out, const Employee& emp)
    {
        out << emp.name << '\n'
            << emp.surname << '\n'
            << emp.patronymic << '\n'
            << static_cast<int>(emp.applyment_date.year()) << '-'
            << static_cast<unsigned>(emp.applyment_date.month()) << '-'
            << static_cast<unsigned>(emp.applyment_date.day()) << '\n';

        // Вывод даты увольнения или "-"
        if (emp.fire_date.has_value()) {
            out << static_cast<int>(emp.fire_date.value().year()) << '-'
                << static_cast<unsigned>(emp.fire_date.value().month()) << '-'
                << static_cast<unsigned>(emp.fire_date.value().day()) << '\n';
        }
        else {
            out << "-\n";
        }

        // Вывод должности
        switch (emp.post) {
        case Posts::Analyst:    out << "Analyst\n"; break;
        case Posts::Developer:  out << "Developer\n"; break;
        case Posts::HR:        out << "HR\n"; break;
        case Posts::Manager:    out << "Manager\n"; break;
        case Posts::Cook:       out << "Cook\n"; break;
        case Posts::Cleaner:    out << "Cleaner\n"; break;
        }

        // Вывод пола
        switch (emp.sex) {
        case Sex::Female: out << "Female\n"; break;
        case Sex::Male:   out << "Male\n"; break;
        case Sex::Frog:    out << "Frog\n"; break;
        }

        out << emp.salary << '\n';
        return out;
    }
        friend std::istream& operator>>(std::istream& in, Employee& emp) {
        Employee tmp;
        std::string input;

        auto clear_istream = [](std::istream& stream) {
            stream.clear();
            stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            };

        // Чтение имени
        while (true) {
            if (!std::getline(in >> std::ws, tmp.name)) {
                if (in.eof()) return in;
                clear_istream(in);
                std::cout << "Ошибка: неверный ввод имени\n";
                continue;
            }
            if (!tmp.name.empty()) break;
            std::cout << "Ошибка: имя не может быть пустым\n";
        }

        // Чтение фамилии
        while (true) {
            if (!std::getline(in >> std::ws, tmp.surname)) {
                if (in.eof()) return in;
                clear_istream(in);
                std::cout << "Ошибка: неверный ввод фамилии\n";
                continue;
            }
            if (!tmp.surname.empty()) break;
            std::cout << "Ошибка: фамилия не может быть пустой\n";
        }

        std::getline(in >> std::ws, tmp.patronymic);

        // Чтение даты приема
        while (true) {
            std::string date_str;
            if (!std::getline(in >> std::ws, date_str)) {
                if (in.eof()) return in;
                clear_istream(in);
                std::cout << "Ошибка: неверный формат даты\n";
                continue;
            }

            std::istringstream date_stream(date_str);
            std::chrono::from_stream(date_stream, "%F", tmp.applyment_date);

            if (!date_stream.fail() && tmp.applyment_date.ok()) break;
            std::cout << "Ошибка: некорректная дата. Используйте формат ГГГГ-ММ-ДД\n";
        }

        // Чтение даты увольнения
        while (true) {
            std::string fire_date_str;
            if (!std::getline(in >> std::ws, fire_date_str)) {
                if (in.eof()) return in;
                clear_istream(in);
                std::cout << "Ошибка: неверный формат даты\n";
                continue;
            }

            if (fire_date_str == "-") {
                tmp.fire_date = std::nullopt;
                break;
            }

            std::istringstream fire_date_stream(fire_date_str);
            std::chrono::year_month_day fire_date;
            std::chrono::from_stream(fire_date_stream, "%F", fire_date);

            if (!fire_date_stream.fail() && fire_date.ok()) {
                tmp.fire_date = fire_date;
                break;
            }
            std::cout << "Ошибка: некорректная дата. Используйте формат ГГГГ-ММ-ДД или -\n";
        }

        // Чтение должности
        while (true) {
            if (!read_enum(in >> std::ws, tmp.post)) {
                if (in.eof()) return in;
                clear_istream(in);
                std::cout << "Ошибка: неверная должность. Допустимые значения: ";
                std::cout << "Cleaner, Cook, Manager, Developer, HR, Analyst\n";
                continue;
            }
            break;
        }

        // Чтение пола
        while (true) {
            if (!read_enum(in >> std::ws, tmp.sex)) {
                if (in.eof()) return in;
                clear_istream(in);
                std::cout << "Ошибка: неверный пол. Допустимые значения: Male, Female, Frog\n";
                continue;
            }
            break;
        }

        // Чтение зарплаты
        while (true) {
            if (!(in >> tmp.salary)) {
                if (in.eof()) return in;
                clear_istream(in);
                std::cout << "Ошибка: зарплата должна быть числом\n";
                continue;
            }
            if (tmp.salary >= 0) break;
            std::cout << "Ошибка: зарплата не может быть отрицательной\n";
        }

        // Очистка буфера
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        emp = tmp;
        return in;
    }

    friend bool operator==(const Employee& a, const Employee& b)
    {
        return
            a.name == b.name &&
            a.surname == b.surname &&
            a.patronymic == b.patronymic &&
            a.applyment_date == b.applyment_date &&
            a.fire_date == b.fire_date &&
            a.post == b.post &&
            a.sex == b.sex &&
            a.salary == b.salary; ;
    }

    friend bool operator!=(const Employee& a, const Employee& b)
    {
        return (a.name != b.name ||
            a.surname != b.surname ||
            a.patronymic != b.patronymic ||
            a.applyment_date != b.applyment_date ||
            a.fire_date == b.fire_date ||
            a.post == b.post ||
            a.sex == b.sex ||
            a.salary == b.salary);
    }
};
bool read_enum(std::istream& in, Posts& patronymic)
{
    std::string str{};
    in >> str;
    if (str == "Manager")
    {
        patronymic = Posts::Manager;
        return true;
    }
    if (str == "HR")
    {
        patronymic = Posts::HR;
        return true;
    }
    if (str == "Cleaner")
    {
        patronymic = Posts::Cleaner;
        return true;
    }
    if (str == "Cook")
    {
        patronymic = Posts::Cook;
        return true;
    }
    if (str == "Analyst")
    {
        patronymic = Posts::Analyst;
        return true;
    }
    if (str == "Developer")
    {
        patronymic = Posts::Developer;
        return true;
    }
    return false;
}

bool read_enum(std::istream& in, Sex& patronymic)
{
    std::string str{};
    in >> str;
    if (str == "Male")
    {
        patronymic = Sex::Male;
        return true;
    }
    if (str == "Female")
    {
        patronymic = Sex::Female;
        return true;
    }
    if (str == "Frog")
    {
        patronymic = Sex::Frog;
        return true;
    }
    return false;
}