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
    // string result = rope.traversePreOrder();
    // assertEqual(result, "17 11 3 Hel _DataStr ucture 2 lo d", 9.2, "PreOrder traversal");
    evaluateTests(1);
}

/*---------------------------------------- Test Rope ----------------------------------------
---------------------------------------------------------------------------------------------*/

void sample_10()
{
    Rope rope;
    rope.insert(0, "Hello_World_This_is_course_DataStructure");
    assertEqual(rope.length(), 40, 10.1, "Length of Rope after insert");
    // assertEqual(rope.traversePreOrder(), "16 8 Hello_Wo rld_This 16 8 _is_cour se_DataS tructure", 10.2, "PreOrder traversal");
    rope.insert(2, "Assignment_2");
    assertEqual(rope.length(), 52, 10.3, "Length of Rope after another insert");
    // assertEqual(rope.traversePreOrder(), "14 10 2 He Assignme nt_2 14 6 llo_Wo rld_This 16 8 _is_cour se_DataS tructure", 10.4, "PreOrder traversal");
    assertEqual(rope.empty(), false, 10.5, "Check empty");
    assertEqual(rope.charAt(0), 'H', 10.6, "Get Char H");
    assertEqual(rope.charAt(7), 'n', 10.7, "Get Char n");
    assertEqual(rope.toString(), "HeAssignment_2llo_World_This_is_course_DataStructure", 10.8, "ToString");
    evaluateTests(6);
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
    // assertEqual(rope.traversePreOrder(), "32 16 8 DSA_is_f oundatio 8 n_for_al most_eve 8 ry_softw are", 12.3, "PreOrder traversal");
    rope.deleteRange(2, 5);
    assertEqual(rope.toString(), "DSfoundation_for_almost_every_software", 12.4, "ToString after delete");
    assertEqual(rope.length(), 38, 12.5, "Length after delete");
    // assertEqual(rope.traversePreOrder(), "11 2 DS 1 f oundatio 16 8 n_for_al most_eve 8 ry_softw are", 12.6, "PreOrder traversal after delete");
    assertEqual(rope.substring(2, 1), "f", 12.7, "Substring");
    evaluateTests(5);
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
    string expectedHistory = "[(insert, 0, 5, Hello), (insert, 5, 10, World), (move, 10, 5, J), (replace, 5, 13, )]\n";
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

void sample_24()
{
    RopeTextBuffer tb;
    tb.insert("A");
    tb.insert("CSE");
    tb.insert("HCMUT");
    tb.moveCursorLeft();
    tb.insert("123");
    tb.moveCursorTo(4);
    tb.deleteRange(3);
    string output = tb.getContent();
    assertEqual(output, "ACSEU123T", 24.1, "Content after operations");
    assertEqual(tb.getCursorPos(), 4, 24.2, "Cursor position after operations");
    tb.undo();
    assertEqual(tb.getContent(), "ACSEHCMU123T", 24.3, "Content after undo");
    assertEqual(tb.getCursorPos(), 4, 24.4, "Cursor position after undo");
    tb.undo();
    assertEqual(tb.getCursorPos(), 11, 24.5, "Cursor position after second undo");
    tb.undo();
    assertEqual(tb.getContent(), "ACSEHCMUT", 24.6, "Content after second undo");
    assertEqual(tb.getCursorPos(), 8, 24.7, "Cursor position after second undo");
    tb.redo();
    assertEqual(tb.getContent(), "ACSEHCMU123T", 24.8, "Content after redo");
    assertEqual(tb.getCursorPos(), 11, 24.9, "Cursor position after redo");
    tb.redo();
    assertEqual(tb.getContent(), "ACSEHCMU123T", 24.10, "Content after second redo");
    assertEqual(tb.getCursorPos(), 4, 24.11, "Cursor position after second redo");
    tb.redo();
    assertEqual(tb.getContent(), "ACSEU123T", 24.12, "Content after third redo");
    assertEqual(tb.getCursorPos(), 4, 24.13, "Cursor position after third redo");
    string historyOutput = captureOutput([&tb]()
                                         { tb.printHistory(); });
    string expectedHistory = "[(insert, 0, 1, A), (insert, 1, 4, CSE), (insert, 4, 9, HCMUT), (move, 9, 8, L), (insert, 8, 11, 123), (move, 11, 4, J), (delete, 4, 4, HCM)]\n";
    assertEqual(historyOutput, expectedHistory, 24.14, "History output after operations");
    evaluateTests(14);
}

// 1 test for HistoryManager

void sample_25()
{
    RopeTextBuffer::HistoryManager hm;
    RopeTextBuffer::HistoryManager::Action action1 = {"insert", 0, 5, "Hello"};
    RopeTextBuffer::HistoryManager::Action action2 = {"insert", 5, 10, "World"};
    hm.addAction(action1);
    hm.addAction(action2);
    string historyOutput = captureOutput([&hm]()
                                         { hm.printHistory(); });
    string expectedHistory = "[(insert, 0, 5, Hello), (insert, 5, 10, World)]\n";
    assertEqual(historyOutput, expectedHistory, 25.1, "History output after two actions");
    evaluateTests(1);
}

void sample_26()
{
    RopeTextBuffer tb;

    // Create very long strings for testing (each over 500 characters)
    string longText1 = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque.";

    string longText2 = "At vero eos et accusamus et iusto odio dignissimos ducimus qui blanditiis praesentium voluptatum deleniti atque corrupti quos dolores et quas molestias excepturi sint occaecati cupiditate non provident, similique sunt in culpa qui officia deserunt mollitia animi, id est laborum et dolorum fuga. Et harum quidem rerum facilis est et expedita distinctio nam libero tempore cum soluta nobis est eligendi optio cumque nihil impedit quo minus id quod maxime placeat facere possimus.";

    string longText3 = "Temporibus autem quibusdam et aut officiis debitis aut rerum necessitatibus saepe eveniet ut et voluptates repudiandae sint et molestiae non recusandae. Itaque earum rerum hic tenetur a sapiente delectus, ut aut reiciendis voluptatibus maiores alias consequatur aut perferendis doloribus asperiores repellat consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt neque porro quisquam est qui dolorem ipsum quia dolor sit amet consectetur.";

    tb.insert(longText1);
    int len1 = (int)longText1.length();
    assertEqual((int)tb.getContent().length(), len1, 26.1, "First long text insertion");
    assertEqual(tb.getCursorPos(), len1, 26.2, "Cursor after first insert");

    tb.moveCursorTo(0);
    tb.insert(longText2);
    int len2 = (int)longText2.length();
    int totalLen = len1 + len2;
    assertEqual((int)tb.getContent().length(), totalLen, 26.3, "Length after insert at beginning");
    assertEqual(tb.getCursorPos(), len2, 26.4, "Cursor after insert at beginning");

    int middlePos = totalLen / 2;
    tb.moveCursorTo(middlePos);
    assertEqual(tb.getCursorPos(), middlePos, 26.5, "Cursor moved to middle");

    int replaceLen = 100;
    int len3 = (int)longText3.length();
    tb.replace(replaceLen, longText3);
    int newTotalLen = totalLen - replaceLen + len3;
    assertEqual((int)tb.getContent().length(), newTotalLen, 26.6, "Length after replace");
    assertEqual(tb.getCursorPos(), middlePos + len3, 26.7, "Cursor after replace");

    tb.moveCursorTo(200);
    int deleteLen = 150;
    tb.deleteRange(deleteLen);
    int finalLen = newTotalLen - deleteLen;
    assertEqual((int)tb.getContent().length(), finalLen, 26.8, "Length after delete");
    assertEqual(tb.getCursorPos(), 200, 26.9, "Cursor after delete");

    tb.undo();
    assertEqual((int)tb.getContent().length(), newTotalLen, 26.10, "Length after undo delete");

    tb.undo();
    assertEqual(tb.getCursorPos(), middlePos + len3, 26.11, "Cursor after undo move");

    tb.undo();
    assertEqual((int)tb.getContent().length(), totalLen, 26.12, "Length after undo replace");
    assertEqual(tb.getCursorPos(), middlePos, 26.13, "Cursor after undo replace");

    tb.redo();
    assertEqual((int)tb.getContent().length(), newTotalLen, 26.14, "Length after redo replace");

    tb.redo();
    assertEqual(tb.getCursorPos(), 200, 26.15, "Cursor after redo move");

    tb.redo();
    assertEqual((int)tb.getContent().length(), finalLen, 26.16, "Length after redo delete");

    int firstL = tb.findFirst('L');
    assertEqual(firstL >= 0, true, 26.17, "Found 'L' in long text");

    if ((int)tb.getContent().length() >= 100)
    {
        string sub = tb.getContent().substr(10, 50);
        assertEqual((int)sub.length(), 50, 26.18, "Substring length");
    }
    else
    {
        assertEqual(true, true, 26.18, "Substring test skipped - content too short");
    }

    tb.moveCursorTo((int)tb.getContent().length());
    tb.insert("_FINAL_TEST_MARKER_");
    string endCheck = tb.getContent().substr(tb.getContent().length() - 19);
    assertEqual(endCheck, "_FINAL_TEST_MARKER_", 26.19, "Final marker check");

    tb.clear();
    assertEqual(tb.getContent(), "", 26.20, "Content cleared");
    assertEqual(tb.getCursorPos(), 0, 26.21, "Cursor reset");

    evaluateTests(21);
}

void sample_27()
{
    Rope rope;

    string megaText1 = "The quick brown fox jumps over the lazy dog. This pangram contains every letter of the English alphabet at least once, making it a perfect test string for comprehensive character coverage and rope data structure validation. We need to ensure that our rope implementation can handle large text insertions, deletions, and modifications efficiently while maintaining the correct tree structure and balance factors throughout all operations performed on this extensive textual content that will challenge every aspect of our rope implementation including split operations, concatenation procedures, and tree rebalancing algorithms that must work correctly under all circumstances and edge cases that might arise during complex text manipulation scenarios in real-world applications where performance and correctness are absolutely critical for success.";

    string megaText2 = "Lorem ipsum dolor sit amet consectetur adipiscing elit sed do eiusmod tempor incididunt ut labore et dolore magna aliqua enim ad minim veniam quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur excepteur sint occaecat cupidatat non proident sunt in culpa qui officia deserunt mollit anim id est laborum sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium totam rem aperiam eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt neque porro quisquam est qui dolorem ipsum quia dolor sit amet consectetur adipisci velit.";

    string megaText3 = "But I must explain to you how all this mistaken idea of denouncing pleasure and praising pain was born and I will give you a complete account of the system and expound the actual teachings of the great explorer of the truth the master builder of human happiness no one rejects dislikes or avoids pleasure itself because it is pleasure but because those who do not know how to pursue pleasure rationally encounter consequences that are extremely painful nor again is there anyone who loves or pursues or desires to obtain pain of itself because it is pain but because occasionally circumstances occur in which toil and pain can procure him some great pleasure to take a trivial example which of us ever undertakes laborious physical exercise except to obtain some advantage from it but who has any right to find fault with a man who chooses to enjoy a pleasure that has no annoying consequences or one who avoids a pain that produces no resultant pleasure.";

    rope.insert(0, megaText1);
    int len1 = (int)megaText1.length();
    assertEqual(rope.length(), len1, 27.1, "Mega text 1 insertion");
    assertEqual(rope.empty(), false, 27.2, "Rope not empty after insertion");

    rope.insert(len1 / 4, megaText2);
    int len2 = (int)megaText2.length();
    int totalAfterSecond = len1 + len2;
    assertEqual(rope.length(), totalAfterSecond, 27.3, "Length after second mega insertion");

    rope.insert(totalAfterSecond, megaText3);
    int len3 = (int)megaText3.length();
    int totalAfterThird = totalAfterSecond + len3;
    assertEqual(rope.length(), totalAfterThird, 27.4, "Length after third mega insertion");

    char firstChar = rope.charAt(0);
    char quarterChar = rope.charAt(totalAfterThird / 4);
    char halfChar = rope.charAt(totalAfterThird / 2);
    char threeQuarterChar = rope.charAt(3 * totalAfterThird / 4);
    char lastChar = rope.charAt(totalAfterThird - 1);

    assertEqual(firstChar == 'T', true, 27.5, "First character correct");
    assertEqual(quarterChar != '\0', true, 27.6, "Quarter position character exists");
    assertEqual(halfChar != '\0', true, 27.7, "Half position character exists");
    assertEqual(threeQuarterChar != '\0', true, 27.8, "Three-quarter position character exists");
    assertEqual(lastChar == '.', true, 27.9, "Last character correct");
    // string ressult = rope.traversePreOrder();
    // string expected = "2555 2379 2211 2051 1899 1755 1100 972 852 740 636 540 452 372 300 236 176 128 88 56 32 16 8 The quic k brown  8 fox jump s over t 16 8 he lazy  dog. Thi s pangra 24 16 8 m contai ns every  letter  of the E 32 24 16 8 nglish a lphabet  at least  once, m aking it 40 32 24 16 8  a perfe ct test  string f or compr ehensive  charact 52 44 36 24 16 8 er cover age and  rope dat 8 a struct ure  Lorem ip sum dolo r sit am 56 48 40 32 24 16 8 et conse ctetur a dipiscin g elit s ed do ei usmod te mpor inc ididunt  64 56 48 40 32 24 16 8 ut labor e et dol ore magn a aliqua  enim ad  minim v eniam qu is nostr ud exerc 72 64 56 48 40 32 24 16 8 itation  ullamco  laboris  nisi ut  aliquip  ex ea co mmodo co nsequat  duis aut e irure  80 72 64 56 48 40 32 24 16 8 dolor in  reprehe nderit i n volupt ate veli t esse c illum do lore eu  fugiat n ulla par iatur ex 88 80 72 64 56 48 40 32 24 16 8 cepteur  sint occ aecat cu pidatat  non proi dent sun t in cul pa qui o fficia d eserunt  mollit a nim id e 96 88 80 72 64 56 48 40 32 24 16 8 st labor um sed u t perspi ciatis u nde omni s iste n atus err or sit v oluptate m accusa ntium do loremque  laudant 104 96 88 80 72 64 56 48 40 32 24 16 8 ium tota m rem ap eriam ea que ipsa  quae ab  illo in ventore  veritati s et qua si archi tecto be atae vit ae dicta  sunt ex 112 104 96 88 80 72 64 56 48 40 32 24 16 8 plicabo  nemo eni m ipsam  voluptat em quia  voluptas  sit asp ernatur  aut odit  aut fug it sed q uia cons equuntur  magni d olores e 120 112 104 96 88 80 72 64 56 48 40 32 24 16 8 os qui r atione v oluptate m sequi  nesciunt  neque p orro qui squam es t qui do lorem ip sum quia  dolor s it amet  consecte tur adip isci vel 647 639 423 3 it. 324 236 156 84 68 20 12 4 vali dation.  We need  40 32 24 16 8 to ensur e that o ur rope  implemen tation c an handl 8 e large  text ins 64 56 48 40 32 24 16 8 ertions,  deletio ns, and  modifica tions ef ficientl y while  maintain ing the  72 64 56 48 40 32 24 16 8 correct  tree str ucture a nd balan ce facto rs throu ghout al l operat ions per formed o 80 72 64 56 48 40 32 24 16 8 n this e xtensive  textual  content  that wi ll chall enge eve ry aspec t of our  rope im plementa 88 80 72 64 56 48 40 32 24 16 8 tion inc luding s plit ope rations,  concate nation p rocedure s, and t ree reba lancing  algorith ms that  104 96 88 80 72 64 56 48 40 32 24 16 8 must wor k correc tly unde r all ci rcumstan ces and  edge cas es that  might ar ise duri ng compl ex text  manipula 96 88 80 72 64 56 48 40 32 24 16 8 tion sce narios i n real-w orld app lication s where  performa nce and  correctn ess are  absolute ly criti 8 cal for  success. But I mu st expla 136 128 120 112 104 96 88 80 72 64 56 48 40 32 24 16 8 in to yo u how al l this m istaken  idea of  denounci ng pleas ure and  praising  pain wa s born a nd I wil l give y ou a com plete ac count of  the sys tem and  144 136 128 120 112 104 96 88 80 72 64 56 48 40 32 24 16 8 expound  the actu al teach ings of  the grea t explor er of th e truth  the mast er build er of hu man happ iness no  one rej ects dis likes or  avoids  pleasure  itself  152 144 136 128 120 112 104 96 88 80 72 64 56 48 40 32 24 16 8 because  it is pl easure b ut becau se those  who do  not know  how to  pursue p leasure  rational ly encou nter con sequence s that a re extre mely pai nful nor  again i s there  160 152 144 136 128 120 112 104 96 88 80 72 64 56 48 40 32 24 16 8 anyone w ho loves  or purs ues or d esires t o obtain  pain of  itself  because  it is pa in but b ecause o ccasiona lly circ umstance s occur  in which  toil an d pain c an procu re him s 168 160 152 144 136 128 120 112 104 96 88 80 72 64 56 48 40 32 24 16 8 ome grea t pleasu re to ta ke a tri vial exa mple whi ch of us  ever un dertakes  laborio us physi cal exer cise exc ept to o btain so me advan tage fro m it but  who has  any rig ht to fi nd fault 136 128 120 112 104 96 88 80 72 64 56 48 40 32 24 16 8  with a  man who  chooses  to enjoy  a pleas ure that  has no  annoying  consequ ences or  one who  avoids  a pain t hat prod uces no  resultan t pleasu re.";
    // assertEqual(ressult, expected, 27.10, "Pre-order traversal result");
    int deleteStart1 = totalAfterThird / 6;
    int deleteLen1 = 200;
    rope.deleteRange(deleteStart1, deleteLen1);
    int afterFirstDelete = totalAfterThird - deleteLen1;
    assertEqual(rope.length(), afterFirstDelete, 27.11, "Length after first large delete");

    int deleteStart2 = afterFirstDelete * 2 / 3;
    int deleteLen2 = 300;
    rope.deleteRange(deleteStart2, deleteLen2);
    int afterSecondDelete = afterFirstDelete - deleteLen2;
    assertEqual(rope.length(), afterSecondDelete, 27.12, "Length after second large delete");

    int middlePos = afterSecondDelete / 2;
    string insertText = "***MIDDLE_INSERTION_MARKER_WITH_VERY_LONG_CONTENT_FOR_TESTING***";
    rope.insert(middlePos, insertText);
    int afterMiddleInsert = afterSecondDelete + (int)insertText.length();
    assertEqual(rope.length(), afterMiddleInsert, 27.13, "Length after middle insertion");

    if (afterMiddleInsert >= 500)
    {
        string sub1 = rope.substring(10, 100);
        assertEqual((int)sub1.length(), 100, 27.14, "First substring length");

        string sub2 = rope.substring(afterMiddleInsert - 200, 150);
        assertEqual((int)sub2.length(), 150, 27.15, "Second substring length");

        string sub3 = rope.substring(middlePos, 50);
        assertEqual(sub3.substr(0, 3), "***", 27.16, "Middle insertion marker found");
    }
    else
    {
        assertEqual(true, true, 27.17, "Substring test 1 skipped - content too short");
        assertEqual(true, true, 27.18, "Substring test 2 skipped - content too short");
        assertEqual(true, true, 27.19, "Substring test 3 skipped - content too short");
    }

    for (int i = 0; i < 5; i++)
    {
        int pos = (afterMiddleInsert / 6) * (i + 1);
        rope.insert(pos, "X");
        afterMiddleInsert++;
    }
    assertEqual(rope.length(), afterMiddleInsert, 27.20, "Length after multiple small insertions");

    int largeDel = 400;
    rope.deleteRange(0, largeDel);
    afterMiddleInsert -= largeDel;
    assertEqual(rope.length(), afterMiddleInsert, 27.21, "Length after large delete from beginning");

    // string traversal = rope.traversePreOrder();
    // assertEqual(traversal.length() > 0, true, 27.22, "Traversal produces output");
    // assertEqual(traversal.find(" ") != string::npos, true, 27.23, "Traversal contains spaces (proper format)");

    if (afterMiddleInsert > 0)
    {
        char newFirst = rope.charAt(0);
        char newLast = rope.charAt(afterMiddleInsert - 1);
        assertEqual(newFirst != '\0', true, 27.24, "New first character exists");
        assertEqual(newLast != '\0', true, 27.25, "New last character exists");
    }
    else
    {
        assertEqual(true, true, 27.26, "First char test skipped - empty rope");
        assertEqual(true, true, 27.27, "Last char test skipped - empty rope");
    }

    string finalContent = rope.toString();
    assertEqual((int)finalContent.length(), afterMiddleInsert, 27.28, "Final toString length matches");
    assertEqual(finalContent.find("***") != string::npos, true, 27.29, "Marker still present in final content");

    evaluateTests(21);
}

void sample_28()
{
    Rope rope;
    rope.insert(0, "HelloWorld");
    string caught;
    try
    {
        rope.insert(99, "");
    }
    catch (const out_of_range &e)
    {
        caught = e.what();
    }
    assertEqual(caught, "Index is invalid!", 28.1, "Insert at invalid index throws exception");
    evaluateTests(1);
}

void sample_29()
{
    Rope rope;
    rope.insert(0, "HelloWorld");
    string caught;
    try
    {
        rope.substring(99, 5);
    }
    catch (const out_of_range &e)
    {
        caught = e.what();
    }
    assertEqual(caught, "Index is invalid!", 29.1, "Insert at invalid index throws exception");
    caught.clear();
    try
    {
        rope.substring(0, 11);
    }
    catch (const out_of_range &e)
    {
        caught = e.what();
    }
    assertEqual(caught, "Length is invalid!", 29.2, "Substring at invalid index throws exception");
    evaluateTests(2);
}

// Test cua VO TIEN

void sample_30()
{
    RopeTextBuffer::HistoryManager hm;

    string historyOutput = captureOutput([&hm]()
                                         { hm.printHistory(); });
    string expectedHistory = "[]\n";
    assertEqual(historyOutput, expectedHistory, 30.1, "Empty history output");

    hm.addAction({"insert", 0, 1, "C"});
    hm.addAction({"delete", 1, 1, "b"});
    hm.addAction({"move", 1, 0, "L"});
    hm.addAction({"move", 0, 1, "R"});
    hm.addAction({"insert", 0, 1, "a"});
    hm.addAction({"move", 1, 5, "J"});
    hm.addAction({"replace", 5, 10, "New_DSA"});
    hm.addAction({"insert", 0, 1, "X"});

    historyOutput = captureOutput([&hm]()
                                  { hm.printHistory(); });
    expectedHistory = "[(insert, 0, 1, C), (delete, 1, 1, b), (move, 1, 0, L), (move, 0, 1, R), (insert, 0, 1, a), (move, 1, 5, J), (replace, 5, 10, New_DSA), (insert, 0, 1, X)]\n";
    assertEqual(historyOutput, expectedHistory, 30.2, "History output after adding actions");
    evaluateTests(2);
}

void sample_31()
{
    RopeTextBuffer ropeTextBuffer;

    ropeTextBuffer.insert("BTL2");
    ropeTextBuffer.insert("CSE");
    ropeTextBuffer.insert("HCMUT");
    ropeTextBuffer.moveCursorLeft();
    ropeTextBuffer.insert("DSA");
    ropeTextBuffer.insert("2025");
    ropeTextBuffer.moveCursorTo(4);
    ropeTextBuffer.deleteRange(3);
    assertEqual(ropeTextBuffer.getContent(), "BTL2HCMUDSA2025T", 31.1, "Content after operations");
    assertEqual(ropeTextBuffer.getCursorPos(), 4, 31.2, "Cursor position after operations");

    string historyOutput = captureOutput([&ropeTextBuffer]()
                                         { ropeTextBuffer.printHistory(); });
    string expectedHistory = "[(insert, 0, 4, BTL2), (insert, 4, 7, CSE), (insert, 7, 12, HCMUT), (move, 12, 11, L), (insert, 11, 14, DSA), (insert, 14, 18, 2025), (move, 18, 4, J), (delete, 4, 4, CSE)]\n";
    assertEqual(historyOutput, expectedHistory, 31.3, "History output after operations");

    ropeTextBuffer.undo();

    ropeTextBuffer.undo();

    ropeTextBuffer.undo();

    ropeTextBuffer.redo();

    ropeTextBuffer.redo();

    ropeTextBuffer.redo();

    assertEqual(ropeTextBuffer.getContent(), "BTL2HCMUDSA2025T", 31.4, "Content after redos");
    assertEqual(ropeTextBuffer.getCursorPos(), 4, 31.5, "Cursor position after redos");
    evaluateTests(5);
}

void sample_32()
{
    RopeTextBuffer buffer;

    assertEqual(buffer.getContent(), "", 32.1, "Initial content should be empty");
    assertEqual(buffer.getCursorPos(), 0, 32.2, "Initial cursor position should be 0");

    buffer.insert("Hello");
    assertEqual(buffer.getContent(), "Hello", 32.3, "Content after insert");
    assertEqual(buffer.getCursorPos(), 5, 32.4, "Cursor position after insert");

    buffer.moveCursorTo(2);
    buffer.insert("_XyZWorddd___");
    assertEqual(buffer.getContent(), "He_XyZWorddd___llo", 32.5, "Content after insert at cursor");
    assertEqual(buffer.getCursorPos(), 15, 32.6, "Cursor position after insert at cursor");

    buffer.moveCursorTo(0);
    buffer.insert("DsA_");
    assertEqual(buffer.getContent(), "DsA_He_XyZWorddd___llo", 32.7, "Content after insert at start");
    assertEqual(buffer.getCursorPos(), 4, 32.8, "Cursor position after insert at start");

    buffer.moveCursorTo(buffer.getContent().size());
    buffer.insert("_END");
    assertEqual(buffer.getContent(), "DsA_He_XyZWorddd___llo_END", 32.9, "Content after insert at end");
    evaluateTests(9);
}

void sample_33()
{
    RopeTextBuffer buffer;
    buffer.insert("Nothing_beats_A_Jet2_holiday!");
    assertEqual(buffer.getContent(), "Nothing_beats_A_Jet2_holiday!", 33.1, "Content after insert");
    assertEqual(buffer.getCursorPos(), 29, 33.2, "Cursor position after insert");
    string caught;
    try
    {
        buffer.moveCursorRight();
    }
    catch (const cursor_error &e)
    {
        caught = e.what();
    }
    assertEqual(caught, "Cursor error!", 33.3, "Moving cursor right at end throws exception");
    buffer.moveCursorTo(0);
    assertEqual(buffer.getCursorPos(), 0, 33.4, "Cursor position after moving to start");
    caught.clear();
    try
    {
        buffer.moveCursorLeft();
    }
    catch (const cursor_error &e)
    {
        caught = e.what();
    }
    assertEqual(caught, "Cursor error!", 33.5, "Moving cursor left at start throws exception");

    buffer.moveCursorRight();
    buffer.moveCursorRight();
    buffer.moveCursorRight();
    assertEqual(buffer.getCursorPos(), 3, 33.6, "Cursor position after moving right");
    buffer.moveCursorLeft();
    buffer.moveCursorLeft();
    buffer.undo();
    buffer.undo();
    buffer.insert("and_right_now_you_can_save_50_pounds_perPerson");
    buffer.moveCursorLeft();
    buffer.deleteRange(2);
    buffer.moveCursorTo(7);
    buffer.deleteRange(5);
    assertEqual(buffer.getCursorPos(), 7, 33.7, "Cursor position after moving left");
    caught.clear();
    try
    {
        buffer.moveCursorTo(200);
    }
    catch (const out_of_range &e)
    {
        caught = e.what();
    }
    assertEqual(caught, "Index is invalid!", 33.8, "Moving cursor to invalid index throws exception");
    assertEqual(buffer.getCursorPos(), 7, 33.9, "Cursor position after moving to invalid index");
    assertEqual(buffer.getContent(), "Notand__now_you_can_save_50_pounds_perPersoing_beats_A_Jet2_holiday!", 33.10, "Content after moving to valid index");
    buffer.clear();
    assertEqual(buffer.getCursorPos(), 0, 33.11, "Cursor position after clear");
    assertEqual(buffer.getContent(), "", 33.12, "Content after clear");

    evaluateTests(12);
}

void sample_34()
{
    RopeTextBuffer buffer;

    buffer.insert("Implementing_the_Text_Buffer_using_Rope_helps_students_understand");
    assertEqual(buffer.getContent(), "Implementing_the_Text_Buffer_using_Rope_helps_students_understand", 34.1, "Content after insert");
    assertEqual(buffer.getCursorPos(), 65, 34.2, "Cursor position after insert");

    buffer.moveCursorTo(2);
    buffer.deleteRange(10);
    assertEqual(buffer.getContent(), "Im_the_Text_Buffer_using_Rope_helps_students_understand", 34.3, "Content after delete");
    assertEqual(buffer.getCursorPos(), 2, 34.4, "Cursor position after delete");

    buffer.deleteRange(5);
    assertEqual(buffer.getContent(), "ImText_Buffer_using_Rope_helps_students_understand", 34.5, "Content after delete");
    assertEqual(buffer.getCursorPos(), 2, 34.6, "Cursor position after delete");

    int remaining = (int)buffer.getContent().size() - buffer.getCursorPos();
    buffer.deleteRange(remaining);
    assertEqual(buffer.getContent(), "Im", 34.7, "Content after delete");
    assertEqual(buffer.getCursorPos(), 2, 34.8, "Cursor position after delete");
    string caught;
    try
    {
        buffer.deleteRange(10);
    }
    catch (const out_of_range &e)
    {
        caught = e.what();
    }
    assertEqual(caught, "Length is invalid!", 34.9, "Deleting range out of bounds throws exception");
    assertEqual(buffer.getContent(), "Im", 34.10, "Content after exception");
    assertEqual(buffer.getCursorPos(), 2, 34.11, "Cursor position after exception");

    buffer.moveCursorTo(0);
    caught.clear();
    try
    {
        buffer.deleteRange(6);
    }
    catch (const out_of_range &e)
    {
        caught = e.what();
    }
    assertEqual(caught, "Length is invalid!", 34.12, "Deleting range out of bounds throws exception");
    assertEqual(buffer.getContent(), "Im", 34.13, "Content after delete");
    assertEqual(buffer.getCursorPos(), 0, 34.14, "Cursor position after delete");
    evaluateTests(14);
}

void sample_35()
{
    RopeTextBuffer buffer;

    buffer.insert("Assignment_2_is_very_easy");
    assertEqual(buffer.getContent(), "Assignment_2_is_very_easy", 35.1, "Content after insert");
    assertEqual(buffer.getCursorPos(), 25, 35.2, "Cursor position after insert");

    buffer.moveCursorTo(21);
    buffer.replace(3, "difficult");
    assertEqual(buffer.getContent(), "Assignment_2_is_very_difficulty", 35.3, "Content after replace");
    assertEqual(buffer.getCursorPos(), 30, 35.4, "Cursor position after replace");
    buffer.moveCursorTo(20);
    buffer.replace(10, "_very_difficulty");
    assertEqual(buffer.getContent(), "Assignment_2_is_very_very_difficultyy", 35.5, "Content after replace");
    assertEqual(buffer.getCursorPos(), 36, 35.6, "Cursor position after replace");

    buffer.moveCursorRight();
    string caught;
    try
    {
        buffer.replace(3, "");
    }
    catch (const std::out_of_range &e)
    {
        caught = e.what();
    }
    assertEqual(caught, "Index is invalid!", 35.7, "Exception message");
    assertEqual(buffer.getContent(), "Assignment_2_is_very_very_difficultyy", 35.8, "Content after replace");
    assertEqual(buffer.getCursorPos(), 37, 35.9, "Cursor position after replace");

    caught.clear();
    try
    {
        buffer.replace(100, "test");
    }
    catch (const std::out_of_range &e)
    {
        caught = e.what();
    }
    assertEqual(caught, "Length is invalid!", 35.10, "Exception message");
    evaluateTests(10);
}

void sample_36()
{
    RopeTextBuffer buffer;

    buffer.insert("Abracadabra");
    assertEqual(buffer.getContent(), "Abracadabra", 36.1, "Content after insert");

    int pos = buffer.findFirst('a');
    assertEqual(pos, 3, 36.2, "First occurrence of 'a'");

    pos = buffer.findFirst('z');
    assertEqual(pos, -1, 36.3, "First occurrence of 'z'");

    int *positions = buffer.findAll('a');
    int expectedA[] = {3, 5, 7, 10};
    for (int i = 0; i < 4; ++i)
    {
        assertEqual(positions[i], expectedA[i], 36.4, "All occurrences of 'a'");
    }
    delete[] positions;
    positions = buffer.findAll('f');
    int checked = 0;
    if (positions == nullptr)
    {
        checked = -1;
    }
    assertEqual(checked, -1, 36.9, "All occurrences of 'f'");
    evaluateTests(8);
}
// test trong 3.1
void sample_37()
{
    RopeTextBuffer buffer;

    assertEqual(buffer.getContent(), "", 37.1, "Initial content should be empty");
    assertEqual(buffer.getCursorPos(), 0, 37.2, "Initial cursor position should be 0");

    buffer.insert("A");
    assertEqual(buffer.getContent(), "A", 37.3, "Content after insert");
    assertEqual(buffer.getCursorPos(), 1, 37.4, "Cursor position after insert");

    buffer.insert("CSE");
    assertEqual(buffer.getContent(), "ACSE", 37.5, "Content after insert");
    assertEqual(buffer.getCursorPos(), 4, 37.6, "Cursor position after insert");

    buffer.insert("HCMUT");
    assertEqual(buffer.getContent(), "ACSEHCMUT", 37.7, "Content after insert");
    assertEqual(buffer.getCursorPos(), 9, 37.8, "Cursor position after insert");

    buffer.moveCursorLeft();
    assertEqual(buffer.getCursorPos(), 8, 37.9, "Cursor position after move left");
    assertEqual(buffer.getContent(), "ACSEHCMUT", 37.10, "Content after move left");

    buffer.insert("123");
    assertEqual(buffer.getContent(), "ACSEHCMU123T", 37.11, "Content after insert");
    assertEqual(buffer.getCursorPos(), 11, 37.12, "Cursor position after insert");

    buffer.moveCursorTo(4);
    assertEqual(buffer.getCursorPos(), 4, 37.13, "Cursor position after move");
    assertEqual(buffer.getContent(), "ACSEHCMU123T", 37.14, "Content after move");

    buffer.deleteRange(3);
    assertEqual(buffer.getContent(), "ACSEU123T", 37.15, "Content after delete");
    assertEqual(buffer.getCursorPos(), 4, 37.16, "Cursor position after delete");

    buffer.undo();

    buffer.undo();

    buffer.undo();

    buffer.redo();

    buffer.redo();

    buffer.redo();
    buffer.moveCursorTo(0);
    buffer.insert("HCMUT");

    assertEqual(buffer.getContent(), "HCMUTACSEU123T", 37.17, "Final content after all operations");
    assertEqual(buffer.getCursorPos(), 5, 37.18, "Final cursor position after all operations");
    buffer.clear();
    assertEqual(buffer.getCursorPos(), 0, 37.19, "Cursor position after clear");
    evaluateTests(19);
}

void sample_38()
{
    RopeTextBuffer buffer;

    // 1. Insert "Hello"
    buffer.insert("Hello");
    assertEqual(buffer.getContent(), "Hello", 38.1, "Content after first insert");
    assertEqual(buffer.getCursorPos(), 5, 38.2, "Cursor position after first insert");

    buffer.insert(" World");
    assertEqual(buffer.getContent(), "Hello World", 38.3, "Content after second insert");
    assertEqual(buffer.getCursorPos(), 11, 38.4, "Cursor position after second insert");

    buffer.moveCursorTo(5);
    buffer.deleteRange(6);

    assertEqual(buffer.getContent(), "Hello", 38.5, "Content after delete");
    assertEqual(buffer.getCursorPos(), 5, 38.6, "Cursor position after delete");

    buffer.undo();
    assertEqual(buffer.getContent(), "Hello World", 38.7, "Content after undo");
    assertEqual(buffer.getCursorPos(), 5, 38.8, "Cursor position after undo");

    buffer.undo();
    assertEqual(buffer.getContent(), "Hello World", 38.9, "Content after undo");
    assertEqual(buffer.getCursorPos(), 11, 38.10, "Cursor position after undo");

    buffer.undo();
    assertEqual(buffer.getContent(), "Hello", 38.11, "Content after undo");
    assertEqual(buffer.getCursorPos(), 5, 38.12, "Cursor position after undo");

    buffer.moveCursorTo(2);
    buffer.moveCursorLeft();
    buffer.moveCursorRight();
    assertEqual(buffer.getCursorPos(), 2, 38.13, "Cursor position after move");

    buffer.undo();
    assertEqual(buffer.getCursorPos(), 1, 38.14, "Cursor position after undo");

    buffer.undo();
    assertEqual(buffer.getCursorPos(), 2, 38.15, "Cursor position after undo");

    buffer.replace(3, "LLO");
    assertEqual(buffer.getContent(), "HeLLO", 38.16, "Content after replace");
    assertEqual(buffer.getCursorPos(), 5, 38.17, "Cursor position after replace");

    buffer.undo();
    assertEqual(buffer.getContent(), "Hello", 38.18, "Content after undo");
    assertEqual(buffer.getCursorPos(), 2, 38.19, "Cursor position after undo");

    string historyOutput = captureOutput([&buffer]()
                                         { buffer.printHistory(); });
    string expectedHistory = "[(insert, 0, 5, Hello), (insert, 5, 11,  World), (move, 11, 5, J), (delete, 5, 5,  World), (move, 5, 2, J), (move, 2, 1, L), (move, 1, 2, R), (replace, 2, 5, llo)]\n";
    assertEqual(historyOutput, expectedHistory, 38.20, "History output after operations");

    buffer.insert("Hello");
    assertEqual(buffer.getContent(), "HeHellollo", 38.21, "Content after insert");
    assertEqual(buffer.getCursorPos(), 7, 38.22, "Cursor position after insert");

    evaluateTests(22);
}

void sample_39()
{
    RopeTextBuffer buffer;

    buffer.insert("Hello");
    assertEqual(buffer.getContent(), "Hello", 39.1, "Content after first insert");
    assertEqual(buffer.getCursorPos(), 5, 39.2, "Cursor position after first insert");

    buffer.insert(" World");

    assertEqual(buffer.getContent(), "Hello World", 39.3, "Content after second insert");
    assertEqual(buffer.getCursorPos(), 11, 39.4, "Cursor position after second insert");

    buffer.moveCursorTo(5);
    buffer.deleteRange(6);
    assertEqual(buffer.getContent(), "Hello", 39.5, "Content after delete");
    assertEqual(buffer.getCursorPos(), 5, 39.6, "Cursor position after delete");

    buffer.undo();
    assertEqual(buffer.getContent(), "Hello World", 39.7, "Content after undo");
    assertEqual(buffer.getCursorPos(), 5, 39.8, "Cursor position after undo");

    buffer.undo();
    assertEqual(buffer.getContent(), "Hello World", 39.9, "Content after undo");
    assertEqual(buffer.getCursorPos(), 11, 39.10, "Cursor position after undo");

    buffer.undo();
    assertEqual(buffer.getContent(), "Hello", 39.11, "Content after undo");
    assertEqual(buffer.getCursorPos(), 5, 39.12, "Cursor position after undo");

    buffer.undo();
    assertEqual(buffer.getContent(), "", 39.13, "Content after undo");
    assertEqual(buffer.getCursorPos(), 0, 39.14, "Cursor position after undo");

    buffer.redo();
    assertEqual(buffer.getContent(), "Hello", 39.15, "Content after redo");
    assertEqual(buffer.getCursorPos(), 5, 39.16, "Cursor position after redo");

    buffer.redo();
    assertEqual(buffer.getContent(), "Hello World", 39.17, "Content after redo");
    assertEqual(buffer.getCursorPos(), 11, 39.18, "Cursor position after redo");

    buffer.redo();
    assertEqual(buffer.getContent(), "Hello World", 39.19, "Content after redo");
    assertEqual(buffer.getCursorPos(), 5, 39.20, "Cursor position after redo");

    buffer.redo();
    assertEqual(buffer.getContent(), "Hello", 39.21, "Content after redo");
    assertEqual(buffer.getCursorPos(), 5, 39.22, "Cursor position after redo");

    buffer.moveCursorTo(2);
    buffer.moveCursorLeft();
    buffer.moveCursorRight();
    assertEqual(buffer.getCursorPos(), 2, 39.23, "Cursor position after move right");

    buffer.undo();
    assertEqual(buffer.getCursorPos(), 1, 39.24, "Cursor position after undo");

    buffer.undo();
    assertEqual(buffer.getCursorPos(), 2, 39.25, "Cursor position after undo");

    buffer.redo();
    assertEqual(buffer.getCursorPos(), 1, 39.26, "Cursor position after redo");

    buffer.redo();
    assertEqual(buffer.getCursorPos(), 2, 39.27, "Cursor position after redo");

    buffer.replace(3, "LLO1");
    assertEqual(buffer.getContent(), "HeLLO1", 39.28, "Content after replace");
    assertEqual(buffer.getCursorPos(), 6, 39.29, "Cursor position after replace");

    buffer.undo();
    assertEqual(buffer.getContent(), "Hello", 39.30, "Content after undo");
    assertEqual(buffer.getCursorPos(), 2, 39.31, "Cursor position after undo");

    buffer.redo();
    assertEqual(buffer.getContent(), "HeLLO1", 39.32, "Content after redo");
    assertEqual(buffer.getCursorPos(), 6, 39.33, "Cursor position after redo");
    evaluateTests(33);
}

void sample_40()
{
    RopeTextBuffer buffer;

    buffer.insert("A");
    buffer.insert("CSE");
    buffer.insert("HCMUT");
    buffer.moveCursorLeft();
    buffer.insert("123");
    buffer.moveCursorTo(4);
    buffer.deleteRange(3);

    assertEqual(buffer.getContent(), "ACSEU123T", 40.1, "Content after operations");
    assertEqual(buffer.getCursorPos(), 4, 40.2, "Cursor position after operations");

    buffer.undo();
    assertEqual(buffer.getContent(), "ACSEHCMU123T", 40.3, "Content after undo");
    assertEqual(buffer.getCursorPos(), 4, 40.4, "Cursor position after undo");

    buffer.undo();

    assertEqual(buffer.getContent(), "ACSEHCMU123T", 40.5, "Content after undo");
    assertEqual(buffer.getCursorPos(), 11, 40.6, "Cursor position after undo");

    buffer.undo();

    assertEqual(buffer.getContent(), "ACSEHCMUT", 40.7, "Content after undo");
    assertEqual(buffer.getCursorPos(), 8, 40.8, "Cursor position after undo");

    buffer.redo();
    assertEqual(buffer.getContent(), "ACSEHCMU123T", 40.9, "Content after redo");
    assertEqual(buffer.getCursorPos(), 11, 40.10, "Cursor position after redo");

    buffer.redo();
    assertEqual(buffer.getContent(), "ACSEHCMU123T", 40.11, "Content after redo");
    assertEqual(buffer.getCursorPos(), 4, 40.12, "Cursor position after redo");

    buffer.redo();
    assertEqual(buffer.getContent(), "ACSEU123T", 40.13, "Content after redo");
    assertEqual(buffer.getCursorPos(), 4, 40.14, "Cursor position after redo");
    evaluateTests(14);
}

void sample_41()
{
    RopeTextBuffer buffer;
    buffer.insert("Hello");
    buffer.insert(" World");
    buffer.undo();
    buffer.insert(" World2");
    buffer.redo();
    assertEqual(buffer.getContent(), "Hello World2", 41.1, "Content after redo");
    assertEqual(buffer.getCursorPos(), 12, 41.2, "Cursor position after redo");
    evaluateTests(2);
}

void sample_42()
{
    RopeTextBuffer buffer;
    buffer.insert("Hello");
    buffer.insert(" World");
    buffer.moveCursorLeft();
    buffer.undo();
    buffer.insert(" World2");
    buffer.redo();
    assertEqual(buffer.getContent(), "Hello World World2", 43.1, "Content after redo");
    assertEqual(buffer.getCursorPos(), 18, 43.2, "Cursor position after redo");
    evaluateTests(2);
}

void sample_43()
{
    RopeTextBuffer buffer;
    buffer.insert("Hello");
    buffer.insert(" World");
    buffer.moveCursorTo(0);
    buffer.replace(5, "Goodbye");
    string historyOutput = captureOutput([&buffer]()
                                         { buffer.printHistory(); });
    assertEqual(historyOutput, "[(insert, 0, 5, Hello), (insert, 5, 11,  World), (move, 11, 0, J), (replace, 0, 7, Hello)]\n", 43.1, "History output after operations");
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
    sample_25();
    sample_26();
    sample_27();
    sample_28();
    sample_29();
    sample_30();
    sample_31();
    sample_32();
    sample_33();
    sample_34();
    sample_35();
    sample_36();
    sample_37();
    sample_38();
    sample_39();
    sample_40();
    sample_41();
    sample_42();
    sample_43();

    cout << "=" << string(50, '=') << endl;
    cout << COLOR_PURPLE << "All tests completed!" << COLOR_RESET << endl;
    cout << "You have passed " << COLOR_GREEN << countOfPassedTests << COLOR_RESET << "/" << 42 << " testcases!" << endl;
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
