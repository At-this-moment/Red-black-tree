#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>


//서넌
void left_rotate(rbtree *t, node_t *x);
void right_rotate(rbtree *t, node_t *x);
void insert_fixup(rbtree *t, node_t *z);


rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;

  p->nil->left = p->nil->right = p->nil->parent = p->nil;  // 👈 이 줄이 꼭 있어야 함!

  p->root = p->nil;
  return p;
}



//트리 내부에서 삽입이나 삭제 후,균형이 깨졌을 때 호출함
void left_rotate(rbtree *t, node_t *x) {
  
  node_t *y = x->right;
  x->right = y->left;

  if (y->left != t->nil) {
    y->left->parent = x;
  }

  y->parent = x->parent;

  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;

  if (y->right != t->nil) {
    y->right->parent = x;
  }

  y->parent = x->parent;

  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }

  y->right = x;
  x->parent = y;
}

void insert_fixup(rbtree *t, node_t *z){
  while(z->parent->color==RBTREE_RED){
    if(z->parent==z->parent->parent->left){
      node_t *y=z->parent->parent->right;

      if(y->color==RBTREE_RED){
        z->parent->color=RBTREE_BLACK;
        y->color=RBTREE_BLACK;
        z->parent->parent->color=RBTREE_RED;
        z=z->parent->parent;
      }
      else{
        if(z==z->parent->right){
          z=z->parent;
          left_rotate(t,z);
        }

        z->parent->color=RBTREE_BLACK;
        z->parent->parent->color=RBTREE_RED;
        right_rotate(t,z->parent->parent);
      }
    }
    else{
      node_t *y=z->parent->parent->left;

      if(y->color==RBTREE_RED){
        z->parent->color=RBTREE_BLACK;
        y->color=RBTREE_BLACK;
        z->parent->parent->color=RBTREE_RED;
        z=z->parent->parent;
      }
      else{
        if(z==z->parent->left){
          z=z->parent;
          right_rotate(t,z);
        }

        z->parent->color=RBTREE_BLACK;
        z->parent->parent->color=RBTREE_RED;
        left_rotate(t,z->parent->parent);
      }
    }
  }
  t->root->color=RBTREE_BLACK;
}

void postorder_delete(rbtree *t, node_t *node){
  if(node==t->nil) return;
  postorder_delete(t,node->left);
  postorder_delete(t,node->right);
  free(node);
}

void delete_rbtree(rbtree *t) {//rb트리에 할당된 메모리 전부 해제
  //순회를 통해 모든 노드 프리시키기
  // TODO: reclaim the tree nodes's memory
  postorder_delete(t,t->root);
  free(t->nil);
  free(t);
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil) {
    t->root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }

  if (v != t->nil) {
    v->parent = u->parent;
  }
}


void delete_fixup(rbtree *t, node_t *x) {
  while (x != t->root && x->color == RBTREE_BLACK) {
    if (x == NULL || x->parent == NULL) {
      printf("🚨 x or x->parent is NULL, skipping fixup!\n");
      return;
    }    
    if (x == x->parent->left) {
      node_t *w = x->parent->right;

      // Case 1
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }

      // Case 2
      if ((w->left == t->nil || w->left->color == RBTREE_BLACK) &&
          (w->right == t->nil || w->right->color == RBTREE_BLACK)) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        // Case 3
        if (w->right == t->nil || w->right->color == RBTREE_BLACK) {
          if (w->left != t->nil)
            w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }

        // Case 4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        if (w->right != t->nil)
          w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    } else {
      node_t *w = x->parent->left;

      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }

      if ((w->left == t->nil || w->left->color == RBTREE_BLACK) &&
          (w->right == t->nil || w->right->color == RBTREE_BLACK)) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->left == t->nil || w->left->color == RBTREE_BLACK) {
          if (w->right != t->nil)
            w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }

        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        if (w->left != t->nil)
          w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

void inorder(const rbtree *t, node_t *node, key_t *arr, size_t *idx, size_t n) {
  if (node == t->nil || *idx >= n) return;
  inorder(t, node->left, arr, idx, n);
  printf(">> inorder visit: %d\n", node->key); // 🔍 중위 순회 디버그
  arr[(*idx)++] = node->key;
  inorder(t, node->right, arr, idx, n);
}



node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *y = t->nil;
  node_t *x = t->root;

  while (x != t->nil) {
    y = x;
    if (key == x->key) return x; // 중복이면 기존 노드 반환 (optional)
    if (key < x->key) x = x->left;
    else x = x->right;
  }

  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;
  z->parent = y;
  z->left = z->right = t->nil;
  z->color = RBTREE_RED;

  if (y == t->nil) t->root = z;
  else if (key < y->key) y->left = z;
  else y->right = z;

  insert_fixup(t, z);
  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *cur = t->root;
  while (cur != t->nil) {
    if (key < cur->key)
      cur = cur->left;
    else if (key > cur->key)
      cur = cur->right;
    else
      return cur;
  }
  printf("❌ rbtree_find: key %d not found\n", key);
  return NULL;
}


node_t *rbtree_min(const rbtree *t) {//트리에서 최소값찾기, 맨 왼쪽 맨 아래 노드
  //왼쪽 끝까지 탐색
  // TODO: implement find
  node_t *cur=t->root;
  if(cur==t->nil) return NULL;//트리가 비어있을때

  while (cur->left!=t->nil){
    cur=cur->left;
  }
  return cur;
}

node_t *rbtree_max(const rbtree *t) {//트리에서 최대값찾기, 맨 오른쪽 맨 아래 노드
  //오른쪽 끝까지 탐색
  // TODO: implement find
  node_t *cur=t->root;
  if(cur==t->nil) return NULL;

  while (cur->right!=t->nil){
    cur=cur->right;
  }
  return cur;
}

node_t *rbtree_minimum(const rbtree *t, node_t *x) {
  while (x->left != t->nil) {
    x = x->left;
  }
  return x;
}

int rbtree_erase(rbtree *t, node_t *p) {
  printf(">> erase called: %d\n", p->key);

  node_t *y = p;
  node_t *x;
  color_t y_original_color = y->color;

  if (p->left == t->nil) {
    x = p->right;
    rbtree_transplant(t, p, p->right);
  } else if (p->right == t->nil) {
    x = p->left;
    rbtree_transplant(t, p, p->left);
  } else {
    y = rbtree_minimum(t, p->right);
    y_original_color = y->color;
    x = y->right;

    if (y->parent == p) {
      x->parent = y;
    } else {
      rbtree_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }

    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  // ✅ delete_fixup은 반드시 BLACK 노드만 처리하므로 조건 필요
  if (y_original_color == RBTREE_BLACK) {
    delete_fixup(t, x);
  }

  // ✅ 메모리 해제는 y가 p와 다를 경우 p만 지우면 됨
  if (y != p) {
    free(p);
  } else {
    free(y);
  }

  return 0;
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  size_t idx = 0;
  inorder(t, t->root, arr, &idx, n);
  printf(">> total elements written: %lu\n", idx);
  return 0;
}