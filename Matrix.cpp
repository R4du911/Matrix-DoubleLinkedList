#include "Matrix.h"
#include <exception>

using namespace std;

//Best case: θ(1)
//Worst case: θ(1)
//Average case: θ(1)
Matrix::Matrix(int nrLines, int nrCols) {

    this->head = nullptr;
    this->tail = nullptr;
    this->nrlinii = nrLines;
    this->nrcol = nrCols;
}

//Best case: θ(1)
//Worst case: θ(1)
//Average case: θ(1)
int Matrix::nrLines() const {
    return this->nrlinii;
}

//Best case: θ(1)
//Worst case: θ(1)
//Average case: θ(1)
int Matrix::nrColumns() const {
    return this->nrcol;
}

//Best case: θ(1)
//Worst case: θ(n)
//Average case: θ(n)
TElem Matrix::element(int i, int j) const {
    if (i > this->nrlinii || j > this->nrcol || i < 0 || j < 0) {
        throw exception();
    }

    DLLNode *currentnode = this->head;
    if (currentnode == nullptr) {
        return NULL_TELEM;
    }

    while(currentnode != nullptr){
        if(std::get<0>(currentnode->info) == i && std::get<1>(currentnode->info) == j)
            return std::get<2>(currentnode->info);
        currentnode = currentnode->next;
    }

    return NULL_TELEM;
}

//Best case: θ(1)
//Worst case: θ(n)
//Average case: θ(n)
TElem Matrix::modify(int i, int j, TElem e) {
    if (i > this->nrlinii || j > this->nrcol || i < 0 || j < 0)
        throw exception();

    ///checks if list is empty and value of element to be added is 0 -> no need to add an element
    if (e == NULL_TELEM && this->head == nullptr) {
        return NULL_TELEM;
    }

    ///adds only if value to be modified to is not 0
    if (e != NULL_TELEM) {
        ///list is empty
        if (this->head == nullptr) {
            DLLNode *newnode = new DLLNode;
            newnode->info = make_tuple(i, j, e);
            newnode->next = nullptr;
            newnode->previous = nullptr;
            this->head = newnode;
            this->tail = newnode;
            return NULL_TELEM;
        }

        ///add element at the beginning
        if (std::get<0>(this->head->info) > i || (std::get<0>(this->head->info) == i && std::get<1>(this->head->info) > j)) {
            DLLNode *currentnode = new DLLNode;
            currentnode->next = this->head;
            currentnode->previous = nullptr;
            currentnode->info = make_tuple(i, j, e);
            this->head->previous = currentnode;
            this->head = currentnode;
            return NULL_TELEM;
        }

        ///add element at the end
        if (std::get<0>(this->tail->info) < i || (std::get<0>(this->tail->info) == i && std::get<1>(this->tail->info) < j)) {
            DLLNode *currentnode = new DLLNode;
            currentnode->previous = this->tail;
            currentnode->info = make_tuple(i, j, e);
            currentnode->next = nullptr;
            this->tail->next = currentnode;
            this->tail = currentnode;
            return NULL_TELEM;
        }
    }

    ///goes through the list and stops at the element if found
    DLLNode *currentnode = this->head;
    while (currentnode != nullptr) {
        if (std::get<0>(currentnode->info) == i && std::get<1>(currentnode->info) == j) {
            ///OLD_VALUE!=0 and NEW_VALUE=0 => remove elem
            if (e == NULL_TELEM) {
                if (currentnode == this->head) {
                    TElem old = std::get<2>(currentnode->info);
                    currentnode->next->previous = nullptr;
                    this->head = currentnode->next;
                    delete currentnode;
                    return old;
                }
                else if (currentnode == this->tail) {
                    TElem old = std::get<2>(currentnode->info);
                    currentnode->previous->next = nullptr;
                    this->tail = currentnode->previous;
                    delete currentnode;
                    return old;
                }
                else {
                    TElem old = std::get<2>(currentnode->info);
                    currentnode->previous->next = currentnode->next;
                    currentnode->next->previous = currentnode->previous;
                    delete currentnode;
                    return old;
                }
            }
            ///OLD_VALUE!=0 and NEW_VALUE!=0 => modify
            else {
                TElem old = std::get<2>(currentnode->info);
                currentnode->info = make_tuple(i, j, e);
                return old;
            }
        }
        currentnode = currentnode->next;
    }

    ///goes through the list and stops before the position where element should be added
    if (e != NULL_TELEM) {

        currentnode = this->head;
        while (currentnode->next != nullptr && ((get<0>(currentnode->info) == i && get<1>(currentnode->info) > j) || get<0>(currentnode->info) > i)){
            currentnode = currentnode->next;
        }

        ///adds element in list
        DLLNode *newnode = new DLLNode;
        newnode->info = make_tuple(i, j, e);
        newnode->previous = currentnode;
        newnode->next = currentnode->next;
        currentnode->next->previous = newnode;
        currentnode->next = newnode;
        return NULL_TELEM;

    }
    return NULL_TELEM;
}


//Best case: θ(n)
//Worst case: θ(n)
//Average case: θ(n)
void Matrix::resize_matrix(int nrlines, int nrcols) {
    if(nrlines >= this->nrlinii && nrcols >= this->nrcol) {
        this->nrlinii = nrlines;
        this->nrcol = nrcols;
    }
    else {
        DLLNode *currentnode = this->head;
        while (currentnode != nullptr) {
            DLLNode *remove_nod;
            if (get<1>(currentnode->info) >= nrcols || get<0>(currentnode->info) >= nrlines) {
                if(currentnode == this->head){
                    currentnode->next->previous = nullptr;
                    currentnode->next = nullptr;
                }
                else if (currentnode == this->tail) {
                    currentnode->previous->next = nullptr;
                    currentnode->previous = nullptr;
                } else {
                    remove_nod = currentnode;
                    currentnode = currentnode->previous;
                    remove_nod->previous->next = remove_nod->next;
                    remove_nod->next->previous = remove_nod->previous;
                    remove_nod->next = nullptr;
                    remove_nod->previous = nullptr;
                }

            }
            currentnode = currentnode->next;
        }
    }
}

Matrix::~Matrix() {
    if (this->head != nullptr) {
        while (this->head->next != nullptr) {
            DLLNode *node = this->head;
            this->head = this->head->next;
            delete node;
        }
        delete this->head;
    } else {
        delete this->head;
    }
}