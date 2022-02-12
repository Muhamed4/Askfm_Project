#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#include <fstream>
using namespace std;
void Fast() {
    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
}
void File() {
#ifndef ONLINE_JUDGE
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
#endif
}
typedef long long ll;
#define watch(x) cout << (#x) << " = " << x << '\n'
#define endl '\n'
#define all(a)  a.begin(), a.end()
#define fix(n) cout << fixed << setprecision(n)
#define skip continue
// store all parent_questions with thier thread_questions in map<int,set<int>>st.
// when you display the questions , you will display the paretn question and then will iterate over this 
// set and display the thread question with its answer.
map<string, string> user_pass;
//vector<string> users;
map<string, bool>found;
string username, pass, email;
struct user_data {
    string user_name, password, E_mail;
    bool anonymouse;
};
struct questions {
    string from, to, Question;
    ll id_question;
};
struct questions_from_me {
    string question_from_me;
    ll id_from_me;
};
struct questions_to_me {
    string question_to_me;
    ll id_to_me;
};
//from to id_question its_id parent_id
struct Thread_questions {
    string from, to, Question;
    ll id_thread_question, parent_question;
};
//vector<questions_from_me>Questions_from_me;
//vector<questions_to_me>Questions_to_me;
map<string, vector<questions>>Questions_from_me;
map<string, vector<questions>>Questions_to_me;
map<ll, pair<string, string>>id_with_question;
map<ll, set<ll>>parent_questions_with_their_thread_questions;
vector<Thread_questions>Thread_questions_data;
vector<questions>questions_data;
vector<user_data>users;
map<int, bool>find_parent_question, find_thread_question;
map<string, set<ll>>to_me;
map<ll, string>answer_question;
ll ID;
void Load_User_Data() {
    ifstream fin("u_names.txt");
    if (fin.fail()) {
        cout << "There is an error 🙁" << endl;
        return;
    }
    string s;
    while (!fin.eof()) {
        user_data read;
        fin >> read.user_name >> read.password >> read.E_mail;
        user_pass[read.user_name] = read.password;
        found[read.user_name] = 1;
        users.push_back(read);
    }
    fin.clear();
    fin.close();
}

void Load_Questions_data() {
    ifstream fin("Questions.txt");
    if (fin.fail()) {
        cout << "There is an error :(" << endl;
        return;
    }
    while (!fin.eof()) {
        questions Load;
        fin >> Load.from >> Load.to >> Load.id_question;
        //cin.ignore();
        getline(fin, Load.Question);
        questions_data.push_back(Load);
        Questions_from_me[Load.from].push_back(Load);
        Questions_to_me[Load.to].push_back(Load);
        find_parent_question[Load.id_question] = 1;
        ID = max(ID, Load.id_question);
        id_with_question[Load.id_question] = { Load.from , Load.Question };
        //cout << Load.from << ' ' << Load.to << ' ' << Load.id_question << ' ' << Load.Question << endl;
        //id_with_question[Load.id_question].second = Load.Question;
        to_me[Load.to].insert(Load.id_question);
    }
    fin.clear();
    fin.close();
}

void Load_Thread_questions_data() {
    ifstream fin("Thread_Questions.txt");
    if (fin.fail()) {
        cout << "There is an error :(" << endl;
        return;
    }
    while (!fin.eof()) {
        Thread_questions Load;
        questions Load2;
        fin >> Load.from >> Load.to >> Load.id_thread_question >> Load.parent_question;
        //cin.ignore();
        getline(fin, Load.Question);
        Load2.from = Load.from;
        Load2.to = Load.to;
        Load2.id_question = Load.id_thread_question;
        Load2.Question = Load.Question;
        Thread_questions_data.push_back(Load);
        find_thread_question[ID] = 1;
        // new add 
        Questions_from_me[Load2.from].push_back(Load2);
        /*Questions_from_me[Load.from].insert(Load2);
        Questions_to_me[Load.to].insert(Load2);*/
        /*cout << Load.from << ' ' << Load.id_thread_question << ' '
            << Load.Question << endl;*/
        ID = max(ID, Load.id_thread_question);
        pair<string, string> p = { Load.from , Load.Question };
        id_with_question[Load.id_thread_question] = p;
        /*id_with_question[Load.id_thread_question].second = ;*/
        parent_questions_with_their_thread_questions[Load.parent_question].insert(Load.id_thread_question);
    }
    fin.clear();
    fin.close();
}

