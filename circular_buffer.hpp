#include <vector>

template <typename T>
class CircularBuffer
{
public:
  CircularBuffer(size_t buffer_size)
  {
    this->buffer_size = buffer_size;
    buffer.resize(buffer_size);
    head = 0;
    tail = 0;
    empty = true
    full = false;
  }

  void put(T element)
  {
    if (!empty)
      head = next_index(head);

    if (head == 0)
      full = true;

    if (full)
      tail = head + 1;

    buffer[head] = element;
    empty = false;
  }

  T last()
  {
    return buffer[head];
  }
  
  vector<T> lastN(int element_count)
  {
    int first = (head - element_count + 1);
    std::vector<T> elements;
    for (int i = first; i != next_index(head); i = next_index(i))
    {
      elements.push_back(buffer[i])  
    }
    return elements;
  }

private:
  size_t next_index(size_t index)
  {
    return (index + 1) % buffer_size;
  }

private:
  std::vector<T> buffer;
  size_t head;
  size_t tail;
  size_t buffer_size;
  bool empty;
  bool full;
};