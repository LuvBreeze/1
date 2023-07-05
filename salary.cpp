#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
using namespace std;

constexpr auto File = "Workers.txt";

float Volume[20]; //

/*
4.课程题目：工资管理程序
内容要求：使用文本命令行窗口设计工资管理程序。要求：
（1）公司主要有4类人员：经理、技术员、销售员、销售经理。要求存储这些人的职工号、姓名、月工资、岗位、年龄、性别等信息。
（2）工资的计算方法：
A.经理：固定月薪为8000；
B.技术员：工作时间*小时工资（100元每小时）；
C.销售员：销售额*4%提成；
D.销售经理：底薪（5000）+所辖部门销售额总额*0.5%；
（3）输入数据要求每类人员不能少于4人，并按以下格式输出：
职工号 姓名 性别 年龄 岗位 工资 排名及某部门经理所辖部门各售货员的业绩及自己的工资表。
*/

// 基类 - 职工
class Worker {
public:
    int rank;
    Worker() {
        num = "none";
        name = "unknown";
        sex = "unknown";
        salary = 0;
        age = 0;
        rank = 0;
        volume = 0;
        staff[0, 1, 2] = "无";
    }

    virtual void Calculate() = 0;  // 用于计算工资
    friend bool compare(const Worker* w1, const Worker* w2);    // 用于排序
    friend void AddWorkers_Show(int c);  //用于创建对象和输出数据
    friend class AddW;     //用于添加员工

    void Show() {
        cout << "| ";
        cout << num << " | ";
        cout << name << " |   ";
        cout << sex << " |   ";
        cout << age << " |   ";
        cout << position << " | ";
        cout << salary << "元 |   ";
        cout << rank << "  |" << endl;
    }
    void Show_Sm() {
        static int i = 0;
        cout << "姓名:" << name << "\t" << "职位:" << position << "\t" << endl;
        cout << "员工1:" << staff[0] << "\t营业额:" << Volume[i] << "元" << endl;
        cout << "员工2:" << staff[1] << "\t营业额:" << Volume[i + 1] << "元" << endl;
        cout << "员工3:" << staff[2] << "\t营业额:" << Volume[i + 2] << "元" << endl;
        cout << "底薪:5000.0\t" << "部门分成:" << (Volume[i] + Volume[i + 1] + Volume[i + 2]) * 0.005 << "\t" << "总工资:" << (Volume[i] + Volume[i + 1] + Volume[i + 2]) * 0.005 + 5000 + 0.01 << endl;
        cout << "*******************************************************" << endl;
        i += 3;
    }
    void Save() {
        fstream outfile(File, ios::out | ios::app);
        if (!outfile) {
            cout << "open error!!" << endl;
            exit(1);
        }
        outfile << "职工号:" << num << "\t" << "姓名:" << name << "\t" << "性别:" << sex << "\t"
            << "年龄:" << age << "\t" << "月薪:" << salary << "\t" << "岗位:" << position <<"\t "<<"月薪排名:"<<rank<<endl;
        outfile.close();
    }
protected:
    string num, name, sex, position, staff[3];
    double salary, volume;
    int age;
};

// 经理类
class Manager : public Worker {
public:
    Manager() : Worker() {}

    Manager(string nu, string n, string se, int a) {
        num = nu;
        name = n;
        sex = se;
        age = a;
        position = "经理    ";
    }

    void Calculate() {
        salary = 8000.01;
    }
};

// 销售经理类
class SalesManager : public Worker {
public:
    SalesManager() : Worker() {}

    SalesManager(string nu, string n, string se, int a) {
        num = nu;
        name = n;
        sex = se;
        age = a;
        position = "销售经理";
    }

    void Calculate() {
        static  int i = 0;
        float any = Volume[i] + Volume[i + 1] + Volume[i + 2];
        salary = 5000 + 0.01 + any * 0.005;
        i += 3;
    }
    friend void AddWorkers_Show(int c);
private:
    string staff[3];
};

// 技术员类
class Technician : public Worker {
public:
    Technician() : Worker() {}

    Technician(string nu, string n, string se, int a, float h) {
        num = nu;
        name = n;
        sex = se;
        age = a;
        hour = h;
        position = "技术员  ";
    }

    void Calculate() {
        salary = hour * 100 + 0.01;
    }

private:
    float hour;
};

