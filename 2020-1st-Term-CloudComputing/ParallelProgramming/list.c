//
//  list.c
//  Assignment
//
//  Created by User on 31/03/2020.
//  Copyright © 2020 User. All rights reserved.
//

#include "list.h"

/* default compare function */
int compare(void *k1, void *k2) {
    long key1 = *((long*)k1);
    long key2 = *((long*)k2);
    
    if (key1 == key2) return 0;
    else if (key1 < key2) return 1;
    else return -1;
}


void init_btree(struct B_TREE_T *tree, int degree) {
    tree->degree = degree;
    tree->depth = 0;
    tree->cmp = &compare;
    
    struct B_TREE_NODE_T *root = (struct B_TREE_NODE_T*) malloc(sizeof(struct B_TREE_NODE_T));
    if (root == 0) {
        perror("init_btree(): couldn't allocated.");
        exit(-1);
    }
    tree->root = &root->head;
    root->elements = (struct LIST_HEAD_T*) malloc(sizeof(struct LIST_HEAD_T));
    init_list_head(root->elements);
    init_btree_head(tree->root, degree);
}


void init_btree_head(struct B_TREE_HEAD_T *head, int degree) {
    head->n = 0;
    head->leaf = TRUE;
    head->parent = 0;
    head->childs = (struct B_TREE_HEAD_T**) malloc(sizeof(struct B_TREE_HEAD_T*) * (degree+1));
    
    if (head->childs == 0) {
        perror("init_btree_head(): couldn't allocated.");
        exit(-1);
    }
    
    memset(head->childs, 0, sizeof(struct B_TREE_HEAD_T*) * (degree+1));
}


void init_list_head(struct LIST_HEAD_T *head) {
    head->prev = 0;
    head->next = 0;
}


/*
 parent_el_idx: the index of parent element.
 head: the head ptr of node to be divided.
 tree: ptr of btree.
*/
void btree_divide(int parent_el_idx,
                  struct B_TREE_HEAD_T *p_head,
                  struct B_TREE_T *tree)
{
    int i;
    int t = ceil(tree->degree/2);
    struct B_TREE_NODE_T *node = list_entry(p_head, struct B_TREE_NODE_T, head);
    struct LIST_HEAD_T *target = node->elements;
    struct B_TREE_NODE_T *pr_node;
    struct LIST_HEAD_T *pr_el_head;
    struct B_TREE_NODE_T *left;
    struct B_TREE_NODE_T *right;
    for (i=0; i<t; i++) target = target->next;
    
    if (p_head->parent == 0) { // 루트노드인 경우
        left = (struct B_TREE_NODE_T*) malloc(sizeof(struct B_TREE_NODE_T));
        right = (struct B_TREE_NODE_T*) malloc(sizeof(struct B_TREE_NODE_T));
        if (left == 0 || right == 0) {
            perror("btree_divide(): left or right not allocated.");
            exit(-1);
        }
        
        init_btree_head(&left->head, tree->degree);
        left->elements = (struct LIST_HEAD_T*) malloc(sizeof(struct LIST_HEAD_T));
        if (left->elements == 0) {
            perror("btree_divide(): left->elements not allocated.");
            exit(-1);
        }
        init_list_head(left->elements);
        left->elements->next = node->elements->next;
        left->elements->next->prev = left->elements;
        target->prev->next = left->elements;
        left->head.parent = p_head;
        left->head.leaf = p_head->leaf;
        left->head.n = t - 1;
        
        init_btree_head(&right->head, tree->degree);
        right->elements = (struct LIST_HEAD_T*) malloc(sizeof(struct LIST_HEAD_T));
        init_list_head(right->elements);
        right->elements->next = target->next;
        right->elements->next->prev = right->elements;
        right->head.parent = p_head;
        right->head.leaf = p_head->leaf;
        right->head.n = tree->degree - left->head.n - 1;
        
        node->elements->next = target;
        target->prev = node->elements;
        target->next = node->elements;
        
        target = right->elements;
        while (target->next != node->elements) target = target->next;
        target->next = right->elements;
        
        p_head->n = 1;
        if (!p_head->leaf) {
            memcpy(left->head.childs, p_head->childs, sizeof(struct B_TREE_HEAD_T*) * t);
            memcpy(right->head.childs, &p_head->childs[t], sizeof(struct B_TREE_HEAD_T*)*(right->head.n+1));
            memset(p_head->childs, 0, sizeof(struct B_TREE_HEAD_T*)*(tree->degree+1));
            
            for (i=0; i<=left->head.n; i++) {
                left->head.childs[i]->parent = &left->head;
            }
            
            for (i=0; i<=right->head.n; i++) {
                right->head.childs[i]->parent = &right->head;
            }
        }
        p_head->childs[0] = &left->head;
        p_head->childs[1] = &right->head;
        p_head->leaf = FALSE;
        tree->depth++;
        
        return;
    }
    
