export module VectorModule;

import <vector>;
import <functional>;
import <iostream>;
import <fstream>;
import <string>;
import <iterator>;
import <algorithm>;

export template <class T>
class Vector
{
private:
    std::vector<T> arr;
public:
    Vector() = default;

    Vector(std::string filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка открытия файла\n";
            return;
        }

        T emp;
        while (file >> emp) {
            arr.push_back((emp));
        }
    }

    bool empty() { return arr.empty(); }


    void fill_list(std::istream& stream)
    {
        std::copy(std::istream_iterator<T>(stream), std::istream_iterator<T>(), std::back_inserter(arr));
    }

    void print_list(std::ostream& out = std::cout)
    {
        out << "--------------------------------------------\n";
        std::copy(arr.begin(), arr.end(), std::ostream_iterator<T>(out, "\n"));
        out << "--------------------------------------------\n";
    }

    void add(T& emp)
    {
        arr.push_back(emp);
    }

    bool remove_by_value(const T& emp)
    {
        auto it = std::find(arr.cbegin(), arr.cend(), emp);
        if (it != arr.end())
        {
            arr.erase(it);
            return true;
        }
        return false;
    }

    bool remove_by_ind(size_t i)
    {
        if (i < arr.size())
        {
            arr.erase(arr.cbegin() + i);
            return true;
        }
        return false;
    }

    bool modify(size_t index, const T& new_value)
    {
        if (index < arr.size())
        {
            arr[index] = new_value;
            return true;
        }
        return false;
    }

    bool select(std::function<bool(const T&)> predicate, Vector& result)
    {
        std::copy_if(arr.begin(), arr.end(), std::back_inserter(result.arr), predicate);
        return !result.empty();
    }

    void sort_by(std::function<bool(const T&, const T&)> comparator, bool ascending = true)
    {
        if (ascending) {
            std::sort(arr.begin(), arr.end(), comparator);
        }
        else {
            std::sort(arr.begin(), arr.end(), [&comparator](const T& a, const T& b) { return !comparator(a, b); });
        }
    }

    size_t size() const { return arr.size(); }
    T& operator[](size_t index) { return arr[index]; }
    const T& operator[](size_t index) const { return arr[index]; }

};