#include "RopeTextBuffer.h"

void throwOutOfIndex()
{
    throw out_of_range("Index is invalid!");
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
    return getTotalLength(root);
}

bool Rope::empty() const
{
    return root == nullptr;
}

char Rope::charAt(int index) const
{
    if (index < 0 || index >= length())
    {
        throwOutOfIndex();
    }
    return charAt(root, index);
}

string Rope::substring(int start, int length) const
{
    if (start < 0 || start >= this->length() || length < 0 || start + length > this->length())
    {
        throwOutOfIndex();
    }

    string result = "";
    for (int i = start; i < start + length; i++)
    {
        result += charAt(i);
    }
    return result;
}

void Rope::insert(int index, const string &s)
{
    if (s.empty())
    {
        return;
    }

    if (index < 0 || index > length())
    {
        throwOutOfIndex();
    }

    Node *insertRoot = nullptr;
    for (int i = 0; i < s.length(); i += Rope::getChunkSize())
    {
        string chunk = s.substr(i, min(Rope::getChunkSize(), (int)s.length() - i));
        Node *chunkNode = new Node(chunk);
        update(chunkNode);
        insertRoot = concatNodes(insertRoot, chunkNode);
    }

    if (root == nullptr)
    {
        root = insertRoot;
        return;
    }

    Node *left, *right;
    split(root, index, left, right);

    root = concatNodes(concatNodes(left, insertRoot), right);
}

void Rope::deleteRange(int start, int length)
{
    if (start < 0 || start >= this->length() || length <= 0)
    {
        throwOutOfIndex();
    }

    int end = min(start + length, this->length());
    if (start >= end)
    {
        return;
    }

    Node *left, *middle, *right;

    split(root, start, left, right);

    split(right, end - start, middle, right);

    destroy(middle);

    root = concatNodes(left, right);
}

string Rope::toString() const
{
    if (root == nullptr)
    {
        return "";
    }
    return toString(root);
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

    return getTotalLength(node->left) + getTotalLength(node->right);
}

void Rope::update(Node *node)
{
    if (node == nullptr)
    {
        return;
    }

    int leftHeight = height(node->left);
    int rightHeight = height(node->right);
    node->height = max(leftHeight, rightHeight) + 1;

    node->weight = getTotalLength(node->left) + getTotalLength(node->right);

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
    int leftHeight = height(node->left);
    int rightHeight = height(node->right);

    int balanceFactor = leftHeight - rightHeight;

    // if left is higher
    if (balanceFactor >= 1)
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
    else if (balanceFactor <= -1)
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
        update(outLeft);
        update(outRight);
        return;
    }

    if (index <= node->weight)
    {
        split(node->left, index, outLeft, outRight);
        if (outRight != nullptr)
        {
            outRight = concatNodes(outRight, node->right);
        }
        else
        {
            outRight = node->right;
        }
    }
    else
    {
        split(node->right, index - node->weight, outLeft, outRight);
        if (outLeft != nullptr)
        {
            outLeft = concatNodes(node->left, outLeft);
        }
        else
        {
            outLeft = node->left;
        }
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

    if (cursorPos < length)
    {
        throwOutOfIndex();
    }

    int startPos = cursorPos - length;
    string deletedData = rope.substring(startPos, length);

    rope.deleteRange(startPos, length);
    cursorPos = startPos;

    HistoryManager::Action action;
    action.actionName = "delete";
    action.cursorBefore = cursorPos + length;
    action.cursorAfter = cursorPos;
    action.data = deletedData;
    history->addAction(action);
}

void RopeTextBuffer::replace(int length, const string &s)
{
    if (length < 0)
    {
        throwOutOfIndex();
    }

    if (cursorPos < length)
    {
        throwOutOfIndex();
    }

    int startPos = cursorPos - length;
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
    action.cursorBefore = startPos + length;
    action.cursorAfter = cursorPos;
    action.data = oldData + "|" + s; // Store both old and new data
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

    cursorPos = index;
}

void RopeTextBuffer::moveCursorLeft()
{
    if (cursorPos == 0)
    {
        throw cursor_error();
    }

    cursorPos--;
}

void RopeTextBuffer::moveCursorRight()
{
    if (cursorPos >= rope.length() - 1)
    {
        throw cursor_error();
    }

    cursorPos++;
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

    // First pass: count occurrences
    for (int i = 0; i < (int)content.length(); i++)
    {
        if (content[i] == c)
        {
            count++;
        }
    }

    // Allocate array with size + 1 (first element stores count)
    int *result = new int[count + 1];
    result[0] = count;

    // Second pass: store positions
    int index = 1;
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
    if (history->current == nullptr)
    {
        return;
    }

    if (!history->canUndo())
    {
        return;
    }
    HistoryManager::Action actionToUndo = history->current->action;

    if (actionToUndo.actionName == "insert")
    {
        cursorPos = actionToUndo.cursorBefore;
    }
    else if (actionToUndo.actionName == "delete")
    {
        cursorPos = actionToUndo.cursorBefore;
    }
    else if (actionToUndo.actionName == "move")
    {
        cursorPos = actionToUndo.cursorBefore;
    }

    history->current = history->current->prev;
}

void RopeTextBuffer::redo()
{
    HistoryManager::Action actionToRedo;
    if (history->current == nullptr)
    {
        if (history->actionHead == nullptr)
        {
            return;
        }

        history->current = history->actionHead;
        actionToRedo = history->current->action;
    }
    else if (history->current->next == nullptr)
    {
        return;
    }
    else
    {
        if (history->canRedo())
        {
            history->current = history->current->next;
            actionToRedo = history->current->action;
        }
    }

    if (actionToRedo.actionName == "insert")
    {
        cursorPos = actionToRedo.cursorAfter;
    }
    else if (actionToRedo.actionName == "delete")
    {
        cursorPos = actionToRedo.cursorAfter;
    }
    else if (actionToRedo.actionName == "move")
    {
        cursorPos = actionToRedo.cursorAfter;
    }

    history->current = history->current->prev;
}

void RopeTextBuffer::printHistory() const
{
    history->printHistory();
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
    clearRedoHistory();
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

    current = newNode;
    actionCount++;
}

bool RopeTextBuffer::HistoryManager::canUndo() const
{
    if (current->prev)
    {
        return true;
    }
    return false;
}

bool RopeTextBuffer::HistoryManager::canRedo() const
{
    if (current && current->next)
    {
        return true;
    }
    return false;
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

void RopeTextBuffer::HistoryManager::clearRedoHistory()
{
    if (current == nullptr)
    {
        return;
    }

    Node *nodeToDelete = current->next;
    while (nodeToDelete != nullptr)
    {
        Node *temp = nodeToDelete;
        nodeToDelete = nodeToDelete->next;
        delete temp;
        actionCount--;
    }

    current->next = nullptr;
    actionTail = current;
}