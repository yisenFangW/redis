#include <climits>
#include <string>
#include <cstdlib>

//static const int MAX_LEVEL = 4;
//static const float PROBABILITY = 0.5;

// redis中跳跃表的实现

struct SkipNode {
    int _key;
    // 定义一个指针数组
    SkipNode **_forward;

    // 指针数组的初始化全部赋0，即nullptr
    SkipNode(int key, int level) : _key(key) {
        _forward = new SkipNode *[level + 1];
        memset(_forward, 0, sizeof(SkipNode *) * (level + 1));
    }

    ~SkipNode() {
        delete[] _forward;
    }
};

class SkipList {
public:
    explicit SkipList(int maxLevel, float p) : _maxLevel(maxLevel), _level(0), _p(p), _size(0) {
        // 初始化创建header的level为最大level，并将值赋为INT_MIN
        header = new SkipNode(INT_MIN, maxLevel);
    }

    ~SkipList() {
        delete header;
    }

    void addNode(int key);

    void delNode(int key);

    void searchNode(int key);

    void display();

    size_t getSize() const { return _size; }

private:
    int randomLevel();

private:
    int _maxLevel;
    SkipNode *header;
    float _p;
    int _level;
    size_t _size;
};