    left = list_entry(p_head, struct B_TREE_NODE_T, head);
    right = (struct B_TREE_NODE_T*) malloc(sizeof(struct B_TREE_NODE_T));
    if (right == 0) {
        perror("btree_divide(): right not allocated.");
        exit(-1);
    }
    
    target->prev->next = left->elements; // == node->elements
    left->head.n = t - 1;
    left->head.leaf = p_head->leaf;
    
    init_btree_head(&right->head, tree->degree);
    right->elements = (struct LIST_HEAD_T*) malloc(sizeof(struct LIST_HEAD_T));
    if (right->elements == 0) {
        perror("btree_divide(): right->elements not allocated.");
        exit(-1);
    }
    init_list_head(right->elements);
    right->elements->next = target->next;
    right->elements->next->prev = right->elements;
    right->head.parent = p_head->parent;
    right->head.leaf = p_head->leaf;
    right->head.n = tree->degree - left->head.n - 1;
    
    // 삽입할 위치 찾기: ... -> target -> parent_el
    pr_node = list_entry(p_head->parent, struct B_TREE_NODE_T, head);
    pr_el_head = pr_node->elements;
    if (parent_el_idx == p_head->parent->n) {
        // 맨 뒤 삽입
        while (pr_el_head->next != pr_node->elements) pr_el_head = pr_el_head->next;
        target->next = pr_el_head->next;
        target->prev = pr_el_head;
        pr_el_head->next = target;
    } else {
        for (i=0; i<parent_el_idx; i++) pr_el_head = pr_el_head->next;
        target->next = pr_el_head->next;
        target->prev = pr_el_head;
        if (parent_el_idx != 0) pr_el_head->next->prev = target;
        pr_el_head->next = target;
    }
    
    target = right->elements;
    while (target->next != node->elements) target = target->next;
    target->next = right->elements;
    
    if (!p_head->leaf) {
        memcpy(right->head.childs, &p_head->childs[t], sizeof(struct B_TREE_HEAD_T*)*(right->head.n+1));
        memset(&p_head->childs[t], 0, sizeof(struct B_TREE_HEAD_T*)*(right->head.n+1));
        
        for (i=0; i<=right->head.n; i++) {
            right->head.childs[i]->parent = &right->head;
        }
    }
    
    p_head->parent->n++;
    
    struct B_TREE_HEAD_T** p_childs = pr_node->head.childs;
    for (i=p_head->parent->n; i>parent_el_idx; i--) {
        p_childs[i] = p_childs[i-1];
    }
    p_childs[parent_el_idx+1] = &right->head;
    if (parent_el_idx == 0) {
        p_childs[0] = &left->head;
    }
    
    if (p_head->parent->n == tree->degree) {
        if (p_head->parent->parent == 0) { // if parent node is root
            btree_divide(-1, p_head->parent, tree);
        } else {
            node = pr_node;
            pr_node = list_entry(pr_node->head.parent, struct B_TREE_NODE_T, head);
            for (i=0; i<=pr_node->head.n; i++) {
                if (pr_node->head.childs[i] == &node->head) break;
            }
            btree_divide(i, p_head->parent, tree);
        }
    }
}



struct B_TREE_NODE_T* _merge_with_parent(struct B_TREE_NODE_T *left,
                                         struct B_TREE_NODE_T *right,
                                         struct LIST_HEAD_T *pr_el_head)
{
    assert (left->head.leaf == right->head.leaf);
    struct LIST_HEAD_T *left_head = left->elements->next;
    struct LIST_HEAD_T *right_head = right->elements->next;
    while(left_head->next != left->elements) left_head = left_head->next;
    pr_el_head->prev->next = pr_el_head->next;
    if (pr_el_head->next->prev != 0) pr_el_head->next->prev = pr_el_head->prev;
    
