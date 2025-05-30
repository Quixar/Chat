template<typename T>
class Wrapper {
public:
    Wrapper() = default;
    Wrapper(const T& value) : data(value) {}

    T& get() { return data; }
    const T& get() const { return data; }

    void set(const T& value) { data = value; }

private:
    T data;
};
