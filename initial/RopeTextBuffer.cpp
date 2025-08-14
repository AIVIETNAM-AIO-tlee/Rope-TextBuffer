#include "RopeTextBuffer.h"
// MSSV: 2252749
// ------------------ Implement stack for action list ------------------

template <typename T>
Stack<T>::Stack()
{
    head = nullptr;
    tail = nullptr;
    count = 0;
}
template <typename T>
Stack<T>::~Stack()
{
    while (!isEmpty())
    {
        pop();
    }
}
template <typename T>
void Stack<T>::push(T data)
{
    Node *newNode = new Node(data);
    if (isEmpty())
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->setNext(newNode);
        newNode->setPrev(tail);
        tail = newNode;
    }
    count++;
}

template <typename T>
void Stack<T>::pop()
{
    if (isEmpty())
    {
        return;
    }
    Node *temp = tail;
    tail = tail->getPrev();
    if (tail)
    {
        tail->setNext(nullptr);
    }
    else
    {
        head = nullptr;
    }
    delete temp;
    count--;
}

template <typename T>
T Stack<T>::top() const
{
    if (isEmpty())
    {
        return T();
    }
    return tail->getData();
}

template <typename T>
bool Stack<T>::isEmpty() const
{
    return count == 0 || head == nullptr;
}

template <typename T>
void Stack<T>::clear()
{
    while (!isEmpty())
    {
        pop();
    }
}

// ---------------------------------------------------------------------
void throwOutOfIndex()
{
    throw out_of_range("Index is invalid!");
}

void throwOutOfLength()
{
    throw out_of_range("Length is invalid!");
}

// ----------------- DoublyLinkedList -----------------
Rope::Rope()
{
    // TODO
    this->root = nullptr;
}

Rope::~Rope()
{
    // TODO
    destroy(root);
}

int Rope::length() const
{
    return getTotalLength(this->root);
}

bool Rope::empty() const
{
    return root == nullptr;
}

char Rope::charAt(int index) const
{
    return charAt(root, index);
}

string Rope::substring(int start, int length) const
{
    if (start < 0 || start >= this->length())
    {
        throwOutOfIndex();
    }

    if (length < 0 || start + length > this->length())
    {
        throwOutOfLength();
    }

    string result = "";
    for (int i = 0; i < length; i++)
    {
        result += charAt(start + i);
    }
    return result;
}

void Rope::insert(int index, const string &s)
{
    if (index < 0 || index > length())
    {
        throwOutOfIndex();
    }

    if (s.empty())
    {
        return;
    }

    Node *R1, *R2;
    split(root, index, R1, R2);
    for (int i = 0; i < (int)s.length(); i += Rope::getChunkSize())
    {
        string chunk = s.substr(i, min(Rope::getChunkSize(), (int)s.length() - i));
        Node *chunkNode = new Node(chunk);
        R1 = concatNodes(R1, chunkNode);
    }
    root = concatNodes(R1, R2);
}

void Rope::deleteRange(int start, int length)
{
    if (start < 0 || start >= this->length())
    {
        throwOutOfIndex();
    }

    if (length < 0 || length > this->length())
    {
        throwOutOfLength();
    }

    int end = min(start + length, this->length());
    if (start >= end)
    {
        return;
    }
    Node *R1, *R2, *R3, *R4;
    // chỗ này tách tại index bắt đầu delete
    split(root, start, R1, R2);

    split(R2, length, R3, R4);

    destroy(R3);

    root = concatNodes(R1, R4);
}

string Rope::toString() const
{
    if (root == nullptr)
    {
        return "";
    }
    return toString(root);
}

string Rope::traversePreOrder() const
{
    string result = "";
    if (root == nullptr)
    {
        return result;
    }

    traversePreOrder(root, result);

    if (!result.empty() && result.back() == ' ')
    {
        result.pop_back();
    }

    return result;
}

string Rope::traversePreOrder(Node *node, string &result) const
{
    if (node == nullptr)
    {
        return result;
    }

    if (node->isLeaf())
    {
        result += node->data + " ";
    }
    else
    {
        result += to_string(node->weight) + " ";
    }

    traversePreOrder(node->left, result);

    traversePreOrder(node->right, result);

    return result;
}

// TODO: implement other methods of DoublyLinkedList

int Rope::height(Node *node) const
{
    if (node == nullptr)
    {
        return 0;
    }
    return node->height;
}

