#ifndef THREAD_SAFE_LIST_H_
#define THREAD_SAFE_LIST_H_

#include <pthread.h>
#include <iostream>
#include <iomanip> // std::setw
#include <stdlib.h>

#define SUCCESS 0
using namespace std;

template <typename T>
class List
{
    public:
        /**
         * Constructor
         */
        List() : head(NULL), size(0) , list_mutex(){
            if (pthread_mutex_init(&list_mutex, NULL)!=SUCCESS){
                std::cerr<<"pthread_mutex_init: failed"<<std::endl;
                exit(-1);
            }
            head = new Node(T());
        }

        /**
         * Destructor
         */
        ~List(){
            Node* current=head;
            Node* next=nullptr;
            while (current){
                next=current->next;
                delete current;
                current=next;
            }
            head=NULL;
            pthread_mutex_destroy(&list_mutex);
        }

        class Node {
         public:
            explicit Node(const T& data) : data(data), next(NULL), prev(NULL)
            , node_mutex(){
                if (pthread_mutex_init(&node_mutex, NULL)!=SUCCESS) {
                    std::cerr << "pthread_mutex_init: failed" << std::endl;
                    exit(-1);
                }
            }
            ~Node(){
                pthread_mutex_destroy(&node_mutex);
            }

            void lock() {
                pthread_mutex_lock(&node_mutex);
            }
            void unlock() {
                pthread_mutex_unlock(&node_mutex);
            }
            T data;
            Node *next;
            Node *prev;
            pthread_mutex_t node_mutex;
        };

        /**
         * Insert new node to list while keeping the list ordered in an ascending order
         * If there is already a node has the same data as @param data then return false (without adding it again)
         * @param data the new data to be added to the list
         * @return true if a new node was added and false otherwise
         */
        bool insert(const T& data) {
            head->lock(); // lock dummy node
            Node *pred = head;
            Node *succ = head->next;
            if (!succ) { // insert first node
                insertAux(data, pred, succ);
                return true;
            }
            succ->lock();

            while (succ) {
                if (data == succ->data) {
                    succ->unlock();
                    pred->unlock();
                    return false; // found identical item in list
                }
                if (data < succ->data) {// insert in the middle of the list
                    insertAux(data, pred, succ);
                    return true;
                }
                lockAndAdvance(&pred, &succ);
            }

            insertAux(data, pred, succ); // insert in the end of the list
            return true;
        }


        /**
         * Remove the node that its data equals to @param value
         * @param value the data to lookup a node that has the same data to be removed
         * @return true if a matched node was found and removed and false otherwise
         */
        bool remove(const T& value) {
            head->lock(); // lock dummy node
            Node *pred = head;
            Node *curr = pred->next;
            if (!curr) { // list empty
                head->unlock();
                return false;
            }
            curr->lock();
            while (curr->data <= value) {
                if (curr->data == value) {
                    removeNodeAux(pred, curr);
                    return true; // remove succeeded
                }
                lockAndAdvance(&pred, &curr);
                if (!curr) {
                    break;
                }
            }
            if (curr) {
                curr->unlock();
            }
            pred->unlock();
            return false; // node not found
        }


        /**
         * Returns the current size of the list
         * @return the list size
         */
        unsigned int getSize() {
			return size;
        }

		// Don't remove
        void print() {
          pthread_mutex_lock(&list_mutex);
          Node* temp = head->next; //start print after dummy
          if (temp == NULL)
          {
            cout << "";
          }
          else if (temp->next == NULL)
          {
            cout << temp->data;
          }
          else
          {
            while (temp != NULL)
            {
              cout << right << setw(3) << temp->data;
              temp = temp->next;
              cout << " ";
            }
          }
          cout << endl;
          pthread_mutex_unlock(&list_mutex);
        }

		// Don't remove
        virtual void __add_hook() {}
		// Don't remove
        virtual void __remove_hook() {}


    private:
    void lockAndAdvance(Node **pred, Node **succ) {
        (*pred)->unlock();
        *pred = *succ;
        *succ = (*succ)->next;
        if(*succ) {
            (*succ)->lock();
        }
    }

    void insertAux(const T& data, Node *pred, Node *succ) {
        Node *new_node = new Node(data);
        pred->next = new_node; //pred always exists
        new_node->prev = pred;
        new_node->next = succ;
        if(succ) {
            succ->prev = new_node;
        }
        __add_hook();
        if(succ) {
            succ->unlock();
        }
        pred->unlock(); // pred always exists
        pthread_mutex_lock(&list_mutex);
        size++;
        pthread_mutex_unlock(&list_mutex);
    }

    void removeNodeAux(Node* pred, Node* curr){
        pred->next = curr->next;
        if (curr->next) {
            curr->next->prev = pred;
        }
        __remove_hook();
        curr->unlock();
        pred->unlock();
        delete curr;
        pthread_mutex_lock(&list_mutex);
        size--;
        pthread_mutex_unlock(&list_mutex);
    }

    Node* head;
    unsigned int size;
    pthread_mutex_t list_mutex;

};

#endif //THREAD_SAFE_LIST_H_