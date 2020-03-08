#include "SkipList.h"
#include <iostream>


void SkipList::addNode(int key) {
    // 用一个具体的update存储在每一层中，新node要存储的位置;
    SkipNode *update[_maxLevel + 1];
    SkipNode *current = header;
    memset(update, 0, sizeof(SkipList *) * (_maxLevel + 1));
    // 将现有的层中，需要插入新node的位置记录在update中;
    for (int i = _level; i >= 0; i--) {
        while (current->_forward[i] != nullptr && current->_forward[i]->_key < key)
            current = current->_forward[i];
        update[i] = current;
    }
    // 获取当前节点的后一个节点，即寻找的那个元素
    current = current->_forward[0];
    // 如果后一个为空或者后一个值不等于key,说明key还不再当前的跳表中,执行添加操作
    if (current == nullptr || current->_key != key) {
        // 获取一个随机的level
        int level = randomLevel();
        // 如果获取的随机level比当前的level大的话，将update[i]在[_level+1,level]之间赋值为header
        if (level > _level) {
            for (int i = _level + 1; i < level + 1; ++i)
                update[i] = header;
            _level = level;
        }
        std::cout << "level:" << level << std::endl;
        auto node = new SkipNode(key, level);
        // 新建值为key的node,在获取的随机level层链表都插入
        for (int i = level; i >= 0; i--) {
            node->_forward[i] = update[i]->_forward[i];
            update[i]->_forward[i] = node;
        }
        _size++;
        std::cout << "add succeed, key: " << key << std::endl;
    }
}

// 删除节点的操作大部分类似与添加节点
void SkipList::delNode(int key) {
    SkipNode *update[_maxLevel + 1];
    SkipNode *current = header;
    memset(update, 0, sizeof(SkipList *) * (_maxLevel + 1));
    for (int i = _level; i >= 0; --i) {
        while (current->_forward[i] != nullptr && current->_forward[i]->_key < key) {
            current = current->_forward[i];
        }
        update[i] = current;
    }
    current = current->_forward[0];
    // 判断下一个不为空，且下一个的值等于key
    if (current != nullptr && current->_key == key) {
        for (int i = 0; i <= _level; i++) {
            // 判断寻找的update[i]后一个是否为current,不是的话，就可以跳出循环了，因为是从0层开始的
            if (update[i]->_forward[i] != current)
                break;
            update[i]->_forward[i] = current->_forward[i];

        }
        delete current;
        while (_level > 0 && header->_forward[_level] == nullptr)
            _level--;
        --_size;
        std::cout << "del succeed, key: " << key << std::endl;
    }
}

// 遍历节点，并打印
void SkipList::display() {
    for (int i = _level; i >= 0; --i) {
        std::cout << "level:" << i << std::endl;
        SkipNode *node = header->_forward[i];
        while (node) {
            std::cout << node->_key << " ";
            node = node->_forward[i];
        }
        std::cout << std::endl;
    }
}

// 获取一个随机的level，可以根据传入的p值，层数越高，概率越小,概率为p^n
int SkipList::randomLevel() {
    float p = (float) random() / RAND_MAX;
    int level = 0;
    while (p < _p && level < _maxLevel) {
        ++level;
        p = (float) random() / RAND_MAX;
    }
    return level;
}

// 寻找跳表中是否有该节点，在添加和删除节点中，涉及到该步骤
void SkipList::searchNode(int key) {
    SkipNode *current = header;
    for (int i = _level; i >= 0; --i) {
        while (current->_forward[i] != nullptr && current->_forward[i]->_key < key)
            current = current->_forward[i];
    }
    current = current->_forward[0];
    if (current && current->_key == key) {
        std::cout << "Found key: " << key << std::endl;
    } else {
        std::cout << "Not Found key " << key << " in list" << std::endl;
    }
}

int main() {

    SkipList lst(3, 0.5);

    for (int i = 0; i < 50; ++i)
        lst.addNode(i);
    lst.display();

    std::cout << "SkipList size:" <<lst.getSize() << std::endl;
    lst.delNode(19);
    lst.display();

    lst.searchNode(20);
    lst.searchNode(300);
    std::cout << "SkipList size:" << lst.getSize() << std::endl;
}