    left_head->next = pr_el_head;
    pr_el_head->prev = left_head;
    pr_el_head->next = right_head;
    right_head->prev = pr_el_head;
    while(right_head->next != right->elements) right_head = right_head->next;
    right_head->next = left->elements;
    
    if (!left->head.leaf) {
        memcpy(&left->head.childs[++(left->head.n)], right->head.childs, sizeof(struct B_TREE_HEAD_T*)*(right->head.n + 1));
    }
    left->head.n += right->head.n;
    free(right);
    return left;
}


struct B_TREE_NODE_T* _merge_with_siblings(struct B_TREE_NODE_T *left,
                                           struct B_TREE_NODE_T *right)
{
    // 왼쪽 서브 트리의 최대 원소 교체
    struct B_TREE_NODE_T *merged;
    struct B_TREE_NODE_T *pr_node = left;
    struct B_TREE_NODE_T *l_subtree;
    struct B_TREE_NODE_T *r_subtree;
    struct LIST_HEAD_T *pr_el_head;
    struct LIST_HEAD_T *alternative;
    
    while (!pr_node->head.leaf) {
        left = list_entry(pr_node->head.childs[pr_node->head.n - 1], struct B_TREE_NODE_T, head);
        right = list_entry(pr_node->head.childs[pr_node->head.n], struct B_TREE_NODE_T, head);
        pr_el_head = pr_node->elements->next;
        while(pr_el_head->next != pr_node->elements) pr_el_head = pr_el_head->next;
        merged = _merge_with_parent(left, right, pr_el_head);
        pr_node->head.childs[pr_node->head.n - 1] = &merged->head;
        pr_node = right;
    }
    
    pr_node->head.n--; // 단말 노드 마지막 원소 제거
    alternative = pr_node->elements->next;
    while(alternative->next != pr_node->elements) alternative = alternative->next;
    pr_el_head = left->elements->next; // 왼쪽 서브 트리 내 루트 노드의 마지막 원소
    while(pr_el_head->next != left->elements) pr_el_head = pr_el_head->next;
    pr_el_head->next = alternative;
    alternative->prev = pr_el_head;
    alternative->next = left->elements;
    
    // 오른쪽 서브 트리와 합치기
    alternative->next = right->elements->next;
    while (alternative->next != right->elements) alternative = alternative->next;
    alternative->next = left->elements;
    memcpy(&left->head.childs[left->head.n], right->head.childs, sizeof(struct B_TREE_HEAD_T*)*(right->head.n+1));
    left->head.n += right->head.n;
    free(right);
    return left;
}


struct B_TREE_NODE_T* _merge_without_parent(struct B_TREE_NODE_T *subtree,
                                            struct B_TREE_HEAD_T *other_subtree,
                                            int direction)
{
    struct B_TREE_NODE_T *merged;
    struct B_TREE_NODE_T *pr_node = subtree;
    if (direction == 0) { // 왼쪽 서브 트리에 포함되는 경우
        struct B_TREE_NODE_T *left;
        struct B_TREE_NODE_T *right;
        struct LIST_HEAD_T *pr_el_head;
        struct LIST_HEAD_T *alternative;
        
        while (!pr_node->head.leaf) {
            left = list_entry(pr_node->head.childs[pr_node->head.n - 1], struct B_TREE_NODE_T, head);
            right = list_entry(pr_node->head.childs[pr_node->head.n], struct B_TREE_NODE_T, head);
            pr_el_head = pr_node->elements->next;
            while(pr_el_head->next != pr_node->elements) pr_el_head = pr_el_head->next;
            merged = _merge_with_parent(left, right, pr_el_head);
            pr_node->head.childs[pr_node->head.n - 1] = &merged->head;
            pr_node = right;
        }
        
        pr_node->head.n--; // 단말 노드 마지막 원소 제거
        alternative = pr_node->elements->next;
        while(alternative->next != pr_node->elements) alternative = alternative->next;
        pr_el_head = subtree->elements->next; // 서브 트리 루트 노드 내 원소 포인터
        while(pr_el_head->next != subtree->elements) pr_el_head = pr_el_head->next;
        pr_el_head->next = alternative;
        alternative->prev = pr_el_head;
        alternative->next = subtree->elements;
        subtree->head.childs[subtree->head.n] = other_subtree;
    } else { // 오른쪽 서브 트리에 포함되는 경우
        merged = _merge_with_siblings(subtree->head.childs[0], subtree->head.childs[1]);
        subtree->head.childs[1] = &merged->head;
        subtree->head.childs[0] = other_subtree;
    }
    return subtree;
}


