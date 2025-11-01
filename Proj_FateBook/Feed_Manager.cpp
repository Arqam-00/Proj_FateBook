#include "Feed_Manager.h"


FeedManager::FeedManager() {}
FeedManager::~FeedManager() {}

void FeedManager::AddToFeed(Post* P) { FeedHeap_.push(P); }
bool FeedManager::IsEmpty() const { return FeedHeap_.empty(); }
Post* FeedManager::PeekTop() const { if (FeedHeap_.empty()) return nullptr; return FeedHeap_.top(); }
void FeedManager::PopTop() { if (!FeedHeap_.empty()) FeedHeap_.pop(); }
void FeedManager::Clear() { while (!FeedHeap_.empty()) FeedHeap_.pop(); }
size_t FeedManager::GetSize() const { return FeedHeap_.size(); }
