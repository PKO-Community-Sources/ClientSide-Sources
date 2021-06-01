
#ifndef _FIFO_H_
#define _FIFO_H_

struct fifo_elem
    {
    fifo_elem() {prev = next = NULL;}
    ~fifo_elem() {prev = next = NULL;}

    fifo_elem* prev;
    fifo_elem* next;};

template <class T>
class fifo
    {
public:
    fifo() {_head = _tail = NULL;}
    ~fifo() {_head = _tail = NULL;}

    void push(T* t)
        { // ��β��ѹ��һ��Ԫ��
        if (t == NULL) return;
        if (_head == NULL)
            { // fifoΪ��
            _head = t;
            _head->prev = NULL;
            _head->next = NULL;
            _tail = _head;}
        else{ // fifo�ǿ�
            _tail->next = t;
            t->prev = _tail;
            t->next = NULL;
            _tail = t;}}

    T* pop()
        { // ��ͷ��ȡ��һ��Ԫ��
        T* tmp;
        if (_head == _tail)
            {
            if (_head == NULL) return NULL; // fifoΪ��
            else{ // fifoʣ��һ��Ԫ��
                tmp = _head;
                _head = _tail = NULL;
                return tmp;}}
        else{ // fifo��ʣ�����һ��Ԫ�أ�ȡ��һ��
            tmp = _head;
            _head = (T *)_head->next;
            _head->prev = NULL;
            return tmp;}}

    int size() const
        { // ȡ��fifo�ĳ���
        int sz = 0;
        T* curr = _head;
        if (curr == NULL) return sz; // fifoΪ��
        while (curr != _tail)
            { // fifo�ǿ�
            ++ sz; curr = (T *)curr->next;}
        return (sz + 1);}

protected:
    T* _head;
    T* _tail;};

#endif // _FIFO_H_