/*
 1. 단말 노드의 원소 수 >= t,
 2. 단말 노드의 원소 수 < t,
    2.1. 왼쪽 형제 노드의 원소 수 >= t, borrow_from_prev
    2.2. 왼쪽 형제 노드의 원소 수 < t,
        2.2.1. 오른쪽 형제 노드의 원소 수 >= t, borrow_from_next
        2.2.2. 오른쪽 형제 노드의 원소 수 < t,
            2.2.2.1. 부모 노드의 원소 수 >= t, 부모 원소, 단말 노드, 형제 노드 병합 merge
            2.2.2.2. 부모 노드의 원소 수 < t, borrow_from_parent
 원소 삭제
 */
void _delete_in_leaf(int t,
                     int el_idx, // 단말 노드에서 삭제되어야 하는 원소 번호
                     int pr_el_idx,
                     struct LIST_HEAD_T *el_head,
                     struct LIST_HEAD_T *pr_el_head,
                     struct B_TREE_NODE_T *p_node,
                     struct B_TREE_HEAD_T *root)
{
    if (p_node->head.n < t) {
        struct B_TREE_NODE_T *pr_node = list_entry(p_node->head.parent, struct B_TREE_NODE_T, head);
        struct B_TREE_HEAD_T *sibling_head;
        int direction = 0; // 1: right, 0: left, -1: impossible
        if (el_idx == 0) {
            sibling_head = pr_node->head.childs[el_idx+1];
            direction = (sibling_head->n >= t) ? 1 : -1;
        } else if (el_idx == pr_node->head.n) {
            sibling_head = pr_node->head.childs[el_idx-1];
            direction = (sibling_head->n >= t) ? 0 : -1;
        } else {
            sibling_head = pr_node->head.childs[el_idx-1];
            if (sibling_head->n < t) {
                struct B_TREE_HEAD_T *right_head = pr_node->head.childs[el_idx+1];
                sibling_head = right_head;
                direction = (right_head->n >= t) ? 1 : -1;
            }
        }
        
        struct B_TREE_NODE_T *sibling = list_entry(sibling_head, struct B_TREE_NODE_T, head);
        if (direction == 0) {
            _borrow_from_left(sibling, p_node, pr_el_head);
        } else if (direction == 1) {
            _borrow_from_right(sibling, p_node, pr_el_head);
        } else {
            int i;
            struct B_TREE_NODE_T *merged;
            if (el_idx == 0) merged = _merge_with_parent(p_node, sibling, pr_el_head);
            else merged = _merge_with_parent(sibling, p_node, pr_el_head);
            for (i=el_idx; i<pr_node->head.n; i++) {
                pr_node->head.childs[i] = pr_node->head.childs[i+1];
            }
            pr_node->head.childs[el_idx] = &merged->head;
            if (pr_node->head.n >= t) {
                pr_node->head.childs[pr_node->head.n--] = 0;
            } else {
                // 부모 노드가 그랜드 부모의 첫번째 자식이면 merged는 부모 노드의 0번째에 위치
                // 그 외의 경우는 child_idx
                // 현재 부모 노드는 t-1개 미만의 원소를 가지고 있다. (병합 때문)
                // 그랜드 부모에게서 받은 원소는 원래 부모 원소가 있던 위치에 연결하고, 얻은 트리는 맨 처음 또는 맨 끝에 연결
                // _borrow_from_parent(pr_el_idx, pr_el_head, pr_node, p_node, root)
            }
        }
    } else {
        p_node->head.n--;
    }
    el_head->prev->next = el_head->next;
    if (el_head->next->prev != 0) el_head->next->prev = el_head->prev;
}


