#include "RopeTextBuffer.h"
#include <functional>
#ifdef _WIN32
#include <crtdbg.h>
#endif

// Color codes
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_PURPLE "\033[35m"
#define COLOR_RESET "\033[0m"

static int countOfPassedTests = 0;
static int countOfTestInSample = 0;

void evaluateTests(int numOfTests)
{
    if (countOfTestInSample == numOfTests)
    {
        countOfPassedTests++;
    }

    countOfTestInSample = 0;
}

template <typename T>
void assertEqual(const T &actual, const T &expected, int testNum, const string &msg = "")
{
    if (actual == expected)
    {
        cout << "Test " << testNum << ": " << COLOR_GREEN << "Passed" << COLOR_RESET;
        if (!msg.empty())
            cout << " - " << msg;
        cout << endl;
        countOfTestInSample++;
    }
    else
    {
        cout << "Test " << testNum << ": " << COLOR_RED << "Failed" << COLOR_RESET;
        if (!msg.empty())
            cout << " - " << msg;
        cout << "\n  Expected: " << expected << "\n  Actual:   " << actual << endl;
    }
}

void assertEqual(const string &actual, const char *expected, int testNum, const string &msg = "")
{
    assertEqual<string>(actual, string(expected), testNum, msg);
}
void assertEqual(const char *actual, const string &expected, int testNum, const string &msg = "")
{
    assertEqual<string>(string(actual), expected, testNum, msg);
}
void assertEqual(const char *actual, const char *expected, int testNum, const string &msg = "")
{
    assertEqual<string>(string(actual), string(expected), testNum, msg);
}

string captureOutput(std::function<void()> func)
{
    std::stringstream buffer;
    std::streambuf *oldCout = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());

    func();

    std::cout.rdbuf(oldCout);
    return buffer.str();
}

/*---------------------------------------- Test cua thay ----------------------------------------
-----------------------------------------------------------------------------------------------*/
void sample_01()
{
    // Test basic Rope construction and empty state
    Rope rope;
    assertEqual(rope.empty(), true, 1, "Empty state");
    assertEqual(rope.length(), 0, 1, "Length");
    evaluateTests(2);
}

void sample_02()
{
    // Test basic Rope insert and toString
    Rope rope;
    rope.insert(0, "Hello");
    assertEqual(rope.toString(), "Hello", 2, "Basic insert and toString");
    assertEqual(rope.length(), 5, 2, "Length");
    evaluateTests(2);
}

void sample_03()
{
    // Test Rope charAt operation
    Rope rope;
    rope.insert(0, "ABC");
    assertEqual(rope.charAt(1), 'B', 3, "charAt(1) should return 'B'");
    assertEqual(rope.charAt(0), 'A', 3, "charAt(0) should return 'A'");
    evaluateTests(2);
}

void sample_04()
{
    // Test RopeTextBuffer basic operations
    RopeTextBuffer tb;
    tb.insert("Hello");
    assertEqual(tb.getContent(), "Hello", 4, "TextBuffer insert");
    assertEqual(tb.getCursorPos(), 5, 4, "Cursor position");
    evaluateTests(2);
}

void sample_05()
{
    // Test RopeTextBuffer cursor movement
    RopeTextBuffer tb;
    tb.insert("ABC");
    tb.moveCursorLeft();
    tb.insert("X");
    assertEqual(tb.getContent(), "ABXC", 5, "Content insert when moving cursor");
    assertEqual(tb.getCursorPos(), 3, 5, "Cursor position");
    evaluateTests(2);
}

void sample_06()
{
    // Test RopeTextBuffer delete operation
    RopeTextBuffer tb;
    tb.insert("Hello");
    tb.moveCursorTo(2);
    tb.deleteRange(2);
    assertEqual(tb.getContent(), "Heo", 6, "Content delete after moving cursor");
    assertEqual(tb.getCursorPos(), 2, 6, "Cursor position");
    string historyOutput = captureOutput([&tb]()
                                         { tb.printHistory(); });

    string expectedHistory = "[(insert, 0, 5, Hello), (move, 5, 2, J), (delete, 2, 2, ll)]\n";
    assertEqual(historyOutput, expectedHistory, 6, "History output");
    evaluateTests(3);
}

void sample_07()
{
    // Test cursor error exception
    RopeTextBuffer tb;
    tb.insert("ABC");
    bool caught = false;
    try
    {
        tb.moveCursorTo(0);
        tb.moveCursorLeft();
    }
    catch (const cursor_error &e)
    {
        caught = true;
    }
    assertEqual(caught, true, 7, "Raise cursor_error");
    evaluateTests(1);
}

void sample_08()
{
    // Test HistoryManager through RopeTextBuffer
    RopeTextBuffer tb;
    tb.insert("Hi");
    tb.insert("!!");
    tb.moveCursorLeft();
    tb.deleteRange(1);
    assertEqual(tb.getContent(), "Hi!", 8, "Content");
    assertEqual(tb.getCursorPos(), 3, 8, "Cursor position");
    string historyOutput = captureOutput([&tb]()
                                         { tb.printHistory(); });

    string expectedHistory = "[(insert, 0, 2, Hi), (insert, 2, 4, !!), (move, 4, 3, L), (delete, 3, 3, !)]\n";
    assertEqual(historyOutput, expectedHistory, 8, "History output");
    evaluateTests(3);
}

void sample_09()
{

    Rope rope;
    rope.insert(0, "Hello_world");
    string result = rope.traversePreOrder();
    cout << "PreOrder traversal: " << result << endl;
}

void run_tests()
{
    cout << "=" << string(50, '=') << endl;

    sample_01();
    sample_02();
    sample_03();
    sample_04();
    sample_05();
    sample_06();
    sample_07();
    sample_08();
    sample_09();

    cout << "=" << string(50, '=') << endl;
    cout << COLOR_PURPLE << "All tests completed!" << COLOR_RESET << endl;
    cout << "You have passed " << COLOR_GREEN << countOfPassedTests << COLOR_RESET << "/" << 9 << " testcases!" << endl;
}

int main(int argc, char **argv)
{
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
#endif
    run_tests();
#ifdef _WIN32
    if (!_CrtDumpMemoryLeaks())
    {
        cout << COLOR_GREEN << "No memory leaks detected!" << COLOR_RESET << endl;
    }
    else
    {
        cout << COLOR_RED << "Memory leaks detected!" << COLOR_RESET << endl;
    }
#endif
}
