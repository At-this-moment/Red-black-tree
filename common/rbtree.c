#include "rbtree.h"
#include <stdlib.h>

/* ───────────  내부 헬퍼 선언 ─────────── */
static void left_rotate(rbtree *t, node_t *x);
static void right_rotate(rbtree *t, node_t *x);
static void insert_fixup(rbtree *t, node_t *z);
static void delete_fixup(rbtree *t, node_t *x);
static void transplant(rbtree *t, node_t *u, node_t *v);
static node_t *minimum(const rbtree *t, node_t *x);
static void postorder_free(rbtree *t, node_t *x);
static void inorder_copy(const rbtree *t,node_t *x,key_t *arr,size_t *i,size_t n);

/* ───────────  생성 / 소멸 ─────────── */
rbtree *new_rbtree(void){
  rbtree *T = calloc(1,sizeof(rbtree));
  T->nil  = calloc(1,sizeof(node_t));
  T->nil->color = RBTREE_BLACK;
  T->nil->left = T->nil->right = T->nil->parent = T->nil;
  T->root = T->nil;
  return T;
}
void delete_rbtree(rbtree *t){
  postorder_free(t,t->root);
  free(t->nil); free(t);
}

/* ───────────  회전 ─────────── */
static void left_rotate(rbtree *t,node_t *x){
  node_t *y = x->right;
  x->right  = y->left;
  if(y->left!=t->nil) y->left->parent = x;
  y->parent = x->parent;
  if(x->parent==t->nil)      t->root = y;
  else if(x==x->parent->left) x->parent->left  = y;
  else                        x->parent->right = y;
  y->left  = x;
  x->parent = y;
}
static void right_rotate(rbtree *t,node_t *x){
  node_t *y = x->left;
  x->left   = y->right;
  if(y->right!=t->nil) y->right->parent = x;
  y->parent = x->parent;
  if(x->parent==t->nil)      t->root = y;
  else if(x==x->parent->right) x->parent->right = y;
  else                         x->parent->left  = y;
  y->right = x;
  x->parent = y;
}