void _borrow_from_left(struct B_TREE_NODE_T *left,
                       struct B_TREE_NODE_T *p_node,
                       struct LIST_HEAD_T *pr_el_head)
{
    assert (left->head.n > p_node->head.n);
    struct LIST_HEAD_T *left_el_head = left->elements->next;
    while(left_el_head->next != left->elements) left_el_head = left_el_head->next;
    left_el_head->prev->next = left->elements;
    
    // move the last element of left sibling node to parent node.
    left_el_head->next = pr_el_head->next;
    left_el_head->prev = pr_el_head->prev;
    pr_el_head->prev->next = left_el_head;
    if (pr_el_head->next->prev != 0) pr_el_head->next->prev = left_el_head;
    
    // move parent element to current node.
    p_node->elements->next->prev = pr_el_head;
    pr_el_head->next = p_node->elements->next;
    pr_el_head->prev = p_node->elements;
    p_node->elements->next = pr_el_head;
    left->head.n--;
}

void _borrow_from_right(struct B_TREE_NODE_T *right,
                        struct B_TREE_NODE_T *p_node,
                        struct LIST_HEAD_T *pr_el_head)
{
    assert (right->head.n > p_node->head.n);
    struct LIST_HEAD_T *el_head = p_node->elements->next;
    struct LIST_HEAD_T *right_el_head = right->elements->next;
    while (el_head->next != p_node->elements) el_head = el_head->next;
    right_el_head->prev->next = right_el_head->next;
    right_el_head->next->prev = right_el_head->prev;
    
    
    // move the first element of right sibling node to parent node.
    right_el_head->next = pr_el_head->next;
    right_el_head->prev = pr_el_head->prev;
    pr_el_head->prev->next = right_el_head;
    if (pr_el_head->next->prev != 0) pr_el_head->next->prev = right_el_head;
    
    // move parent element to current node.
    pr_el_head->prev = el_head;
    pr_el_head->next = el_head->next;
    el_head->next = pr_el_head;
    right->head.n--;
}


/*
 1. 왼쪽 자식 노드의 원소 수 >= t,
    - 왼쪽 자식 노드의 마지막 원소 --> 부모 원소 --> 오른쪽 자식 노드의 첫번째 원소 : 원소 교체
    - 왼쪽 자식 노드에 서브트리(TREE_A) 가 있으면,
        - 오른쪽 트리(=요청한 트리)의 처음에 TREE_A를 추가
        - 오른쪽 트리에서도 병합이 발생 : TREE_B = _merge_without_parent(오른쪽트리의 0번째, 1번째 트리)
        - TREE_A의 뒤에 TREE_B를 추가
 
 2. 오른쪽 자식노드의 원소수 >= t,
    - 오른쪽 자식 노드의 첫번째 원소 --> 부모 원소 --> 왼쪽 자식 노드의 마지막 원소 : 원소 교체
    - 오른쪽 자식 노드에 서브트리(TREE_A)가 있으면,
        - 왼쪽 트리(=요청한 트리)의 끝에 TREE_A를 추가
        - 왼쪽 트리에서도 병합이 발생 : TREE_B = _merge_without_parent(왼쪽 트리의 n번째, n-1번째 트리)
        - TREE_A의 앞에 TREE_B를 추가
 
 3. 현재 노드의 원소 수 >= t,
    - 요청한 트리에서 삭제되는 원소의 자식 트리들은 먼저 병합 : _merge_without_parent()
    - 부모 원소가 삭제되는 원소의 위치로 가고,
    - 형제 트리와 그대로 합친다. (링크만 연결)
 
 4. 현재 노드의 원소 수 < t,
    - 재귀호출(자기부모)
    - 자신의 원소를 자식에게 전달
    -
 부모 원소 반환
 */
// 부모 노드가 그랜드 부모의 첫번째 자식이면 merged는 부모 노드의 0번째에 위치
// 그 외의 경우는 child_idx
// 현재 부모 노드는 t-1개 미만의 원소를 가지고 있다. (병합 때문)
// 그랜드 부모에게서 받은 원소는 원래 부모 원소가 있던 위치에 연결하고, 얻은 트리는 맨 처음 또는 맨 끝에 연결
// _borrow_from_parent(pr_el_idx, pr_el_head, pr_node, p_node, root)
struct LIST_HEAD_T* _borrow_from_parent(int pr_el_idx,
                                        struct LIST_HEAD_T *pr_el_head,
                                        struct B_TREE_NODE_T *pr_node,
                                        struct B_TREE_NODE_T *p_node,
                                        struct B_TREE_NODE_T *root)
{
    // pr_node 는 t - 1 미만의 원소 수를 가지고 있음
    // grand_pr_node를 찾아야 함
    //
    struct LIST_HEAD_T *alternative = 0;
    
