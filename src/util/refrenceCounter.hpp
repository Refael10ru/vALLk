

namespace vallk::util
{

template<class T>
class Ref
{
    RefCounter<T>* m_refrence;
public:
    Ref() = delete;

    Ref(RefCounter<T>& refcounter) : m_refrence(refcounter) 
    { this->refrence->m_counter++; }

    Ref(Ref<T>& oth) : refrence(oth.m_refrence) 
    { this->refrence->m_counter++; }

    Ref(Ref<T>&& oth) : refrence(oth.m_refrence)
    { oth.m_refrence = nullptr }

    ~Ref() { if(this->m_refrence != nullptr) this->m_refrence.m_counter--; }


    T& operator*()
    {
        return *this->m_refrence;
    }
};

template<class T>
class RefCounter : T
// refrence Counter
{
    friend class Ref;

    uint32_t m_counter;
public:
    uint32_t counter() { return this->m_counter; }
};


}