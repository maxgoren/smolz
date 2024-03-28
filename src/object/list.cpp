#include "object.hpp"

ListNode* copyList(ListNode* a) {
    ListNode dummy; ListNode *t = &dummy;
    ListNode* it = a;
    while (it != nullptr) {
        ListNode* nn = new ListNode;
        nn->data = it->data;
        nn->next = nullptr;
        t->next = nn;
        t = t->next;
        it = it->next;
    }    
    return dummy.next;
}

ListNode* mergeList(ListNode* a, ListNode* b) {
    ListNode dummy; ListNode *c = &dummy;
    while (a != nullptr && b != nullptr) {
        //thats just ugly.
        if (stof(toString(b->data)) > stof(toString(a->data))) {
            c->next = a; a = a->next; c = c->next;
        } else {
            c->next = b; b = b->next; c = c->next;
        }
    }
    c->next = (a == nullptr) ? b:a;
    return dummy.next;
}

ListNode* mergeSortList(ListNode* h) {
    if (h == nullptr || h->next == nullptr)
        return h;
    ListNode* fast = h->next, *slow = h;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    ListNode* front = h;
    ListNode* back = slow->next;
    slow->next = nullptr;
    return mergeList(mergeSortList(front), mergeSortList(back));
}