int Rope::getTotalLength(Node *node) const
{
    if (node == nullptr)
    {
        return 0;
    }

    if (node->isLeaf())
    {
        return node->data.length();
    }

    return node->weight + getTotalLength(node->right);
}

void Rope::update(Node *node)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->isLeaf())
    {
        node->weight = node->data.length();
        node->height = 1;
        node->balance = Node::EH;
        return;
    }

    int leftHeight = height(node->left);
    int rightHeight = height(node->right);
    node->height = max(leftHeight, rightHeight) + 1;

    node->weight = getTotalLength(node->left);

    if (leftHeight > rightHeight)
    {
        node->balance = Node::LH;
    }
    else if (leftHeight < rightHeight)
    {
        node->balance = Node::RH;
    }
    else
    {
        node->balance = Node::EH;
    }
}

Rope::Node *Rope::rotateLeft(Node *x)
{
    Node *temp_1 = x->right;
    Node *temp_2 = temp_1->left;

    temp_1->left = x;
    x->right = temp_2;

    update(x);
    update(temp_1);
    return temp_1;
}

Rope::Node *Rope::rotateRight(Node *y)
{
    Node *temp_1 = y->left;
    Node *temp_2 = temp_1->right;

    temp_1->right = y;
    y->left = temp_2;

    update(y);
    update(temp_1);
    return temp_1;
}

