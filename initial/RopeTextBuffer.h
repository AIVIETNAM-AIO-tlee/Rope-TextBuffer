#ifndef ROPE_TEXTBUFFER_H
#define ROPE_TEXTBUFFER_H
// MSSV: 2252749
#include "main.h"

/*
    Student can define other list data structures here
*/

template <typename T>
class Stack
{
public:
    class Node;

protected:
    Node *head;
    Node *tail;
    int count;

public:
    Stack();
    ~Stack();

    void push(T data);
    void pop();
    T top() const;
    bool isEmpty() const;
    void clear();

    Node *getHead() const { return head; }
    void setHead(Node *newHead) { head = newHead; }
    Node *getTail() const { return tail; }
    void setTail(Node *newTail) { tail = newTail; }
    void setCount(int newCount) { count = newCount; }

public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        friend class Stack<T>;

    public:
        Node()
        {
            this->next = nullptr;
            this->prev = nullptr;
        }

        Node(const T &data)
        {
            this->data = data;
            this->next = nullptr;
            this->prev = nullptr;
        }

        Node *getNext() const
        {
            return next;
        }

        void setNext(Node *next)
        {
            this->next = next;
        }

        void setPrev(Node *prev)
        {
            this->prev = prev;
        }

        Node *getPrev() const
        {
            return prev;
        }
        T getData() const
        {
            return data;
        }
    };
};

/**
 * Rope (AVL-based, fixed leaf chunk size = 8)
 */
class Rope
{
public:
    static const int CHUNK_SIZE = 8;
    static int getChunkSize()
    {
        return CHUNK_SIZE;
    }

private:
    class Node
    {
    public:
        enum BalanceFactor
        {
            LH = 1,
            EH = 0,
            RH = -1
        };

    private:
        Node *left;
        Node *right;
        string data;
        int weight;
        int height;
        BalanceFactor balance;

        Node();
        explicit Node(const string &s)
        {
            this->left = nullptr;
            this->right = nullptr;
            this->data = s;
            this->weight = 0;
            this->height = 0;
            this->balance = EH;
        };
        bool isLeaf() const
        {
            if (this->left || this->right)
            {
                return false;
            }

            return true;
        };
        friend class Rope;
    };

    Node *root;

    int height(Node *node) const;
    int getTotalLength(Node *node) const;
    void update(Node *node);
    Node *rotateLeft(Node *x);
    Node *rotateRight(Node *y);
    Node *rebalance(Node *node);
    void split(Node *node, int index, Node *&outLeft, Node *&outRight);
    Node *concatNodes(Node *left, Node *right);
    char charAt(Node *node, int index) const;
    string toString(Node *node) const;
    string traversePreOrder(Node *node, string &result) const;
    void destroy(Node *&node);

public:
    Rope();
    ~Rope();

    int length() const;
    bool empty() const;
    char charAt(int index) const;
    string substring(int start, int length) const;
    void insert(int index, const string &s);
    void deleteRange(int start, int length);
    string toString() const;

    // ------------------------

    string traversePreOrder() const;

#ifdef TESTING
    friend class TestHelper;
#endif
};

class RopeTextBuffer
{
public:
    class HistoryManager;

private:
    Rope rope;
    int cursorPos;
    HistoryManager *history;

public:
    RopeTextBuffer();
    ~RopeTextBuffer();

    void insert(const string &s);
    void deleteRange(int length);
    void replace(int length, const string &s);
    void moveCursorTo(int index);
    void moveCursorLeft();
    void moveCursorRight();
    int getCursorPos() const;
    string getContent() const;
    int findFirst(char c) const;
    int *findAll(char c) const;
    void undo();
    void redo();
    void printHistory() const;
    void clear();
#ifdef TESTING
    friend class TestHelper;
#endif
};

class RopeTextBuffer::HistoryManager
{
public:
    struct Action
    {
        string actionName;
        int cursorBefore;
        int cursorAfter;
        string data;
        string newData; // dùng cho undo/redo
    };

    class Node
    {
    public:
        Action action;
        Node *next;
        Node *prev;

    public:
        Node(const Action &a) : action(a), next(nullptr), prev(nullptr) {};
        ~Node() {};
        friend class HistoryManger;
    };
    // TODO: may provide some attributes
private:
    Node *actionHead;
    Node *actionTail;
    Node *current;
    Stack<Action> actionStackUndo;
    // Stack<Action> ActionStack;
    int actionCount;

public:
    HistoryManager();
    ~HistoryManager();
    void addAction(const Action &a);
    bool canUndo() const;
    bool canRedo() const;
    void printHistory() const;
    friend class RopeTextBuffer;
#ifdef TESTING
    friend class TestHelper;
#endif
};

#endif // ROPE_TEXTBUFFER_H
