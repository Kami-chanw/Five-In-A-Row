#include "ahocorasick.h"
#include <queue>

AhoCorasick::AhoCorasick(const std::vector<Pattern>& patterns) : patterns(patterns) {
    root = new TrieNode();
    buildTrie();
    buildFailureLinks();
}

AhoCorasick::AhoCorasick(const AhoCorasick& other) : patterns(other.patterns) {
    root = cloneTrie(other.root);
    buildFailureLinks();
}

AhoCorasick::AhoCorasick(AhoCorasick&& other) noexcept
    : root(other.root), patterns(std::move(other.patterns)) {
    other.root = nullptr;
}

AhoCorasick& AhoCorasick::operator=(const AhoCorasick& other) {
    if (this != &other) {
        destroyTrie(root);
        root     = cloneTrie(other.root);
        patterns = other.patterns;
        buildFailureLinks();
    }
    return *this;
}

AhoCorasick& AhoCorasick::operator=(AhoCorasick&& other) noexcept {
    if (this != &other) {
        destroyTrie(root);
        root       = other.root;
        patterns   = std::move(other.patterns);
        other.root = nullptr;
    }
    return *this;
}

AhoCorasick::~AhoCorasick() { destroyTrie(root); }

int AhoCorasick::search(const std::string& text) {
    TrieNode* currentNode = root;
    int       totalScore  = 0;

    for (size_t i = 0; i < text.length(); i++) {
        int index = text[i] - '0';

        while (currentNode != root && currentNode->children[index] == nullptr) {
            currentNode = currentNode->fail;
        }

        if (currentNode->children[index] != nullptr) {
            currentNode = currentNode->children[index];

            if (currentNode->isEndOfPattern)
                totalScore += patterns[currentNode->patternIndex].score;
        }
    }
    return totalScore;
}

AhoCorasick::TrieNode* AhoCorasick::cloneTrie(TrieNode* node) {
    if (node == nullptr) {
        return nullptr;
    }

    TrieNode* cloneNode       = new TrieNode{};
    cloneNode->isEndOfPattern = node->isEndOfPattern;
    cloneNode->patternIndex   = node->patternIndex;

    for (int i = 0; i < MaxChildren; i++) {
        if (node->children[i] != nullptr) {
            cloneNode->children[i] = cloneTrie(node->children[i]);
        }
    }

    return cloneNode;
}

void AhoCorasick::destroyTrie(TrieNode* node) {
    if (node) {
        for (int i = 0; i < MaxChildren; ++i)
            destroyTrie(node->children[i]);
        delete node;
    }
}

void AhoCorasick::buildTrie() {
    for (size_t i = 0; i < patterns.size(); i++) {
        const std::string& pattern     = patterns[i].pattern;
        TrieNode*          currentNode = root;

        for (char c : pattern) {
            int index = c - '0';
            if (currentNode->children[index] == nullptr) {
                currentNode->children[index] = new TrieNode();
            }
            currentNode = currentNode->children[index];
        }

        currentNode->isEndOfPattern = true;
        currentNode->patternIndex   = i;
    }
}

void AhoCorasick::buildFailureLinks() {
    std::queue<TrieNode*> nodeQueue;
    root->fail = root;
    for (int i = 0; i < MaxChildren; i++) {
        if (root->children[i] != nullptr) {
            root->children[i]->fail = root;
            nodeQueue.push(root->children[i]);
        }
    }

    while (!nodeQueue.empty()) {
        TrieNode* currentNode = nodeQueue.front();
        nodeQueue.pop();

        for (int i = 0; i < MaxChildren; i++) {
            TrieNode* childNode = currentNode->children[i];
            if (childNode != nullptr) {
                TrieNode* failNode = currentNode->fail;

                while (failNode != root && failNode->children[i] == nullptr)
                    failNode = failNode->fail;

                if (failNode->children[i] != nullptr)
                    failNode = failNode->children[i];

                childNode->fail = failNode;

                if (failNode->isEndOfPattern) {
                    childNode->isEndOfPattern = true;
                    childNode->patternIndex   = failNode->patternIndex;
                }

                nodeQueue.push(childNode);
            }
        }
    }
}

AhoCorasick::TrieNode::TrieNode() {
  std::fill_n(children, MaxChildren, nullptr);
}