    return alternative;
}


/* 왼쪽 서브 트리에서 전임자 찾기
 1. 단말 노드 찾기
 2. 단말 노드의 원소 수 >= t, 최대 원소 반환
 3. 단말 노드의 원소 수 < t,
    3.1. 형제 노드의 원소 수 >= t, borrow from left
    3.2. 형제 노드의 원소 수 < t,
        3.2.1. 부모 노드의 원소 수 >= t, 부모 원소 + 형제 노드 + 단말 노드 병합 merge
        3.2.2. 부모 노드의 원소 수 < t,
            3.2.2.1.
                상향식 탐색
            3.2.2.2. 못빌리면 널값 반환
 
 최대 원소 반환
*/
struct LIST_HEAD_T* _get_predecessor(struct B_TREE_NODE_T *subtree,
                                     struct B_TREE_T *tree)
{
    
    int t = ceil(tree->degree/2);
    struct B_TREE_HEAD_T *descendant_head = subtree->head.childs[subtree->head.n];
    struct B_TREE_HEAD_T *sibling_head = subtree->head.childs[subtree->head.n - 1];
    struct B_TREE_HEAD_T *pr_head_of_desc = &subtree->head;
    struct B_TREE_NODE_T *descendant = subtree;
    struct B_TREE_NODE_T *sibling;
    struct LIST_HEAD_T *predecessor;
    
    if (subtree->head.leaf) {
        if (subtree->head.n >= t) {
            predecessor = subtree->elements->next;
            while (predecessor->next != subtree->elements) predecessor = predecessor->next;
            predecessor->prev->next = subtree->elements;
            subtree->head.n--;
            return predecessor;
        } else {
            return 0;
        }
    }
    
    while (!descendant_head->leaf) {
        pr_head_of_desc = descendant_head;
        sibling_head = descendant_head->childs[descendant_head->n - 1];
        descendant_head = descendant_head->childs[descendant_head->n];
    }
    descendant = list_entry(descendant_head, struct B_TREE_NODE_T, head);
    sibling = list_entry(sibling_head, struct B_TREE_NODE_T, head);
    predecessor = descendant->elements->next;
    while(predecessor->next != descendant->elements) predecessor = predecessor->next;
    if (descendant_head->n < t) {
        struct B_TREE_NODE_T *pr_node_of_desc = list_entry(pr_head_of_desc, struct B_TREE_NODE_T, head);
        struct LIST_HEAD_T *pr_el_head_of_desc = pr_node_of_desc->elements->next;
        while (pr_el_head_of_desc->next != pr_node_of_desc->elements) pr_el_head_of_desc = pr_el_head_of_desc->next;
        if (sibling_head->n >= t) {
            _borrow_from_left(sibling, descendant, pr_el_head_of_desc);
        } else {
            if (pr_head_of_desc->n >= t) {
                struct B_TREE_NODE_T *merged = _merge_with_parent(sibling, descendant, pr_el_head_of_desc);
                pr_head_of_desc->childs[pr_head_of_desc->n--] = 0;
                pr_head_of_desc->childs[pr_head_of_desc->n] = &merged->head;
            } else {
                //struct LIST_HEAD_T *alternative = _borrow_from_parent(pr_node_of_desc, sibling, descendant, tree);
                //if (alternative == 0) return 0;
            }
        }
    }
    predecessor->prev->next = predecessor->next;
    if (predecessor->next->prev != 0) predecessor->next->prev = predecessor->prev;
    return predecessor;
}


