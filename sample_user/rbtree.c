#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {//새로운 rb트리생성
  //루트와 닐 노드 초기화
  // TODO: initialize struct if needed
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  return p;
}

void delete_rbtree(rbtree *t) {//rb트리에 할당된 메모리 전부 해제
  //순회를 통해 모든 노드 프리시키기
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {//rb트리에 키값 삽입
  //바이너리 서치트리 삽입
  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {//특정 키값 검색
  //바이너리 서치트리 탐색
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {//트리에서 최소값찾기, 맨 왼쪽 맨 아래 노드
  //왼쪽 끝까지 탐색
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {//트리에서 최대값찾기, 맨 오른쪽 맨 아래 노드
  //오른쪽 끝까지 탐색
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {//트리에서 특정 노드 삭제
  //삭제 케이스들 돌리기, rb트리 균형 유지를 위해 fix하기
  // TODO: implement erase
  return 0;//성공시 0, 실패시 -1?
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {//트리의 키값들 오름차순으로 어레이화시킴
  //순회하면서 어레이에 저장
  // TODO: implement to_array
  return 0;
}