// 销售员类
class Salesman : public Worker {
public:
    int i = 0;
    Salesman() : Worker() {}
    Salesman(string nu, string n, string se, int a, float v) {
        num = nu;
        name = n;
        sex = se;
        volume = v;
        age = a;
        position = "销售员  ";
    }
    void Calculate() {
        static int i = 0;
        salary = volume * 0.04 + 0.02;
        Volume[i] = volume;
        i++;
    }
    void Caculate_Sm(int i) {
        Volume[i] = volume;
    }
    friend void AddWorkers_Show(int c);
private:
    float volume;
};

bool compare(const Worker* w1, const Worker* w2) {
    return w1->salary > w2->salary;
}

//用于添加员工
SalesManager Add_Sm() {
    string num, name, sex;
    int age;
    cout << "输入职工号:" << endl;
    cin >> num;
    num.append("Sm");               //职工号分类
    cout << "输入姓名:" << endl;
    cin >> name;
    cout << "输入性别:" << endl;
    cin >> sex;
    cout << "输入年龄:" << endl;
    cin >> age;
    SalesManager Sm(num, name, sex, age);
    return Sm;
}
Manager Add_M() {
    string num, name, sex;
    int age;
    cout << "输入职工号:" << endl;
    cin >> num;
    num.append("M");
    cout << "输入姓名:" << endl;
    cin >> name;
    cout << "输入性别:" << endl;
    cin >> sex;
    cout << "输入年龄:" << endl;
    cin >> age;
    Manager M(num, name, sex, age);
    return M;
}
Technician Add_T() {
    string num, name, sex;
    int age;
    float hour;
    cout << "输入职工号:" << endl;
    cin >> num;
    num.append("T");
    cout << "输入姓名:" << endl;
    cin >> name;
    cout << "输入性别:" << endl;
    cin >> sex;
    cout << "输入年龄:" << endl;
    cin >> age;
    cout << "输入工作时间" << endl;
    cin >> hour;
    Technician T(num, name, sex, age, hour);
    return T;

}
Salesman Add_S() {
    string num, name, sex;
    int age;
    float volume;
    cout << "输入职工号:" << endl;
    cin >> num;
    num.append("S");
    cout << "输入姓名:" << endl;
    cin >> name;
    cout << "输入性别:" << endl;
    cin >> sex;
    cout << "输入年龄:" << endl;
    cin >> age;
    cout << "输入营业额:" << endl;
    cin >> volume;
    Salesman S(num, name, sex, age, volume);
    return S;
}

int Show_Add() {
    int c;
    cout << endl << endl;
    cout << "请输入添加员工类型:" << endl;
    cout << "******0.添加经理********" << endl;
    cout << "******1.添加销售经理****" << endl;
    cout << "******2.添加销售员******" << endl;
    cout << "******3.添加技术员******" << endl;
    cout << "******4-.返回上一级*****" << endl;
    cout << endl;
    cout << "请输入你的选择:" << endl;
    cin >> c; //接收用户的选项
    return c;
}

