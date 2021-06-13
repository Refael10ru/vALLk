

namespace vallk::util
{

template<class T>
class ref
{ 
public:
    ref() = delete;

    ref(ref_counter<T>& refCounter) : _refrence(refCounter) 
    { this->refrence->_counter++; }

    ref(ref<T>& oth) : refrence(oth._refrence) 
    { this->refrence->_counter++; }

    ref(ref<T>&& oth) : refrence(oth._refrence)
    { oth._refrence = nullptr }

    ~ref() { if(this->_refrence != nullptr) this->_refrence._counter--; }

    void operator=(ref<T>& oth)
    {
        this->_refrence = oth._refrence;
        this->_refrence->_counter++;
    }
    void operator=(ref<T>&& oth)
    {
        this->_refrence = oth._refrence;
        oth._refrence = nullptr;
    }

    T& operator->()
    {
        return this->_refrence;
    }


    T& operator*()
    {
        return *this->_refrence;
    }
private:
    ref_counter<T>* _refrence;
};

template<class T>
class ref_counter : T
// refrence Counter
{
public:
    uint32_t counter() { return this->_counter;

private:
    friend class ref;

    uint32_t _counter;
}
};


}