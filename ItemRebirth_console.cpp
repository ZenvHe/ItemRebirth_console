#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <locale>    // 用于设置本地化
#include <codecvt>   // 用于UTF-8编码转换
#ifdef _WIN32
#include <fcntl.h>   // 用于设置 Windows 下的输出模式
#include <io.h>      // 用于 _setmode
#endif

using namespace std;

// 定义物品结构体
struct Item {
    wstring name;
    wstring description;
    wstring contactInfo;

    // 构造函数
    Item(wstring n, wstring d, wstring c) : name(n), description(d), contactInfo(c) {}
};

// 定义物品管理类
class ItemManager {
private:
    vector<Item> items;
    const string fileName = "items.txt"; // 保存物品数据的文件名

    // 从文件加载物品
    void loadFromFile() {
        wifstream file(fileName);
        file.imbue(locale(file.getloc(), new codecvt_utf8<wchar_t>));  // 设置为UTF-8
        if (file.is_open()) {
            wstring line, name, description, contactInfo;
            while (getline(file, line)) {
                if (line.rfind(L"物品名称: ", 0) == 0) {
                    name = line.substr(6);  // 提取名称
                    getline(file, line);
                    description = line.substr(6);  // 提取描述
                    getline(file, line);
                    contactInfo = line.substr(7);  // 提取联系人信息
                    items.push_back(Item(name, description, contactInfo));
                    getline(file, line);  // 跳过分隔符 ---
                }
            }
            file.close();
        }
    }

    // 将物品保存到文件
    void saveToFile() const {
        wofstream file(fileName);
        file.imbue(locale(file.getloc(), new codecvt_utf8<wchar_t>));  // 设置为UTF-8
        if (file.is_open()) {
            for (const auto& item : items) {
                file << L"物品名称: " << item.name << endl;
                file << L"物品描述: " << item.description << endl;
                file << L"联系人信息: " << item.contactInfo << endl;
                file << L"---" << endl;  // 物品分隔符
            }
            file.close();
        }
    }

public:
    // 构造函数：启动时加载文件中的物品信息
    ItemManager() {
        loadFromFile();
    }

    // 添加物品
    void addItem(const wstring& name, const wstring& description, const wstring& contactInfo) {
        Item newItem(name, description, contactInfo);
        items.push_back(newItem);
        saveToFile();  // 每次添加物品后保存到文件
        wcout << L"物品添加成功！" << endl;
    }

    // 删除物品
    void deleteItem(const wstring& name) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->name == name) {
                items.erase(it);
                saveToFile();  // 每次删除物品后保存到文件
                wcout << L"物品删除成功！" << endl;
                return;
            }
        }
        wcout << L"未找到该物品！" << endl;
    }

    // 显示所有物品
    void displayItems() const {
        if (items.empty()) {
            wcout << L"目前没有物品可供展示！" << endl;
            return;
        }

        wcout << L"物品列表：" << endl;
        for (const auto& item : items) {
            wcout << L"物品名称: " << item.name << endl;
            wcout << L"物品描述: " << item.description << endl;
            wcout << L"联系人信息: " << item.contactInfo << endl;
            wcout << L"----------------------------" << endl;
        }
    }

    // 查找物品
    void searchItem(const wstring& name) const {
        for (const auto& item : items) {
            if (item.name == name) {
                wcout << L"找到物品：" << endl;
                wcout << L"物品名称: " << item.name << endl;
                wcout << L"物品描述: " << item.description << endl;
                wcout << L"联系人信息: " << item.contactInfo << endl;
                return;
            }
        }
        wcout << L"未找到该物品！" << endl;
    }
};

// 函数用于获取用户输入的有效数字
int getValidChoice() {
    int choice;
    while (true) {
        wcout << L"请选择操作：";
        wcin >> choice;

        // 检查输入是否为数字
        if (wcin.fail()) {
            wcin.clear(); // 清除错误状态
            wcin.ignore(numeric_limits<streamsize>::max(), '\n'); // 忽略当前输入的错误内容
            wcout << L"无效输入，请输入一个数字！" << endl;
        }
        else {
            wcin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除缓冲区剩余的换行符
            return choice;
        }
    }
}

// 函数用于清除控制台内容
void clearConsole() {
#ifdef _WIN32
    system("cls");  // Windows 系统
#else
    system("clear");  // Linux 或 macOS 系统
#endif
}

// 主程序
int main() {
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_U8TEXT);  // 设置Windows控制台为UTF-8模式
    _setmode(_fileno(stdin), _O_U8TEXT);   // 设置输入模式为UTF-8
#endif

    ItemManager manager;
    int choice;
    wstring name, description, contactInfo;

    do {
        clearConsole();  // 每次显示菜单前清除控制台
        wcout << L"\n--- 物品“复活”程序 ---" << endl;
        wcout << L"1. 添加物品" << endl;
        wcout << L"2. 删除物品" << endl;
        wcout << L"3. 显示所有物品" << endl;
        wcout << L"4. 查找物品" << endl;
        wcout << L"5. 退出" << endl;

        // 获取有效的数字输入
        choice = getValidChoice();

        switch (choice) {
        case 1:
            wcout << L"输入物品名称: ";
            getline(wcin, name);
            wcout << L"输入物品描述: ";
            getline(wcin, description);
            wcout << L"输入联系人信息: ";
            getline(wcin, contactInfo);
            manager.addItem(name, description, contactInfo);
            break;

        case 2:
            wcout << L"输入要删除的物品名称: ";
            getline(wcin, name);
            manager.deleteItem(name);
            break;

        case 3:
            manager.displayItems();
            break;

        case 4:
            wcout << L"输入要查找的物品名称: ";
            getline(wcin, name);
            manager.searchItem(name);
            break;

        case 5:
            wcout << L"退出程序。" << endl;
            break;

        default:
            wcout << L"无效选择，请重新输入。" << endl;
        }

        // 暂停等待用户按键后继续刷新
        wcout << L"按任意键继续..." << endl;
        wcin.get();

    } while (choice != 5);

    return 0;
}