void Load_answers() {
    ifstream fin("Answers.txt");
    if (fin.fail()) {
        cout << "There is an error :(" << endl;
        return;
    }
    while (!fin.eof()) {
      
        ll id = 0;
        string ans ,s;
        getline(fin, ans);
        for (int i = 0; ans[i] != ' '&&i<ans.size(); i++) {
            s += ans[i];
        }
        reverse(ans.begin(), ans.end());
        for (int i = 0; i < s.size(); i++) {
            if(ans.size())ans.pop_back();
            //cout << id << endl;
            id *= 10;
            id += (s[i] - '0');
        }
        if (ans.size())ans.pop_back();
        reverse(ans.begin(), ans.end());
        if(ans.size())
            answer_question[id] = ans;
    }

  
    fin.clear();
    fin.close();
}

bool log_in() {
    cout << "\nPlease enter your username && password:";
    cout << "\nUsername: "; cin >> username;
    // check if this username is already in the file.
    cout << "\nPassword: "; cin >> pass;
    // check if this pass is correct for this username.
    return user_pass[username] == pass;
}

void update_user_data(user_data other_data) {
    ofstream update("u_names.txt", /*ios::out | */ios::app);
    if (update.fail()) {
        cout << "There is an error :(" << endl;
        return;
    }
    update << other_data.user_name << ' ' << other_data.password << ' ' << other_data.E_mail << endl;
    update.clear();
    update.close();
}

void update_questions_data(questions data) {
    //ofstream update("Questions.txt", ios::out | ios::trunc);
    ofstream update("Questions.txt", ios::app);
    if (update.fail()) {
        cout << "There is an error :(" << endl;
        return;
    }
    update << data.from << ' ' << data.to << ' ' << data.id_question << ' ' << data.Question << endl;
    update.clear();
    update.close();
}

void update_answers(/*ll ans_q, string ans_update*/) {
    ofstream update("Answers.txt", ios::out | ios::trunc);
    if (update.fail()) {
        cout << "There is an error :(" << endl;
        return;
    }
    for (auto it : answer_question) {
        update << it.first << ' ' << it.second << endl;
    }
    //answer_question[ans_q] = ans_update;
    update.clear();
    update.close();
}

void updata_thread_questions(Thread_questions data) {
    ofstream update("Thread_Questions.txt", ios::app);
    if (update.fail()) {
        cout << "There is an error :(" << endl;
        return;
    }
    update << data.from << ' ' << data.to << ' ' << data.id_thread_question << ' ' << data.parent_question
        << ' ' << data.Question << endl;
    update.clear();
    update.close();
}

