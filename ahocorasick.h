#ifndef AHOCORASICK_H
#define AHOCORASICK_H

#include <string>
#include <vector>

struct Pattern {
    std::string pattern;
    int         score;

    Pattern(const std::string& str, int value) : pattern(str), score(value) {}
};

class AhoCorasick {
private:
    static constexpr int MaxChildren = 2;

    struct TrieNode {
        TrieNode* children[MaxChildren];
        TrieNode* fail           = nullptr;
        bool      isEndOfPattern = false;
        int       patternIndex   = -1;

        TrieNode();
    };

    TrieNode*            root;
    std::vector<Pattern> patterns;

public:
    AhoCorasick(const std::vector<Pattern>& patterns);

    AhoCorasick(const AhoCorasick& other);
    AhoCorasick(AhoCorasick&& other) noexcept;

    AhoCorasick& operator=(const AhoCorasick& other);
    AhoCorasick& operator=(AhoCorasick&& other) noexcept;

    ~AhoCorasick();

    int search(const std::string& text);

private:
    TrieNode* cloneTrie(TrieNode* node);

    void destroyTrie(TrieNode* node);
    void buildTrie();
    void buildFailureLinks();
};

#endif  // AHOCORASICK_H
