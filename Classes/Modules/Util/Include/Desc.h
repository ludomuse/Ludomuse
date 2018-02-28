#ifndef _DESC_H_
#define _DESC_H_


template <typename T>
class Desc {
  T** m_pPointer;
  int* m_iRefCount;

 public:
  Desc(T* a_pPointer = nullptr)
  {
    m_pPointer = new T*();
    m_iRefCount = new int();

    *m_pPointer = a_pPointer;
    *m_iRefCount = 1;
  }


  Desc(const Desc& m_rCopyFrom)
  {
    m_pPointer = m_rCopyFrom.m_pPointer;
    m_iRefCount = m_rCopyFrom.m_iRefCount;

    ++(*m_iRefCount);
  }

  ~Desc()
  {
    --(*m_iRefCount);
    if (*m_iRefCount <= 0)
    {
      delete m_pPointer;
      delete m_iRefCount;
    }
  }


  void Set(T* a_pNewValue)
  {
    *m_pPointer = a_pNewValue;
  }

  T* Get()
  {
    return *m_pPointer;
  }


  bool IsValid()
  {
    return *m_pPointer;
  }
};

#endif /* _DESC_H_ */
