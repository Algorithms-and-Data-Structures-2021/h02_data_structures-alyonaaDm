#include "linked_list.hpp"

#include <cassert>    // assert
#include <stdexcept>  // out_of_range

#include "private/internal.hpp"  // это не тот приват, о котором вы могли подумать

namespace itis {

    void LinkedList::Add(Element e) {
        // Tip 1: создайте узел в куче со переданным значением
        // Tip 2: есть 2 случая - список пустой и непустой
        // Tip 3: не забудьте обновить поля head и tail
        // напишите свой код здесь ...

        Node *node = new Node(e, nullptr);

        if (head_ == nullptr) {
            head_ = node;
            tail_ = node;
        } else {
            tail_->next = node;
            tail_ = node;
        }
        size_++;
    }

    void LinkedList::Insert(int index, Element e) {
        internal::check_out_of_range(index, 0, size_ + 1);

        // Tip 1: вставка элементов на позицию size эквивалентно операции добавления в конец
        // Tip 2: рассмотрите несколько случаев:
        //        (1) список пустой,
        //        (2) добавляем в начало списка,
        //        (3) добавляем в конец списка
        //        (4) все остальное

        // напишите свой код здесь ...
        if (head_ == nullptr) {
            Node *node = new Node(e, nullptr);
            head_ = node;
            tail_ = node;
        } else if (index == 0) {
            Node *node = new Node(e, head_);
            head_ = node;
        } else if (index == size_) {
            Node *node = new Node(e, nullptr);
            tail_->next = node;
            tail_ = node;
        } else {
            Node *prenode = find_node(index - 1);
            Node *node = new Node(e, prenode->next);
            prenode->next = node;
        }
        size_++;
    }

    void LinkedList::Set(int index, Element e) {
        internal::check_out_of_range(index, 0, size_);
        // Tip 1: используйте функцию find_node(index)
        // напишите свой код здесь ...
        find_node(index)->data = e;
    }

    Element LinkedList::Remove(int index) {
        internal::check_out_of_range(index, 0, size_);
        // Tip 1: рассмотрите случай, когда удаляется элемент в начале списка
        // Tip 2: используйте функцию find_node(index)
        // напишите свой код здесь ...
        Element ans = Element::UNINITIALIZED;
        if (index == 0) {
            if (size_ == 1) {
                ans = head_->data;
                delete head_;
                head_ = nullptr;
                tail_ = nullptr;

            } else {
                Node *node = head_->next;
                ans = head_->data;
                delete head_;
                head_ = node;
            }
        } else if (index == size_ - 1) {
            tail_ = find_node(index - 1);
            ans = tail_->next->data;
            delete tail_->next;
            tail_->next = nullptr;
        } else {
            Node *node = find_node(index - 1);
            Node *noDE = node->next;
            node->next = noDE->next;
            ans = noDE->data;
            delete noDE;;
        }
        size_--;
        return ans;
    }

    void LinkedList::Clear() {
        // Tip 1: люди в черном (MIB) пришли стереть вам память
        // напишите свой код здесь ...
        int size = size_;
        for (int i = 0; i < size; i++) {
            Node *ans = head_->next;
            delete head_;
            head_ = ans;
            size_--;
        }
        tail_ = nullptr;
    }

    Element LinkedList::Get(int index) const {
        internal::check_out_of_range(index, 0, size_);
        // напишите свой код здесь ...
        return find_node(index)->data;
    }

    int LinkedList::IndexOf(Element e) const {
        // напишите свой код здесь ...
        Node *first = head_;
        int counter = 0;
        while (first != nullptr) {
            if (first->data == e) {
                return counter;
            }
            counter++;
            first = first->next;
        }
        return -1;
    }

    Node *LinkedList::find_node(int index) const {
        assert(index >= 0 && index < size_);
        // Tip 1: можете сразу обработать случаи поиска начала и конца списка
        // напишите свой код здесь ...
        int counter = 0;
        for (Node *current_node = head_; current_node != nullptr; current_node = current_node->next) {
            if (counter == index) return current_node;
            counter++;
        }
        return {};
    }

// РЕАЛИЗОВАНО

    LinkedList::~LinkedList() {
        Clear();
    }

    bool LinkedList::Contains(Element e) const {
        // если индекс не найден, значит и элемента нет
        return IndexOf(e) == kNotFoundElementIndex;
    }

    int LinkedList::GetSize() const {
        return size_;
    }

    bool LinkedList::IsEmpty() const {
        return size_ == 0;
    }

    Element LinkedList::tail() const {
        // вместо выброса ошибки в случае nullptr, римским парламентов было решено возвращать "специальное" значение
        return tail_ ? tail_->data : Element::UNINITIALIZED;
    }

    Element LinkedList::head() const {
        return head_ ? head_->data : Element::UNINITIALIZED;
    }

// === RESTRICTED AREA: необходимо для тестирования ===

    LinkedList::LinkedList(
            const std::vector<Element> &elements) {
        assert(!elements.empty());

        size_ = elements.size();
        head_ = new Node(elements[0], nullptr);

        auto current_node = head_;

        for (int index = 1; index < static_cast<int>(elements.size()); index++) {
            current_node->next = new Node(elements[index], nullptr);
            current_node = current_node->next;
        }
        tail_ = current_node;
    }

    std::ostream &operator<<(std::ostream &os, const LinkedList &list) {
        if (list.head_ != nullptr && list.tail_ != nullptr) {
            os << "{ ";
            for (auto current_node = list.head_; current_node != list.tail_; current_node = current_node->next) {
                os << internal::elem_to_str(current_node->data) << ", ";
            }
            os << internal::elem_to_str(list.tail_->data) << " }";
        } else {
            os << "{ nullptr }";
        }
        return os;
    }

    bool operator==(const LinkedList &list, const std::vector<Element> &elements) {
        if (list.size_ != static_cast<int>(elements.size())) return false;
        Node *current_node = list.head_;

        for (const auto e : elements) {
            if (current_node == nullptr) return false;
            if (current_node->data != e) return false;
            current_node = current_node->next;
        }
        return true;
    }

}  // namespace itis