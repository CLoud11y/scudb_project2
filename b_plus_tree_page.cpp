/**
 * b_plus_tree_page.cpp
 */
#include "page/b_plus_tree_page.h"

namespace scudb {

/*
 * Helper methods to get/set page type
 * Page type enum class is defined in b_plus_tree_page.h
 */
bool BPlusTreePage::IsLeafPage() const { 
  return page_type_ == IndexPageType::LEAF_PAGE; //判断是否为叶节点
}

bool BPlusTreePage::IsRootPage() const { 
  return parent_page_id_ == INVALID_PAGE_ID; //判断是否为根节点
}

void BPlusTreePage::SetPageType(IndexPageType type) { 
  page_type_ = type;
}

/*
 * Helper methods to get/set size (number of key/value pairs stored in that
 * page)
 */
int BPlusTreePage::GetSize() const { 
  return size_;
}

void BPlusTreePage::SetSize(int size) { 
  size_ = size;
}

void BPlusTreePage::IncreaseSize(int amount) { 
  size_ += amount; 
}

/*
 * Helper methods to get/set max size (capacity) of the page
 */
int BPlusTreePage::GetMaxSize() const { 
  return max_size_; 
}

void BPlusTreePage::SetMaxSize(int size) { 
  max_size_ = size; 
}

/*
 * Helper method to get min page size
 * Generally, min page size == max page size / 2
 * With n = 4 in our example B+-tree, each leaf must contain at least 2 values, and at most 3 values.
 */
int BPlusTreePage::GetMinSize() const {
  if(!IsRootPage()) {
    return (max_size_ ) / 2;
  }
  else {
    //为什么最小不是零：1时仅含一个指针没有key, 相当于空
    //根节点且是叶结点有可能空所以是1，是根节点不是叶结点就是2
    return IsLeafPage() ? 1 : 2;
  }
  
}

/*
 * Helper methods to get/set parent page id
 */
page_id_t BPlusTreePage::GetParentPageId() const { 
  return parent_page_id_; 
}

void BPlusTreePage::SetParentPageId(page_id_t page_id) { 
  parent_page_id_ = page_id; 
}

/*
 * Helper methods to get/set self page id
 */
page_id_t BPlusTreePage::GetPageId() const { 
  return page_id_; 
}

void BPlusTreePage::SetPageId(page_id_t id) { 
  page_id_ = id; 
}

/*
 * Helper methods to set lsn
 */
void BPlusTreePage::SetLSN(lsn_t lsn) { 
  lsn_ = lsn; 
}

/* for concurrent index */
bool BPlusTreePage::IsSafe(OpType op) {
  int size = GetSize();
  if (op == OpType::INSERT) {
    return size < GetMaxSize();
  }
  int minSize = GetMinSize() + 1;
  if (op == OpType::DELETE) {
    return (IsLeafPage()) ? size >= minSize : size > minSize;
  }
  
}

} // namespace scudb