/* 오른쪽 서브 트리에서 후임자 찾기
 1. 오른쪽 자식 노드의 원소 수 >= t,
    1.1. 첫번째 자식 원소 제거
    1.2. 첫번째 자식 트리와 형제 서브 트리를 병합
 2. 오른쪽 자식 노드의 원소 수 < t, 널값 반환
*/
struct LIST_HEAD_T* _get_successor(struct B_TREE_NODE_T *subtree,
                                   struct B_TREE_NODE_T *sibling,
                                   struct B_TREE_T *tree)
{
    int t = ceil(tree->degree/2);
    struct LIST_HEAD_T *ret = 0;
    if (subtree->head.n >= t) {
        ret = subtree->elements->next;
        ret->prev->next = ret->next;
        ret->next->prev = ret->prev;
        _merge_without_parent(sibling, subtree->head.childs[0], 0);
    }
    return ret;
}

/*
 1. 전임자 탐색: _get_predecessor
 1.1. 전임자 == 0 && 삭제할 원소 != 첫번째 원소 && 현재 노드의 원소 수 >= t,
 1.1.1. 직전 원소의 왼쪽 자식 노드의 원소 수 >= t, borrow from left
 1.1.2. 직전 원소의 왼쪽 자식 노드의 원소 수 < t, 직전 원소 + 왼쪽 서브트리 + 오른쪽 서브 트리 병합 merge 후, 최대 원소로 대체
 2. 위의 경우가 아닌 경우
 2.1. 오른쪽 자식 노드의 원소 수 >= t, 후임자 탐색
 2.1.1. 오른쪽 자식 노드의 첫번째 원소를 빌려오고, 왼쪽 서브트리에 오른쪽에서의 첫번째 자식 트리를 병합
 2.2. 오른쪽 자식 노드의 원소 수 < t,
 2.2.1. 현재 노드의 원소 수 >= t, 왼쪽 서브트리 + 오른쪽 서브 트리 병합 (부모 없는 병합) - _merge 함수에서 후술
 - 루트 노드인 경우는 제외
 2.2.2. 현재 노드의 원소 수 < t, borrow_from_parent == _delete_from_no_leaf(parent)
 - 왼쪽 형제 노드의 원소 수 >= t, 왼쪽 부모 원소에게 빌리기 요청
 - 오른쪽 형제 노드의 원소 수 >= t, 오른쪽 부모 원소에게 빌리기 요청
 - 둘 다 해당 안되는 경우,
 - 부모 원소 수 >= t,
 - 부모 원소 + 형제 노드 병합 = 트리 A
 - 현재 노드의 자식 트리 2개를 병합 = 트리 B
 - 트리 A의 가장 끝에 트리 B를 자식으로 연결
 - 부모 원소 수 < t,
 - 상향식 탐색: 부모에게 트리 조정을 요구할 때, 자신이 위치한 서브 트리의 링크를 전달
 - borrow 함수는 항상 삭제할 원소를 대체할 원소를 반환
 원소 삭제
 */
void _delete_in_no_leaf(int el_idx,
                          int pr_el_idx,
                          struct LIST_HEAD_T *el_head,
                          struct LIST_HEAD_T *pr_el_head,
                          struct B_TREE_NODE_T *p_node,
                          struct B_TREE_T *tree)
{
    int t = ceil(tree->degree/2);
    struct B_TREE_NODE_T *left = list_entry(p_node->head.childs[el_idx], struct B_TREE_NODE_T, head);
    struct B_TREE_NODE_T *right = list_entry(p_node->head.childs[el_idx+1], struct B_TREE_NODE_T, head);
    struct LIST_HEAD_T *alternative = _get_predecessor(left, tree);
    if (alternative == 0) {
        alternative = _get_successor(left, right, tree);
        if (alternative == 0) {
            int i;
            struct B_TREE_NODE_T *merged;
            if (p_node->head.n >= t) {
                if (el_idx == 0) {
                    merged = _merge_with_siblings(left, right);
                    p_node->head.childs[0] = &merged->head;
                    for (i=1; i<p_node->head.n; i++) {
                        p_node->head.childs[i] = p_node->head.childs[i+1];
                    }
                } else {
                    merged = _merge_with_parent(p_node->head.childs[el_idx-1], p_node->head.childs[el_idx], el_head->prev);
                    p_node->head.childs[el_idx-1] = &merged->head;
                    alternative = _get_predecessor(&merged->head, tree);
                    assert (alternative != 0);
                    for (i=el_idx; i<p_node->head.n; i++) {
                        p_node->head.childs[i] = p_node->head.childs[i+1];
                    }
                }
            } else {
                merged = _merge_with_siblings(left, right);
                p_node->head.childs[el_idx] = &merged->head;
                for (i=el_idx+1; i<p_node->head.n; i++) {
                    p_node->head.childs[i] = p_node->head.childs[i+1];
                }
                // borrow_from_parent
            }
            p_node->head.childs[p_node->head.n--] = 0;
        }
    }
    alternative->next = el_head->next;
    alternative->prev = el_head->prev;
    el_head->prev->next = alternative;
    if (el_head->next->prev != 0) el_head->next->prev = alternative;
}