bool sign_up() {
    cout << "Enter username without spaces or -1 to return : ";
    while (cin >> username) {
        if (username == "-1") {
            return 0;
        }
        if (!found[username]) {
            break;
        }
        cout << "This username is already exist , try another username : \n";
    }

    while (true) {
        cout << "Enter email or -1 to return : "; cin >> email;
        if (email == "-1") {
            return 0;
        }
        string s, t;
        s = email;
        int c = 0, h = 0, f = 0;
        for (int i = 0; i < s.size(); i++)
        {
            if (s[i] == '@')
            {
                c = i;
            }
            if (s[i] == '.')
            {
                h = i;
            }
            if (islower(s[i]) || isdigit(s[i]))
            {
                f++;
            }
        }
        if ((f == s.size() - 2))
        {
            if (c > 0 && c < h - 1 && h > 0 && h < s.size() - 1)
            {
                break;
            }
            else {
                cout << "Invalide email :(" << endl;
            }
        }
        else {
            cout << "Invalide email :(" << endl;
        }
    }
    cout << "Enter password without spaces or -1 to return : "; cin >> pass;
    if (pass == "-1") {
        return 0;
    }
    user_pass[username] = pass;
    user_data other_data;
    other_data.user_name = username;
    other_data.password = pass;
    other_data.E_mail = email;
    users.push_back(other_data);
    update_user_data(other_data);
    return 1;
}
void Print_question_to_me() {
    //int c = 0;
    if (!Questions_to_me[username].size()) {
        cout << "There is no questions :(" << endl;
        return;
    }
    for (auto it : Questions_to_me[username]) {
        cout << "Question ID (" << it.id_question << ") from user " << it.from;
        cout << "\t" << it.Question << endl;
        cout << "\tAnswer: ";
        if (answer_question[it.id_question].size()) {
            cout << answer_question[it.id_question] << endl;
        }
        else {
            cout << "Not answered yet" << endl;
        }
        for (auto itt : parent_questions_with_their_thread_questions[it.id_question]) {
            cout << "\tThread: Question ID (" << itt << ") from user ";
            cout << id_with_question[itt].first << "\t";
            cout << id_with_question[itt].second << endl;
            cout << "\tAnswer: ";
            if (answer_question[itt].size()) {
                cout << answer_question[itt] << endl;
            }
            else {
                cout << "Not answered yet" << endl;
            }
        }
    }
}

void Feed() {
    for (auto itt : users) {
        if (!Questions_to_me[itt.user_name].size()) {
            // There is no questions :("
            continue;
        }
        for (auto it : Questions_to_me[itt.user_name]) {
            cout << "Question ID (" << it.id_question << ") from user " << it.from;
            cout << "\t" << it.Question << endl;
            cout << "\tAnswer: ";
            if (answer_question[it.id_question].size()) {
                cout << answer_question[it.id_question] << endl;
            }
            else {
                cout << "Not answered yet" << endl;
            }
            for (auto itt : parent_questions_with_their_thread_questions[it.id_question]) {
                cout << "\tThread: Question ID (" << itt << ") from user ";
                cout << id_with_question[itt].first << "\t";
                cout << id_with_question[itt].second << endl;
                cout << "\tAnswer: ";
                if (answer_question[itt].size()) {
                    cout << answer_question[itt] << endl;
                }
                else {
                    cout << "Not answered yet" << endl;
                }
            }
        }
    }
}

void Print_question_from_me() {
    if (!Questions_from_me[username].size()) {
        cout << "There is no questions :(" << endl;
        return;
    }
    for (auto it : Questions_from_me[username]) {
        cout << "Question ID (" << it.id_question << ") to user " << it.to;
        cout << "\t" << it.Question << endl;
        cout << "\tAnswer: ";
        if (answer_question[it.id_question].size()) {
            cout << answer_question[it.id_question] << endl;
        }
        else {
            cout << "Not answered yet" << endl;
        }
    }
}

void Answer_question() {
    while (true) {
        cout << "Enter Question id or -1 to cancel: ";
        ll ans_question; cin >> ans_question;
        if (ans_question == -1) {
            break;
        }
        auto it = to_me[username].find(ans_question);
        if (it == to_me[username].end()) {
            cout << "There is no question with this id , try again\n";
            skip;
        }
        else {
            if (answer_question[ans_question].size()) {
                cout << "Warning: Already answered. Answer will be updated\n";
            }
            string answ;
            cin.ignore();
            cout << "Enter answer: "; getline(cin, answ);
            answer_question[ans_question] = answ;
            update_answers();
        }
        break;
    }
}

