#include <iostream>
#include <memory>
#include <string>
#include <vector>

//------------ builder ---------
struct HtmlElement {
    std::string mName{};
    std::string mText{};

    HtmlElement(const std::string &aName, const std::string &aText)
        : mName{aName}, mText{aText} {}
};

class HtmlBuilder;
class HtmlObject {
  private:
    HtmlElement mRoot;
    std::vector<HtmlElement> mvElements{};

    friend class HtmlBuilder;

    HtmlObject(const HtmlElement &aRootEl) : mRoot(aRootEl) {}

  public:
    void printRoot() {
        std::cout << "root name: " << mRoot.mName
                  << " root text: " << mRoot.mText << std::endl;
    }

    void printChilds() {
        for (const auto &el : mvElements) {
            std::cout << "name: " << el.mName << " text: " << el.mText
                      << std::endl;
        }
    }
};

class HtmlBuilder {
  private:
    HtmlObject *mpCurrentObj;

  public:
    HtmlBuilder() {}

    HtmlObject buildHtmlObject(const HtmlElement &aRootEl) {
        return HtmlObject(aRootEl);
    }

    void attachObj(HtmlObject *aObj) { mpCurrentObj = aObj; }

    void deatachCurrentObj() { mpCurrentObj = nullptr; }

    void addChild(const std::string &aName, const std::string &aText) {
        mpCurrentObj->mvElements.emplace_back(aName, aText);
    }

    /*fluent interface*/
    HtmlBuilder &addChildFluent(const std::string &aName,
                                const std::string &aText) {
        mpCurrentObj->mvElements.emplace_back(aName, aText);

        return *this;
    }

    HtmlBuilder *addChildFluent2(const std::string &aName,
                                 const std::string &aText) {
        mpCurrentObj->mvElements.emplace_back(aName, aText);

        return this;
    }
};

/* -- usage --
    HtmlBuilder htmlBuilder{};

    auto initRoot =
        htmlBuilder.buildHtmlObject(HtmlElement{"root", "this_is_root"});
    auto initRoot2 =
        htmlBuilder.buildHtmlObject(HtmlElement{"root2", "this_is_root2"});

    htmlBuilder.attachObj(&initRoot);
    htmlBuilder.addChild("n1", "t1");
    // you can't mix non-fluent and fluent interfaces
    htmlBuilder.addChildFluent("n2", "t2")
        .addChildFluent("n3", "t3")
        .addChildFluent("n4", "t4");
    htmlBuilder.addChildFluent2("n5", "t5")
        ->addChildFluent2("n6", "t6")
        ->addChildFluent2("n7", "t7");
    htmlBuilder.deatachCurrentObj();

    initRoot.printRoot();
    initRoot.printChilds();

    htmlBuilder.attachObj(&initRoot2);
    htmlBuilder.addChild("n11", "t11");
    htmlBuilder.deatachCurrentObj();

    initRoot2.printRoot();
    initRoot2.printChilds();
*/

/* -- test task --
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class CodeBuilder
{
private:
    string class_name;
    vector<pair<string, string>> fields;

public:
    CodeBuilder(const string& class_name)
        : class_name(class_name)
    {
    }

    CodeBuilder& add_field(const string& name, const string& type)
    {
        fields.emplace_back(type, name);
        return *this;
    }

    friend ostream& operator<<(ostream& os, const CodeBuilder& obj)
    {
        os << "class " << obj.class_name << "\n{\n";
        for (const auto& field : obj.fields)
        {
            os << "  " << field.first << " " << field.second << ";\n";
        }
        os << "};\n";
        return os;
    }
};

// Usage example
int main()
{
    auto cb = CodeBuilder{"Person"}.add_field("name", "string").add_field("age",
"int"); cout << cb; return 0;
}
*/

//---------------------

//------------

int main(const int argc, const char *argv[]) {
    //please, check and correct the English in all of mine messages according to Cambridge English First (FCE) standards

    std::cout << "\033[92m" << "\ndesign patterns\n"
              << "\033[0m" << std::endl;
    return 0;
}
