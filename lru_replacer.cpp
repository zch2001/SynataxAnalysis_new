/**
 * LRU implementation
 */
#include "buffer/lru_replacer.h"
#include "page/page.h"

namespace scudb {

    template <typename T> LRUReplacer<T>::LRUReplacer() : size_c(0) {
        head = new node();
        tail = head;
    }

    template <typename T> LRUReplacer<T>::~LRUReplacer() = default;

/*
 * Insert value into LRU
 */
    template <typename T> void LRUReplacer<T>::Insert(const T &value) {
        std::lock_guard<std::mutex> lock(mutex_w);
        
        auto it = tables.find(value);
        // ��ʱ������û�и�ҳ�棬��ҳ������β���´���ת�Ӵ�����ʼ��
        if (it == tables.end()) {
            tail->next = new node(value, tail);
            tail = tail->next;
            tables.emplace(value, tail);
            size_c++;
        }
        //ҳ���Ѿ��ڶ����У���Ϊ����ٴη��ʵ����轫���ŵ���β
        else {
            if (it->second != tail) {
                // �ȴ�ԭλ���Ƴ�
                node* pre = it->second->pre;
                node* cur = pre->next;
                pre->next = std::move(cur->next);
                pre->next->pre = pre;

                // �ٷŵ�β��
                cur->pre = tail;
                tail->next = std::move(cur);
                tail = tail->next;
            }
        }
    }

/* If LRU is non-empty, pop the head member from LRU to argument "value", and
 * return true. If LRU is empty, return false
 */
    template <typename T> bool LRUReplacer<T>::Victim(T& value) {
        std::lock_guard<std::mutex> lock(mutex_w);
        //�������Ϊ�շ���false
        if (size_c == 0) {
            return false;
        }
        //����ɾ��������δ��ʹ�õ�ҳ��
        value = head->next->data;
        head->next = head->next->next;
        if (head->next != nullptr) {
            head->next->pre = head;
        }

        tables.erase(value);
        if (--size_c == 0) {
            tail = head;
        }

        return true;
    }

/*
 * Remove value from LRU. If removal is successful, return true, otherwise
 * return false
 */
    template <typename T> bool LRUReplacer<T>::Erase(const T& value) {
        std::lock_guard<std::mutex> lock(mutex_w);
        // ɾ������������λ��ҳ��
        auto it = tables.find(value);
        if (it != tables.end()) {
            // ���ɾ��ҳ�����м�����Ҫ���²���
            if (it->second != tail) {
                node* pre = it->second->pre;
                node* cur = pre->next;
                pre->next = std::move(cur->next);
                pre->next->pre = pre;
            }
            //����ֱ���ڶ�βɾ��
            else {
                tail = tail->pre;
                delete tail->next;
            }
            //�������ɾ����ͷβָ��ָ��һ��
            tables.erase(value);
            if (--size_c == 0) {
                tail = head;
            }
            return true;
        }

        return false;
    }

    template <typename T> size_t LRUReplacer<T>::Size() {
        std::lock_guard<std::mutex> lock(mutex_w);
        return size_c;
    }

template class LRUReplacer<Page *>;
// test only
template class LRUReplacer<int>;

} // namespace scudb