void Ask_question() {
    cout << "Enter Username or -1 to cancel: ";
    string ans; cin >> ans;
    if (ans == "-1") {
        return;
    }
    if (ans == username) {
        cout << "You can't ask yourself :(" << endl;
    }
    else {
        if (!found[ans]) {
            cout << "There is no user with this name :(" << endl;
        }
        else {
            cout << "For thread question: Enter Question id or -1 for new question: ";
            int type_question; cin >> type_question;
            if (type_question != -1) {
                if (!find_parent_question[type_question]) {
                    cout << "There is no parent question with this id :(" << endl;
                }
                else {
                    cout << "Enter question text: ";
                    cin.ignore();
                    string question; getline(cin, question);
                    Thread_questions data;
                    questions data2;
                    data.from = username;
                    data.to = ans;
                    data.id_thread_question = ++ID;
                    data.parent_question = type_question;
                    data.Question = question;
                    data2.from = username;
                    data2.to = ans;
                    data2.id_question = ID;
                    data2.Question = question;
                    Thread_questions_data.push_back(data);
                    find_thread_question[ID] = 1;
                    Questions_from_me[data.from].push_back(data2);
                    Questions_to_me[data.to].push_back(data2);
                    parent_questions_with_their_thread_questions[data.parent_question].insert(data.id_thread_question);
                    id_with_question[data.id_thread_question] = { data.from , data.Question };
                    //id_with_question[data.id_thread_question].second = ;
                    updata_thread_questions(data);
                }
            }
            else {
                cout << "Enter question text: ";
                string question;
                cin.ignore();
                getline(cin, question);
                questions data;
                data.from = username;
                data.to = ans;
                data.Question = question;
                data.id_question = ++ID;
                questions_data.push_back(data);
                find_parent_question[ID] = 1;
                Questions_from_me[data.from].push_back(data);
                Questions_to_me[data.to].push_back(data);
                id_with_question[data.id_question] = { data.from,data.Question };
                //id_with_question[data.id_question].second = ;
                update_questions_data(data);
            }
        }
    }
}

void List_System_Users() {
    for (auto it : users) {
        if (it.user_name == "") {
            skip;
        }
        cout << "Name :\t" << it.user_name << endl;
    }
}
void run() {
     //to load the last data that was stored in the file
    Load_User_Data();
    Load_Questions_data();
    Load_Thread_questions_data();
    Load_answers();
    while (true) {
        cout << "Menu:" << endl;
        cout << "\n\t1: Login" << endl;
        cout << "\t2: Sign Up" << endl;
        cout << "\t3: Exit" << endl;
        cout << "\nEnter number in range 1 - 3: ";
        int type; cin >> type;
        if (type == 1) {
            //cout << log_in() << endl;
            if (log_in()) {
                cout << "Welcome " << username << " :)" << endl;
                //break;
            }
            else {
                cout << "Wrong username or password :(" << endl;
                continue;
            }
        }
        else if (type == 2) {
            if (!sign_up()) {
                continue;
            }
        }
        else if (type == 3) {
            return;
        }
        else {
            cout << "Invalid input , please try again:(" << endl;
        }
        while (true) {
            cout << "Menu:" << endl;
            cout << "\t1: Print Question To Me" << endl;
            cout << "\t2: Print Question From Me" << endl;
            cout << "\t3: Answer Question" << endl;
            //cout << "\t4: Delet Question" << endl;
            cout << "\t4: Ask Question" << endl;
            cout << "\t5: List System Users" << endl;
            cout << "\t6: Feed" << endl;
            cout << "\t7: Logout" << endl;
            cout << "Enter number in range 1 - 7 : ";
            int type; cin >> type;
            if (type == 1) {
                Print_question_to_me();
            }
            else if (type == 2) {
                Print_question_from_me();
            }
            else if (type == 3) {
                Answer_question();
            }
            else if (type == 4) {
                Ask_question();
            }
            else if (type == 5) {
                List_System_Users();
            }
            else if (type == 6) {
                Feed();
            }
            else if (type == 7) {
                break;
            }
            else {
                cout << "Invalid range :(" << endl;
            }
        }
    }
    
}

int main() {
    Fast();
    run();
    
}