Rope::Node *Rope::rebalance(Node *node)
{

    if (node == nullptr)
    {
        return node;
    }

    update(node);

    int leftHeight = height(node->left);
    int rightHeight = height(node->right);

    int balanceFactor = leftHeight - rightHeight;

    // if left is higher
    if (balanceFactor > 1)
    {
        int leftLeftHeight = height(node->left->left);
        int leftRightHeight = height(node->left->right);
        if (leftLeftHeight >= leftRightHeight)
        {
            // left-left case
            return rotateRight(node);
        }
        else
        {
            // right of left case
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
    }
    else if (balanceFactor < -1)
    {
        int rightLeftHeight = height(node->right->left);
        int rightRightHeight = height(node->right->right);
        if (rightRightHeight >= rightLeftHeight)
        {
            // right-right case
            return rotateLeft(node);
        }
        else
        {
            // left of right case
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
    }

    return node;
}

void Rope::split(Node *node, int index, Node *&outLeft, Node *&outRight)
{
    if (node == nullptr)
    {
        outLeft = nullptr;
        outRight = nullptr;
        return;
    }

    if (node->isLeaf())
    {
        if (index == 0)
        {
            outLeft = nullptr;
            outRight = new Node(node->data);
        }
        else if (index >= (int)node->data.length())
        {
            outLeft = new Node(node->data);
            outRight = nullptr;
        }
        else
        {
            outLeft = new Node(node->data.substr(0, index));
            outRight = new Node(node->data.substr(index));
        }

        return;
    }

    if (index <= node->weight)
    {
        split(node->left, index, outLeft, node->left);
        outRight = concatNodes(node->left, node->right);
    }
    else
    {
        split(node->right, index - node->weight, node->right, outRight);
        outLeft = concatNodes(node->left, node->right);
    }
}

Rope::Node *Rope::concatNodes(Node *left, Node *right)
{
    if (left == nullptr)
    {
        return right;
    }
    if (right == nullptr)
    {
        return left;
    }

    Node *newNode = new Node("");
    newNode->left = left;
    newNode->right = right;

    update(newNode);
    return rebalance(newNode);
}

char Rope::charAt(Node *node, int index) const
{
    if (node == nullptr || index < 0 || index >= getTotalLength(node))
    {
        throwOutOfIndex();
    }

    if (node->isLeaf())
    {
        return node->data[index];
    }

    if (index < node->weight)
    {
        return charAt(node->left, index);
    }
    else
    {
        return charAt(node->right, index - node->weight);
    }
}

string Rope::toString(Node *node) const
{
    if (node == nullptr)
    {
        return "";
    }
    if (node->isLeaf())
    {
        return node->data;
    }
    return toString(node->left) + toString(node->right);
}

void Rope::destroy(Node *&node)
{
    if (node == nullptr)
    {
        return;
    }

    destroy(node->left);
    destroy(node->right);
    delete node;
    node = nullptr;
}

// ----------------- RopeTextBuffer -----------------
RopeTextBuffer::RopeTextBuffer()
{
    // TODO
    this->cursorPos = 0;
    this->history = new HistoryManager();
}

RopeTextBuffer::~RopeTextBuffer()
{
    // TODO
    delete history;
}

// TODO: implement other methods of TextBuffer

void RopeTextBuffer::insert(const string &s)
{
    if (s.empty())
    {
        return;
    }

    int oldCursor = cursorPos;
    rope.insert(cursorPos, s);
    cursorPos += s.length();

    HistoryManager::Action action;
    action.actionName = "insert";
    action.cursorBefore = oldCursor;
    action.cursorAfter = cursorPos;
    action.data = s;
    history->addAction(action);
}

void RopeTextBuffer::deleteRange(int length)
{
    if (length <= 0)
    {
        return;
    }

    if (length > rope.length())
    {
        throwOutOfLength();
    }

    int startPos = cursorPos;
    string deletedData = rope.substring(startPos, length);

    rope.deleteRange(startPos, length);
    cursorPos = startPos;

    HistoryManager::Action action;
    action.actionName = "delete";
    action.cursorBefore = startPos;
    action.cursorAfter = cursorPos;
    action.data = deletedData;
    history->addAction(action);
}

void RopeTextBuffer::replace(int length, const string &s)
{
    if (length < 0 || length > rope.length())
    {
        throwOutOfLength();
    }

    int startPos = cursorPos;
    string oldData = "";
    if (length > 0)
    {
        oldData = rope.substring(startPos, length);
        rope.deleteRange(startPos, length);
    }

    rope.insert(startPos, s);
    cursorPos = startPos + s.length();

    HistoryManager::Action action;
    action.actionName = "replace";
    action.cursorBefore = startPos;
    action.cursorAfter = cursorPos;
    action.data = oldData;
    action.newData = s;
    history->addAction(action);
}

void RopeTextBuffer::moveCursorTo(int index)
{
    if (index < 0 || index > rope.length())
    {
        throwOutOfIndex();
    }

    if (cursorPos == index)
    {
        return;
    }
    int oldCursor = cursorPos;
    cursorPos = index;

    HistoryManager::Action action;
    action.actionName = "move";
    action.cursorBefore = oldCursor;
    action.cursorAfter = cursorPos;
    action.data = "J";
    history->addAction(action);
}

void RopeTextBuffer::moveCursorLeft()
{
    if (cursorPos == 0)
    {
        throw cursor_error();
    }

    int oldCursor = cursorPos;
    cursorPos--;

    HistoryManager::Action action;
    action.actionName = "move";
    action.cursorBefore = oldCursor;
    action.cursorAfter = cursorPos;
    action.data = "L";
    history->addAction(action);
}

void RopeTextBuffer::moveCursorRight()
{
    if (cursorPos >= rope.length())
    {
        throw cursor_error();
    }

    int oldCursor = cursorPos;
    cursorPos++;

    HistoryManager::Action action;
    action.actionName = "move";
    action.cursorBefore = oldCursor;
    action.cursorAfter = cursorPos;
    action.data = "R";
    history->addAction(action);
}

int RopeTextBuffer::getCursorPos() const
{
    return cursorPos;
}

string RopeTextBuffer::getContent() const
{
    return rope.toString();
}

int RopeTextBuffer::findFirst(char c) const
{
    string content = rope.toString();
    for (int i = 0; i < (int)content.length(); i++)
    {
        if (content[i] == c)
        {
            return i;
        }
    }
    return -1;
}

int *RopeTextBuffer::findAll(char c) const
{
    string content = rope.toString();
    int count = 0;

    for (int i = 0; i < (int)content.length(); i++)
    {
        if (content[i] == c)
        {
            count++;
        }
    }

    if (count == 0)
    {
        return nullptr;
    }

    int *result = new int[count];
    int index = 0;

    for (int i = 0; i < (int)content.length(); i++)
    {
        if (content[i] == c)
        {
            result[index++] = i;
        }
    }

    return result;
}

void RopeTextBuffer::undo()
{
    if (!history->canUndo())
    {
        return;
    }
    HistoryManager::Action actionToUndo = history->current->action;

    if (actionToUndo.actionName == "insert")
    {
        rope.deleteRange(actionToUndo.cursorBefore, actionToUndo.data.length());
        cursorPos = actionToUndo.cursorBefore;
    }
    else if (actionToUndo.actionName == "delete")
    {
        rope.insert(actionToUndo.cursorBefore, actionToUndo.data);
        cursorPos = actionToUndo.cursorBefore;
    }
    else if (actionToUndo.actionName == "replace")
    {
        rope.deleteRange(actionToUndo.cursorBefore, actionToUndo.newData.length());
        rope.insert(actionToUndo.cursorBefore, actionToUndo.data);
        cursorPos = actionToUndo.cursorBefore;
    }
    else if (actionToUndo.actionName == "move")
    {
        cursorPos = actionToUndo.cursorBefore;
    }
    // đưa về action trước
    history->actionStackUndo.push(actionToUndo);
    history->current = history->current->prev;
}

void RopeTextBuffer::redo()
{
    if (!history->canRedo())
    {
        return;
    }

    HistoryManager::Action actionToRedo = history->actionStackUndo.top();
    history->actionStackUndo.pop();

    if (history->current == nullptr)
    {
        history->current = history->actionHead;
    }
    else
    {
        history->current = history->current->next;
    }

    if (actionToRedo.actionName == "insert")
    {
        rope.insert(actionToRedo.cursorBefore, actionToRedo.data);
        cursorPos = actionToRedo.cursorAfter;
    }
    else if (actionToRedo.actionName == "delete")
    {
        rope.deleteRange(actionToRedo.cursorBefore, actionToRedo.data.length());
        cursorPos = actionToRedo.cursorAfter;
    }
    else if (actionToRedo.actionName == "replace")
    {
        rope.deleteRange(actionToRedo.cursorBefore, actionToRedo.data.length());
        rope.insert(actionToRedo.cursorBefore, actionToRedo.newData);
        cursorPos = actionToRedo.cursorAfter;
    }
    else if (actionToRedo.actionName == "move")
    {
        cursorPos = actionToRedo.cursorAfter;
    }
}

void RopeTextBuffer::printHistory() const
{
    history->printHistory();
}

void RopeTextBuffer::clear()
{
    rope.deleteRange(0, rope.length());
    cursorPos = 0;
    delete history;
    history = new HistoryManager();
}

// ----------------- HistoryManager -----------------
RopeTextBuffer::HistoryManager::HistoryManager()
{
    // TODO
    this->actionHead = nullptr;
    this->actionTail = nullptr;
    this->current = nullptr;
    this->actionCount = 0;
}

RopeTextBuffer::HistoryManager::~HistoryManager()
{
    // TODO
    Node *curr = actionHead;
    while (curr != nullptr)
    {
        Node *temp = curr;
        curr = curr->next;
        delete temp;
    }

    actionHead = nullptr;
    actionTail = nullptr;
    current = nullptr;
    actionCount = 0;
}

// TODO: implement other methods of HistoryManager
void RopeTextBuffer::HistoryManager::addAction(const Action &a)
{
    Node *newNode = new Node(a);
    if (actionHead == nullptr)
    {
        actionHead = actionTail = newNode;
    }
    else
    {
        actionTail->next = newNode;
        newNode->prev = actionTail;
        actionTail = newNode;
    }

    current = actionTail;
    if (a.actionName == "insert" || a.actionName == "replace")
    {
        actionStackUndo.clear();
    }

    actionCount++;
}

bool RopeTextBuffer::HistoryManager::canUndo() const
{
    if (current == nullptr)
    {
        return false;
    }
    return true;

    // if (actionStackUndo.isEmpty())
    // {
    //     return false;
    // }
    // return true;
}

bool RopeTextBuffer::HistoryManager::canRedo() const
{
    // return (current && current->next) || (current == nullptr && actionHead != nullptr);
    if (actionStackUndo.isEmpty())
    {
        return false;
    }
    return true;
}

void RopeTextBuffer::HistoryManager::printHistory() const
{
    string historyStr = "[";
    Node *curr = actionHead;
    while (curr != nullptr)
    {
        historyStr += "(" + curr->action.actionName + ", " + to_string(curr->action.cursorBefore) + ", " + to_string(curr->action.cursorAfter) + ", " + curr->action.data + ")";
        if (curr->next != nullptr)
        {
            historyStr += ", ";
        }
        curr = curr->next;
    }
    historyStr += "]";
    cout << historyStr << endl;
}

template class Stack<RopeTextBuffer::HistoryManager::Action>;