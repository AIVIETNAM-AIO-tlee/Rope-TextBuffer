#include "RopeTextBuffer.h"

#ifdef _WIN32
#include <crtdbg.h>
#endif

// Color codes
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_PURPLE "\033[35m"
#define COLOR_RESET "\033[0m"

static int countOfPassedTests = 0;

template <typename T>
void assertEqual(const T &actual, const T &expected, int testNum, const string &msg = "")
{
    if (actual == expected)
    {
        cout << "Test " << testNum << ": " << COLOR_GREEN << "Passed" << COLOR_RESET;
        if (!msg.empty())
            cout << " - " << msg;
        cout << endl;
        countOfPassedTests++;
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

/*---------------------------------------- Test cua thay ----------------------------------------
-----------------------------------------------------------------------------------------------*/
void sample_01()
{
    // Test basic Rope construction and empty state
    Rope rope;
    assertEqual(rope.empty(), true, 1, "Empty state");
    assertEqual(rope.length(), 0, 2, "Length");
}

void sample_02()
{
    // Test basic Rope insert and toString
    Rope rope;
    rope.insert(0, "Hello");
    assertEqual(rope.toString(), "Hello", 3, "Basic insert and toString");
    assertEqual(rope.length(), 5, 4, "Length");
}

void sample_03()
{
    // Test Rope charAt operation
    Rope rope;
    rope.insert(0, "ABC");
    assertEqual(rope.charAt(1), 'B', 5, "charAt(1) should return 'B'");
    assertEqual(rope.charAt(0), 'A', 6, "charAt(0) should return 'A'");
}

void sample_04()
{
    // Test RopeTextBuffer basic operations
    RopeTextBuffer tb;
    tb.insert("Hello");
    assertEqual(tb.getContent(), "Hello", 7, "TextBuffer insert");
    assertEqual(tb.getCursorPos(), 5, 8, "Cursor position");
}

void sample_05()
{
    // Test RopeTextBuffer cursor movement
    RopeTextBuffer tb;
    tb.insert("ABC");
    tb.moveCursorLeft();
    tb.insert("X");
    assertEqual(tb.getContent(), "ABXC", 9, "Content insert when moving cursor");
    assertEqual(tb.getCursorPos(), 3, 10, "Cursor position");
}

void sample_06()
{
    // Test RopeTextBuffer delete operation
    RopeTextBuffer tb;
    tb.insert("Hello");
    tb.moveCursorTo(2);
    tb.deleteRange(2);
    assertEqual(tb.getContent(), "heo", 11, "Content delete after moving cursor");
    assertEqual(tb.getCursorPos(), 2, 12, "Cursor position");
}

void sample_07()
{
    // Test cursor error exception
    cout << "Test 13: Testing cursor_error exception" << endl;
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
    assertEqual(caught, true, 13, "Raise cursor_error");
}

void sample_08()
{
    // Test HistoryManager through RopeTextBuffer
    RopeTextBuffer tb;
    tb.insert("Hi");
    tb.insert("!!");
    tb.moveCursorLeft();
    tb.deleteRange(1);
    assertEqual(tb.getContent(), "Hi!", 14, "Content");
    assertEqual(tb.getCursorPos(), 3, 15, "Cursor position");
    tb.printHistory();
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

    cout << "=" << string(50, '=') << endl;
    cout << COLOR_PURPLE << "All tests completed!" << COLOR_RESET << endl;
    cout << "You have passed " << COLOR_GREEN << countOfPassedTests << COLOR_RESET << "/" << 15 << " tests!" << endl;
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