/* ───────────  삽입 ─────────── */
node_t *rbtree_insert(rbtree *t,const key_t k){
  node_t *y=t->nil,*x=t->root;
  while(x!=t->nil){
    y=x;
    x = (k < x->key) ? x->left : x->right;   /* 중복 허용 */
  }
  node_t *z = calloc(1,sizeof(node_t));
  z->key=k; z->color=RBTREE_RED;
  z->left=z->right=t->nil;  z->parent=y;
  if(y==t->nil)         t->root=z;
  else if(k<y->key)     y->left = z;
  else                  y->right= z;
  insert_fixup(t,z);
  return z;
}
static void insert_fixup(rbtree *t,node_t *z){
  while(z->parent->color==RBTREE_RED){
    if(z->parent==z->parent->parent->left){
      node_t *y = z->parent->parent->right;
      if(y->color==RBTREE_RED){
        z->parent->color = y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }else{
        if(z==z->parent->right){ z=z->parent; left_rotate(t,z); }
        z->parent->color=RBTREE_BLACK;
        z->parent->parent->color=RBTREE_RED;
        right_rotate(t,z->parent->parent);
      }
    }else{
      node_t *y = z->parent->parent->left;
      if(y->color==RBTREE_RED){
        z->parent->color = y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }else{
        if(z==z->parent->left){ z=z->parent; right_rotate(t,z); }
        z->parent->color=RBTREE_BLACK;
        z->parent->parent->color=RBTREE_RED;
        left_rotate(t,z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

/* ───────────  탐색/최소/최대 ─────────── */
node_t *rbtree_find(const rbtree *t,const key_t k){
  node_t *cur=t->root;
  while(cur!=t->nil){
    if(k==cur->key) return cur;
    cur = (k<cur->key)?cur->left:cur->right;
  }
  return NULL;
}
node_t *rbtree_min (const rbtree *t){ return minimum(t,t->root); }
node_t *rbtree_max (const rbtree *t){
  node_t *x=t->root; if(x==t->nil) return NULL;
  while(x->right!=t->nil) x=x->right; return x;
}
static node_t *minimum(const rbtree *t,node_t *x){
  while(x->left!=t->nil) x=x->left; return x;
}

/* ───────────  삭제 ─────────── */
static void transplant(rbtree *t,node_t *u,node_t *v){
  if(u->parent==t->nil)       t->root = v;
  else if(u==u->parent->left) u->parent->left  = v;
  else                        u->parent->right = v;
  v->parent = u->parent;          /* v==nil 이어도 parent 반드시 설정 */
}
int rbtree_erase(rbtree *t,node_t *z){
  node_t *y=z, *x;
  color_t y_orig_col = y->color;

  if(z->left==t->nil){
    x = z->right;
    transplant(t,z,z->right);
  }else if(z->right==t->nil){
    x = z->left;
    transplant(t,z,z->left);
  }else{
    y = minimum(t,z->right);
    y_orig_col = y->color;
    x = y->right;
    if(y->parent==z){
      x->parent = y;                /* x 가 nil 이더라도 parent 지정 */
    }else{
      transplant(t,y,y->right);
      y->right = z->right;  y->right->parent = y;
    }
    transplant(t,z,y);
    y->left = z->left;   y->left->parent  = y;
    y->color = z->color;
  }

  if(y_orig_col==RBTREE_BLACK) delete_fixup(t,x);
  free(z);
  return 0;
}
static void delete_fixup(rbtree *t,node_t *x){
  while(x!=t->root && x->color==RBTREE_BLACK){
    if(x==x->parent->left){
      node_t *w=x->parent->right;
      if(w->color==RBTREE_RED){
        w->color=RBTREE_BLACK; x->parent->color=RBTREE_RED;
        left_rotate(t,x->parent);  w=x->parent->right;
      }
      if(w->left->color==RBTREE_BLACK && w->right->color==RBTREE_BLACK){
        w->color=RBTREE_RED; x=x->parent;
      }else{
        if(w->right->color==RBTREE_BLACK){
          w->left->color=RBTREE_BLACK; w->color=RBTREE_RED;
          right_rotate(t,w); w=x->parent->right;
        }
        w->color=x->parent->color; x->parent->color=RBTREE_BLACK;
        w->right->color=RBTREE_BLACK;
        left_rotate(t,x->parent); x=t->root;
      }
    }else{
      node_t *w=x->parent->left;
      if(w->color==RBTREE_RED){
        w->color=RBTREE_BLACK; x->parent->color=RBTREE_RED;
        right_rotate(t,x->parent); w=x->parent->left;
      }
      if(w->right->color==RBTREE_BLACK && w->left->color==RBTREE_BLACK){
        w->color=RBTREE_RED; x=x->parent;
      }else{
        if(w->left->color==RBTREE_BLACK){
          w->right->color=RBTREE_BLACK; w->color=RBTREE_RED;
          left_rotate(t,w); w=x->parent->left;
        }
        w->color=x->parent->color; x->parent->color=RBTREE_BLACK;
        w->left->color=RBTREE_BLACK;
        right_rotate(t,x->parent); x=t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

/* ───────────  to_array ─────────── */
int rbtree_to_array(const rbtree *t,key_t *arr,const size_t n){
  size_t i=0; inorder_copy(t,t->root,arr,&i,n); return 0;
}
static void inorder_copy(const rbtree *t,node_t *x,key_t *a,size_t *i,size_t n){
  if(x==t->nil || *i>=n) return;
  inorder_copy(t,x->left ,a,i,n);
  a[(*i)++] = x->key;
  inorder_copy(t,x->right,a,i,n);
}

/* ───────────  postorder free ─────────── */
static void postorder_free(rbtree *t,node_t *x){
  if(x==t->nil) return;
  postorder_free(t,x->left); postorder_free(t,x->right); free(x);
}