void AddWorkers_Show(int c) {

    void menu_();

    // 创建容器
    vector<Worker*> workers;
    vector<Worker*> Smanagers;

    // 添加经理
    workers.push_back(new Manager("1001M ", "张德天", "男", 30));
    workers.push_back(new Manager("1002M ", "李娣  ", "女", 35));
    workers.push_back(new Manager("1003M ", "王辉阳", "男", 28));
    workers.push_back(new Manager("1004M ", "刘红  ", "女", 32));

    // 添加技术员
    workers.push_back(new Technician("2001T ", "闫飞舟", "男", 25, 76.5));
    workers.push_back(new Technician("2002T ", "伊晗  ", "女", 26, 78.7));
    workers.push_back(new Technician("2003T ", "程安然", "男", 23, 75.5));
    workers.push_back(new Technician("2004T ", "屈迎梅", "女", 24, 77.9));

    // 添加销售员
    workers.push_back(new Salesman("3001S ", "吴俊  ", "男", 27, 140001.13));
    workers.push_back(new Salesman("3002S ", "祈香岚", "女", 29, 150003.67));
    workers.push_back(new Salesman("3004S ", "李惠  ", "女", 28, 130505.39));
    workers.push_back(new Salesman("3001S ", "缑鸿振", "男", 27, 133332.73));
    workers.push_back(new Salesman("3003S ", "俟正祥", "男", 26, 122566.15));
    workers.push_back(new Salesman("3002S ", "雷采春", "女", 29, 153235.31));
    workers.push_back(new Salesman("3003S ", "马新儿", "男", 26, 132522.37));
    workers.push_back(new Salesman("3004S ", "郭暎  ", "女", 28, 135000.23));
    workers.push_back(new Salesman("3001S ", "秋建本", "男", 27, 118961.87));
    workers.push_back(new Salesman("3002S ", "安绿兰", "女", 29, 162537.39));
    workers.push_back(new Salesman("3003S ", "禹泽  ", "男", 26, 162563.15));
    workers.push_back(new Salesman("3004S ", "闻珺  ", "女", 28, 171633.83));




    // 添加销售经理
    workers.push_back(new SalesManager("4001Sm", "聂志尚", "男", 33));
    workers.push_back(new SalesManager("4002Sm", "施水儿", "女", 36));
    workers.push_back(new SalesManager("4003Sm", "施祺祥", "男", 31));
    workers.push_back(new SalesManager("4004Sm", "许慕悦", "女", 34));


    Smanagers.push_back(new SalesManager("4001Sm", "聂志尚", "男", 33));
    Smanagers.push_back(new SalesManager("4002Sm", "施水儿", "女", 36));
    Smanagers.push_back(new SalesManager("4003Sm", "施祺祥", "男", 31));
    Smanagers.push_back(new SalesManager("4004Sm", "许慕悦", "女", 34));

    Smanagers[0]->staff[0] = workers[8]->name;
    Smanagers[0]->staff[1] = workers[9]->name;
    Smanagers[0]->staff[2] = workers[10]->name;

    Smanagers[1]->staff[0] = workers[11]->name;
    Smanagers[1]->staff[1] = workers[12]->name;
    Smanagers[1]->staff[2] = workers[13]->name;

    Smanagers[2]->staff[0] = workers[14]->name;
    Smanagers[2]->staff[1] = workers[15]->name;
    Smanagers[2]->staff[2] = workers[16]->name;

    Smanagers[3]->staff[0] = workers[17]->name;
    Smanagers[3]->staff[1] = workers[18]->name;
    Smanagers[3]->staff[2] = workers[19]->name;

    //手动添加的员工
    while (c == 2) {
        int i = Show_Add();
        if (i == 0) {
            workers.push_back(new Manager(Add_M()));
            cout << "经理添加成功!" << endl;
        }
        else if (i == 1) {
            workers.push_back(new SalesManager(Add_Sm()));
            cout << "销售经理添加成功!" << endl;
        }
        else if (i == 2) {
            workers.push_back(new Salesman(Add_S()));
            cout << "销售员添加成功" << endl;
        }
        else if (i == 3) {
            workers.push_back(new Technician(Add_T()));
            cout << "技术员添加成功!" << endl;
        }
        else {
            menu_();
        }
    }


        // 计算工资
        for (auto worker : workers) {
            worker->Calculate();
        }


        // 排序
        sort(workers.begin(), workers.end(), compare);

        // 添加薪水排名
        for (int a = 0; a < workers.size(); a++) {
            workers[a]->rank = a + 1;
        }
        
       while (c == 1) {
            // 输出数据
            cout << "| 职工号 | 姓名   | 性别 | 年龄 |   岗位     | 工资      | 排名 |" << endl;
            cout << "-----------------------------------------------------------------" << endl;
            for (int c = 0; c < workers.size(); c++) {
                workers[c]->Show();
            }
           
            //销售经理数据
            cout << endl << endl;
            cout << "*******************************************************" << endl;
            Smanagers[0]->Show_Sm();
            Smanagers[1]->Show_Sm();
            Smanagers[2]->Show_Sm();
            Smanagers[3]->Show_Sm();
            c--;
        }


        //写入文件
        for (int b = 0; b < workers.size(); b++) {
            workers[b]->Save();
        }


    // 清除缓存并释放内存
    for (auto worker : workers) {
        delete worker;
    }
}

void exit() {                //退出

    cout << "欢迎下次使用!" << endl;

    system("pause");
    exit(0);

}
void show_menu() {           //展示菜单

    cout << "---------------------------" << endl;
    cout << " 欢迎使用工资管理程序！" << endl;

    cout << "      0.退出管理系统       " << endl;

    cout << "      1.显示员工信息       " << endl;

    cout << "      2.增加员工信息       " << endl;
    cout << "---------------------------" << endl;
    cout << endl;
}

//菜单
void menu_() {
    int c = 1; //用于存储用户的选项

    while (c) {
        show_menu();
        cout << "请输入你的选择:";
        cin >> c;   //接收用户的选项

        switch (c) {
        case 0:exit();
            break;

        case 1:AddWorkers_Show(c);
            break;

        case 2:AddWorkers_Show(c);
            break;

        default:system("cls"); //清屏
            break;
        }
       c-- ;
    }
}

int main() {
    menu_();
    system("pause");

    return 0;
}
