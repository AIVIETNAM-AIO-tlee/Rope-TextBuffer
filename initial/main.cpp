#include "RopeTextBuffer.h"
#include <functional>
#include <iomanip>
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
void assertEqual(const T &actual, const T &expected, float testNum, const string &msg = "")
{
    if (actual == expected)
    {
        cout << "Test " << fixed << setprecision(1) << testNum << ": " << COLOR_GREEN << "Passed" << COLOR_RESET;
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

void assertEqual(const string &actual, const char *expected, float testNum, const string &msg = "")
{
    assertEqual<string>(actual, string(expected), testNum, msg);
}
void assertEqual(const char *actual, const string &expected, float testNum, const string &msg = "")
{
    assertEqual<string>(string(actual), expected, testNum, msg);
}
void assertEqual(const char *actual, const char *expected, float testNum, const string &msg = "")
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
    assertEqual(rope.empty(), true, 1.1, "Empty state");
    assertEqual(rope.length(), 0, 1.2, "Length");
    evaluateTests(2);
}

void sample_02()
{
    // Test basic Rope insert and toString
    Rope rope;
    rope.insert(0, "Hello");
    assertEqual(rope.toString(), "Hello", 2.1, "Basic insert and toString");
    assertEqual(rope.length(), 5, 2.2, "Length");
    evaluateTests(2);
}

void sample_03()
{
    // Test Rope charAt operation
    Rope rope;
    rope.insert(0, "ABC");
    assertEqual(rope.charAt(1), 'B', 3.1, "charAt(1) should return 'B'");
    assertEqual(rope.charAt(0), 'A', 3.2, "charAt(0) should return 'A'");
    evaluateTests(2);
}

void sample_04()
{
    // Test RopeTextBuffer basic operations
    RopeTextBuffer tb;
    tb.insert("Hello");
    assertEqual(tb.getContent(), "Hello", 4.1, "TextBuffer insert");
    assertEqual(tb.getCursorPos(), 5, 4.2, "Cursor position");
    evaluateTests(2);
}

void sample_05()
{
    // Test RopeTextBuffer cursor movement
    RopeTextBuffer tb;
    tb.insert("ABC");
    tb.moveCursorLeft();
    tb.insert("X");
    assertEqual(tb.getContent(), "ABXC", 5.1, "Content insert when moving cursor");
    assertEqual(tb.getCursorPos(), 3, 5.2, "Cursor position");
    evaluateTests(2);
}

void sample_06()
{
    // Test RopeTextBuffer delete operation
    RopeTextBuffer tb;
    tb.insert("Hello");
    tb.moveCursorTo(2);
    tb.deleteRange(2);
    assertEqual(tb.getContent(), "Heo", 6.1, "Content delete after moving cursor");
    assertEqual(tb.getCursorPos(), 2, 6.2, "Cursor position");
    string historyOutput = captureOutput([&tb]()
                                         { tb.printHistory(); });

    string expectedHistory = "[(insert, 0, 5, Hello), (move, 5, 2, J), (delete, 2, 2, ll)]\n";
    assertEqual(historyOutput, expectedHistory, 6.3, "History output");
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
    assertEqual(caught, true, 7.1, "Raise cursor_error");
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
    assertEqual(tb.getContent(), "Hi!", 8.1, "Content");
    assertEqual(tb.getCursorPos(), 3, 8.2, "Cursor position");
    string historyOutput = captureOutput([&tb]()
                                         { tb.printHistory(); });
    string expectedHistory = "[(insert, 0, 2, Hi), (insert, 2, 4, !!), (move, 4, 3, L), (delete, 3, 3, !)]\n";
    assertEqual(historyOutput, expectedHistory, 8.3, "History output");
    evaluateTests(3);
}

void sample_09()
{

    Rope rope;
    rope.insert(0, "Hello_world");
    rope.deleteRange(5, 5);
    rope.insert(3, "_DataStructure");
    assertEqual(rope.toString(), "Hel_DataStructurelod", 9.1, "Content");
    string result = rope.traversePreOrder();
    assertEqual(result, "17 11 3 Hel _DataStr ucture 2 lo d", 9.2, "PreOrder traversal");
    evaluateTests(2);
}

/*---------------------------------------- Test Rope ----------------------------------------
---------------------------------------------------------------------------------------------*/

void sample_10()
{
    Rope rope;
    rope.insert(0, "Hello_World_This_is_course_DataStructure");
    assertEqual(rope.length(), 40, 10.1, "Length of Rope after insert");
    assertEqual(rope.traversePreOrder(), "16 8 Hello_Wo rld_This 16 8 _is_cour se_DataS tructure", 10.2, "PreOrder traversal");
    rope.insert(2, "Assignment_2");
    assertEqual(rope.length(), 52, 10.3, "Length of Rope after another insert");
    assertEqual(rope.traversePreOrder(), "14 10 2 He Assignme nt_2 14 6 llo_Wo rld_This 16 8 _is_cour se_DataS tructure", 10.4, "PreOrder traversal");
    assertEqual(rope.empty(), false, 10.5, "Check empty");
    assertEqual(rope.charAt(0), 'H', 10.6, "Get Char H");
    assertEqual(rope.charAt(7), 'n', 10.7, "Get Char n");
    assertEqual(rope.toString(), "HeAssignment_2llo_World_This_is_course_DataStructure", 10.8, "ToString");
    evaluateTests(8);
}

void sample_11()
{
    Rope rope;
    string caughtMessage = "";
    try
    {
        rope.insert(5, "DSA_is_foundation_for_almost_every_software");
    }
    catch (const out_of_range &e)
    {
        caughtMessage = e.what();
    }
    assertEqual(caughtMessage, "Index is invalid!", 11.1, "Exception message");
    assertEqual(rope.empty(), true, 11.2, "Check empty");
    evaluateTests(2);
}

void sample_12()
{
    Rope rope;
    rope.insert(0, "DSA_is_foundation_for_almost_every_software");
    assertEqual(rope.toString(), "DSA_is_foundation_for_almost_every_software", 12.1, "ToString");
    assertEqual(rope.length(), 43, 12.2, "Length");
    assertEqual(rope.traversePreOrder(), "32 16 8 DSA_is_f oundatio 8 n_for_al most_eve 8 ry_softw are", 12.3, "PreOrder traversal");
    rope.deleteRange(2, 5);
    assertEqual(rope.toString(), "DSfoundation_for_almost_every_software", 12.4, "ToString after delete");
    assertEqual(rope.length(), 38, 12.5, "Length after delete");
    assertEqual(rope.traversePreOrder(), "11 2 DS 1 f oundatio 16 8 n_for_al most_eve 8 ry_softw are", 12.6, "PreOrder traversal after delete");
    assertEqual(rope.substring(2, 1), "f", 12.7, "Substring");
    evaluateTests(7);
}

/*---------------------------------------- Test RopeTextBuffer ------------------------------
---------------------------------------------------------------------------------------------*/

void sample_13()
{
    RopeTextBuffer tb;
    tb.insert("Hello");
    tb.insert("World");
    assertEqual(tb.getContent(), "HelloWorld", 13.1, "GetContent");
    assertEqual(tb.getCursorPos(), 10, 13.2, "Cursor position");
    tb.moveCursorTo(5);
    tb.deleteRange(5);
    assertEqual(tb.getContent(), "Hello", 13.3, "GetContent after delete");
    assertEqual(tb.getCursorPos(), 5, 13.4, "Cursor position after delete");
    evaluateTests(4);
}

void sample_14()
{
    RopeTextBuffer tb;
    tb.insert("Hello");
    tb.insert("World");
    tb.moveCursorTo(5);
    tb.replace(0, "Everyone");
    assertEqual(tb.getContent(), "HelloEveryoneWorld", 14.1, "GetContent after replace");
    assertEqual(tb.getCursorPos(), 13, 14.2, "Cursor position after replace");
    tb.moveCursorLeft();
    tb.replace(6, "DataStructure");
    assertEqual(tb.getContent(), "HelloEveryonDataStructure", 14.3, "GetContent after replace");
    assertEqual(tb.getCursorPos(), 25, 14.4, "Cursor position after replace");
    evaluateTests(4);
}

void sample_15()
{
    RopeTextBuffer tb;
    tb.insert("Hello");
    tb.insert("Worldoooooo");

    string content = tb.getContent();
    int count = 0;
    for (char c : content)
        if (c == 'o')
            count++;

    int *indexes = tb.findAll('o');
    string actualPositions = "[";
    for (int i = 0; i < count; i++)
    {
        if (i > 0)
            actualPositions += ", ";
        actualPositions += to_string(indexes[i]);
    }
    actualPositions += "]";
    string expectedPositions = "[4, 6, 10, 11, 12, 13, 14, 15]";
    assertEqual(actualPositions, expectedPositions, 15.1, "findAll positions");
    delete[] indexes;
    evaluateTests(1);
}

void sample_16()
{
    RopeTextBuffer tb;
    tb.insert("Hello");
    tb.insert("Worldoooooo");
    int index = tb.findFirst('o');
    assertEqual(index, 4, 16.1, "findFirst position");
    tb.moveCursorTo(3);
    assertEqual(tb.getCursorPos(), 3, 16.2, "Cursor position");
    tb.deleteRange(5);
    assertEqual(tb.getContent(), "Helldoooooo", 16.3, "Content after delete");
    index = tb.findFirst('o');
    assertEqual(index, 5, 16.4, "findFirst position after delete");
    evaluateTests(4);
}

// PrintHistory and clear tests
void sample_17()
{
    RopeTextBuffer tb;
    tb.insert("Hello");
    tb.insert("World");
    tb.moveCursorTo(5);
    tb.deleteRange(5);
    string output = tb.getContent();
    assertEqual(output, "Hello", 17.1, "Content after delete");
    string historyOutput = captureOutput([&tb]()
                                         { tb.printHistory(); });
    string expectedHistory = "[(insert, 0, 5, Hello), (insert, 5, 10, World), (move, 10, 5, J), (delete, 5, 5, World)]\n";
    assertEqual(historyOutput, expectedHistory, 17.1, "History output after operations");

    tb.clear();
    output = tb.getContent();
    assertEqual(output, "", 17.2, "Content after clear");
    evaluateTests(3);
}

// undo/redo tests

void sample_18()
{
    RopeTextBuffer tb;
    tb.insert("Hello");
    tb.insert("World");
    tb.undo();
    string output = tb.getContent(); // should be Hello
    assertEqual(output, "Hello", 18.1, "Content after undo");
    assertEqual(tb.getCursorPos(), 5, 18.2, "Cursor position after undo");
    tb.redo();
    output = tb.getContent(); // should be HelloWorld
    assertEqual(output, "HelloWorld", 18.3, "Content after redo");
    assertEqual(tb.getCursorPos(), 10, 18.4, "Cursor position after redo");
    evaluateTests(4);
}

void sample_19()
{
    RopeTextBuffer tb;
    tb.insert("Hello");
    tb.insert("World");
    tb.moveCursorTo(5);
    tb.replace(0, "Everyone");
    string output = tb.getContent(); // should be HelloEveryoneWorld
    assertEqual(output, "HelloEveryoneWorld", 19.1, "Content after replace");
    tb.undo();
    output = tb.getContent();
    assertEqual(output, "HelloWorld", 19.2, "Content after undo");
    assertEqual(tb.getCursorPos(), 5, 19.3, "Cursor position after undo");
    tb.redo();
    output = tb.getContent();
    assertEqual(output, "HelloEveryoneWorld", 19.4, "Content after redo");
    assertEqual(tb.getCursorPos(), 13, 19.5, "Cursor position after redo");

    string historyOutput = captureOutput([&tb]()
                                         { tb.printHistory(); });
    string expectedHistory = "[(insert, 0, 5, Hello), (insert, 5, 10, World), (move, 10, 5, J), (replace, 5, 13, Everyone)]\n";
    assertEqual(historyOutput, expectedHistory, 19.6, "History output after undo/redo operations");

    tb.clear();
    output = tb.getContent();
    assertEqual(output, "", 19.7, "Content after clear");
    evaluateTests(7);
}

void sample_20()
{
    RopeTextBuffer tb;
    tb.insert("Hello");
    tb.insert("World");
    tb.undo();
    tb.undo();
    string output = tb.getContent();
    assertEqual(output, "", 20.1, "Content after two undos");
    assertEqual(tb.getCursorPos(), 0, 20.2, "Cursor position after two undos");
    evaluateTests(2);
}

void sample_21()
{
    RopeTextBuffer tb;
    tb.insert("Hello");
    tb.insert("World");
    tb.undo();
    tb.undo();
    tb.redo();
    string output = tb.getContent();
    assertEqual(output, "Hello", 21.1, "Content after two undos");
    assertEqual(tb.getCursorPos(), 5, 21.2, "Cursor position after two undos");
    evaluateTests(2);
}

void sample_22()
{
    RopeTextBuffer tb;
    tb.insert("Hello");
    tb.insert("World");
    tb.moveCursorTo(5);
    tb.replace(0, "Everyone");
    tb.redo(); // k thay doi
    assertEqual(tb.getContent(), "HelloEveryoneWorld", 22.1, "Content after redo");
    assertEqual(tb.getCursorPos(), 13, 22.2, "Cursor position after redo");
    evaluateTests(2);
}

void sample_23()
{
    RopeTextBuffer tb;
    tb.insert("Hello");
    tb.insert("World");
    tb.moveCursorTo(5);
    tb.undo();
    assertEqual(tb.getCursorPos(), 10, 23.1, "Cursor position after undo");
    tb.redo();
    assertEqual(tb.getCursorPos(), 5, 23.2, "Cursor position after redo");
    tb.moveCursorLeft();
    assertEqual(tb.getCursorPos(), 4, 23.3, "Cursor position after move left");
    tb.moveCursorRight();
    assertEqual(tb.getCursorPos(), 5, 23.4, "Cursor position after move right");
    tb.undo();
    tb.deleteRange(3);
    assertEqual(tb.getContent(), "Hellrld", 23.5, "Content after delete");
    assertEqual(tb.getCursorPos(), 4, 23.6, "Cursor position after delete");
    tb.undo();
    assertEqual(tb.getContent(), "HelloWorld", 23.7, "Content after undo delete");
    assertEqual(tb.getCursorPos(), 4, 23.8, "Cursor position after undo delete");
    string historyOutput = captureOutput([&tb]()
                                         { tb.printHistory(); });
    string expectedHistory = "[(insert, 0, 5, Hello), (insert, 5, 10, World), (move, 10, 5, J), (move, 5, 4, L), (move, 4, 5, R), (delete, 4, 4, oWo)]\n";
    assertEqual(historyOutput, expectedHistory, 23.9, "History output after operations");
    tb.clear();
    string output = tb.getContent();
    assertEqual(output, "", 23.10, "Content after clear");
    evaluateTests(10);
}

// 1 test for HistoryManager

void sample_24()
{
    RopeTextBuffer::HistoryManager hm;
    RopeTextBuffer::HistoryManager::Action action1 = {"insert", 0, 5, "Hello"};
    RopeTextBuffer::HistoryManager::Action action2 = {"insert", 5, 10, "World"};
    hm.addAction(action1);
    hm.addAction(action2);
    string historyOutput = captureOutput([&hm]()
                                         { hm.printHistory(); });
    string expectedHistory = "[(insert, 0, 5, Hello), (insert, 5, 10, World)]\n";
    assertEqual(historyOutput, expectedHistory, 24.1, "History output after two actions");
    evaluateTests(1);
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
    sample_10();
    sample_11();
    sample_12();
    sample_13();
    sample_14();
    sample_15();
    sample_16();
    sample_17();
    sample_18();
    sample_19();
    sample_20();
    sample_21();
    sample_22();
    sample_23();
    sample_24();

    cout << "=" << string(50, '=') << endl;
    cout << COLOR_PURPLE << "All tests completed!" << COLOR_RESET << endl;
    cout << "You have passed " << COLOR_GREEN << countOfPassedTests << COLOR_RESET << "/" << 24 << " testcases!" << endl;
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