void btree_delete(int el_idx,
                  int subtree_idx,
                  struct B_TREE_HEAD_T *p_head,
                  struct LIST_HEAD_T *el_head,
                  struct LIST_HEAD_T *pr_el_head,
                  struct B_TREE_T *tree)
{
    int i;
    struct B_TREE_NODE_T *p_node = list_entry(p_head, struct B_TREE_NODE_T, head);
    int pr_el_idx = (subtree_idx == 0) ? 0 : subtree_idx - 1;
    if (p_head->leaf) {
        int t = ceil(tree->degree / 2);
        _delete_in_leaf(t, el_idx, pr_el_idx, el_head, pr_el_head, p_node, tree->root);
    } else {
        _delete_in_no_leaf(el_idx, pr_el_idx, el_head, pr_el_head, p_node, tree);
    }
}

/*
struct LIST_HEAD_T* btree_delete(int el_idx,
                                 int subtree_idx,
                                 struct B_TREE_HEAD_T *p_head,
                                 struct LIST_HEAD_T *el_head,
                                 struct LIST_HEAD_T *pr_el_head,
                                 struct B_TREE_T *tree)
{
    int i;
    int t = ceil(tree->degree / 2);
    struct B_TREE_NODE_T *p_node = list_entry(p_head, struct B_TREE_NODE_T, head);
    struct B_TREE_NODE_T *pr_node = list_entry(p_head->parent, struct B_TREE_NODE_T, head);
    int pr_el_idx = (subtree_idx == 0) ? 0 : subtree_idx - 1;
    if (p_head->leaf) {
        // delete the element
        el_head->prev->next = el_head->next;
        if (el_head->next->prev != 0) {
            el_head->next->prev = el_head->prev;
        }
        p_head->n--;
        init_list_head(el_head);
        
        // operation
        if (p_head->n < t-1) {
            struct B_TREE_NODE_T *sibling = 0;
            int right_sibling = 0;
            if (subtree_idx == 0) {
                sibling = list_entry(p_head->parent->childs[subtree_idx+1], struct B_TREE_NODE_T, head);
                right_sibling = 1;
            } else if (subtree_idx == p_head->n) {
                sibling = list_entry(p_head->parent->childs[subtree_idx-1], struct B_TREE_NODE_T, head);
            } else {
                sibling = list_entry(p_head->parent->childs[subtree_idx-1], struct B_TREE_NODE_T, head);
                struct B_TREE_NODE_T *r_sibling = list_entry(p_head->parent->childs[subtree_idx+1], struct B_TREE_NODE_T, head);
                if (sibling->head.n < t && r_sibling->head.n >= t) {
                    pr_el_head = pr_el_head->next;
                    sibling = r_sibling;
                    right_sibling = 1;
                }
            }
            
            if (sibling->head.n >= t) {
                btree_borrow(p_node, right_sibling, sibling, pr_el_head);
                p_head->n++;
            } else {
                if (p_node->head.parent->n >= t) {
                    struct B_TREE_NODE_T *merged = btree_merge(right_sibling, sibling, p_node, pr_el_head);
                    assert (merged->head.n <= tree->degree - 1);
                    pr_node->head.childs[pr_el_idx] = &merged->head;
                    for (i=pr_el_idx+1; i<pr_node->head.n; i++) {
                        pr_node->head.childs[i] = pr_node->head.childs[i+1];
                    }
                    pr_node->head.childs[pr_node->head.n] = 0;
                    pr_node->head.n--;
                } else {
                    // 상향식 탐색
                    return 0; // 탐색 불가능한 경우
                }
                
            }
        }
        return el_head;
    } else {
    }
    return 0;
}
*/

int btree_empty(struct B_TREE_T *tree) {
    return tree->root->n == 0;
}
