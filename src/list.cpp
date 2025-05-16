#include <Arduino.h>

template <typename T>

class Array
{
private:
  T *elements;
  int count;
  int maxSize;

public:
  Array(int size = 20)
  {
    count = 0;
    maxSize = size;
    elements = new T[maxSize];
  }

  ~Array()
  {
    delete[] elements;
  }

  bool addElement(T newElement)
  {
    if (count < maxSize)
    {
      elements[count] = newElement;
      count++;
      return true;
    }
    return false;
  }

  bool removeElement(int index)
  {
    if (index >= 0 && index < count)
    {
      for (int i = index; i < count - 1; i++)
      {
        elements[i] = elements[i + 1];
      }
      count--;
      return true;
    }
    return false;
  }

  void printArray()
  {
    for (int i = 0; i < count; i++)
    {
      Serial.println(elements[i]);
    }
  }

  T getElementByIdx(int index)
  {
    if (index >= 0 && index < count)
    {
      return elements[index];
    }
    return T();
  }

  int getElementCount()
  {
    return count;
  }
};