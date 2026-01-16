#include <functional>
#include <iostream>

template<typename T>
class LinkedStack {
private:
    struct Node {
        T data;
        Node* next;
    };

    Node* topNode;

public:
    LinkedStack() {
        topNode = nullptr;
    }

    bool IsEmpty() {
        return topNode == nullptr;
    }

    void Push(T value) {
        Node* n = new Node();
        n->data = value;
        n->next = topNode;
        topNode = n;
    }

    T Pop() {
        if (IsEmpty()) {
            throw "empty stack";
        }
        Node* n = topNode;
        T val = n->data;
        topNode = n->next;
        delete n;
        return val;
    }

    T Peek() {
        if (IsEmpty()) {
            throw "empty stack";
        }
        return topNode->data;
    }

    // Reverse
    LinkedStack<T> Reverse() {
        LinkedStack<T> temp;
        LinkedStack<T> result;

        while (!IsEmpty()) {
            temp.Push(Pop());
        }

        while (!temp.IsEmpty()) {
            T v = temp.Pop();
            result.Push(v);
            Push(v); // restore original
        }

        return result;
    }

    // Where
    LinkedStack<T> Where(std::function<bool(T)> pred) {
        LinkedStack<T> temp;
        LinkedStack<T> result;

        while (!IsEmpty()) {
            T v = Pop();
            if (pred(v)) {
                result.Push(v);
            }
            temp.Push(v);
        }

        while (!temp.IsEmpty()) {
            Push(temp.Pop());
        }

        return result.Reverse();
    }

    // Select
    template<typename R>
    LinkedStack<R> Select(std::function<R(T)> selector) {
        LinkedStack<T> temp;
        LinkedStack<R> result;

        while (!IsEmpty()) {
            T v = Pop();
            result.Push(selector(v));
            temp.Push(v);
        }

        while (!temp.IsEmpty()) {
            Push(temp.Pop());
        }

        return result.Reverse();
    }

    // First
    T First(std::function<bool(T)> pred) {
        LinkedStack<T> temp;
        bool found = false;
        T res;

        while (!IsEmpty()) {
            T v = Pop();
            if (!found && pred(v)) {
                res = v;
                found = true;
            }
            temp.Push(v);
        }

        while (!temp.IsEmpty()) {
            Push(temp.Pop());
        }

        if (!found) throw "not found";
        return res;
    }

    // Contains
    bool Contains(std::function<bool(T)> pred) {
        LinkedStack<T> temp;
        bool ok = false;

        while (!IsEmpty()) {
            T v = Pop();
            if (pred(v)) ok = true;
            temp.Push(v);
        }

        while (!temp.IsEmpty()) {
            Push(temp.Pop());
        }

        return ok;
    }

    // Count
    int Count(std::function<bool(T)> pred) {
        LinkedStack<T> temp;
        int c = 0;

        while (!IsEmpty()) {
            T v = Pop();
            if (pred(v)) c++;
            temp.Push(v);
        }

        while (!temp.IsEmpty()) {
            Push(temp.Pop());
        }

        return c;
    }

    // Max
    T Max(std::function<int(T,T)> comp) {
        LinkedStack<T> temp;

        if (IsEmpty()) throw "empty";

        T maxVal = Pop();
        temp.Push(maxVal);

        while (!IsEmpty()) {
            T v = Pop();
            if (comp(maxVal, v) == -1) {
                maxVal = v;
            }
            temp.Push(v);
        }

        while (!temp.IsEmpty()) {
            Push(temp.Pop());
        }

        return maxVal;
    }
};    

int main() {
    LinkedStack<int> s;
    s.Push(1);
    s.Push(5);
    s.Push(3);
    s.Push(8);

    auto w = s.Where([](int x){ return x > 3; });
    auto sel = s.Select<int>([](int x){ return x + 5; });
    auto rev = s.Reverse();

    std::cout << s.First([](int x){ return x > 4; }) << "\n";
    std::cout << s.Contains([](int x){ return x == 3; }) << "\n";
    std::cout << s.Count([](int x){ return x > 2; }) << "\n";

    std::cout << s.Max([](int a,int b){
        if (b > a) return -1;
        if (a > b) return 1;
        return 0;
    }) << "\n";
}