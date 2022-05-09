#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <mutex>
#include "stack.hpp"
#include <unistd.h>

pthread_mutex_t  mutex_lock = PTHREAD_MUTEX_INITIALIZER;
block_header *head=NULL;

//
void * my_malloc(size_t size){
    pthread_mutex_lock(&mutex_lock);
    void* new_block;
    block_header *iter=head;
    while (iter)
    {
        if (iter->is_free==1&&iter->size>=size)
        {
           new_block=(void*)(ulong)iter + sizeof(block_header);
           iter->is_free=0;
           pthread_mutex_unlock(&mutex_lock);
           return new_block;
        }
        else
            iter=iter->next_block;
        
    }
    iter=(block_header*)sbrk(size + sizeof(block_header));
    iter->is_free=0;
    iter->size=size;
    iter->next_block=head;
    head=iter;
    new_block=(void*)iter + sizeof(block_header);
    pthread_mutex_unlock(&mutex_lock);
    return new_block;
}

void my_free(void *item_to_free){
    pthread_mutex_lock(&mutex_lock);
    block_header* del_block=(block_header*)(item_to_free-sizeof(block_header));
    del_block->is_free=1;
    
    pthread_mutex_unlock(&mutex_lock);


}

stack::stack(){
    node *stack_point = NULL;
    this->head = stack_point;
    this->size = 0;
}
stack::~stack(){
    
}
void stack::PUSH(string &data){
    pthread_mutex_lock(&mutex_lock);
    node *n = new node();
    n->set_data(data);
    this->set_new_head(*n);
    pthread_mutex_unlock(&mutex_lock);

}

string stack::POP(){
    pthread_mutex_lock(&mutex_lock);
    node *temp = this->head;
    this->head = temp->next_node();
    string data = temp->get_data();
    free(temp);
    pthread_mutex_unlock(&mutex_lock);
    return data;
}

string stack::TOP(){
    pthread_mutex_lock(&mutex_lock);
    string top="OUTPUT: "+ this->head->get_data();
    pthread_mutex_unlock(&mutex_lock);
    return top;
}
// node::node(){

//  }
// int main(){
//     string s = "ilan";
//     string s1 = "ilan2";
//     string s2 = "ilan3";
//     stack st;
//     st.PUSH(s);
//     st.PUSH(s1);
//     st.PUSH(s2);
//     string test_top = st.TOP();
//     string ilan = st.POP();
// }