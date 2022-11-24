#include <bits/stdc++.h>
using namespace std;

set<char> find_first(int m, vector<pair<char, string>> rules, set<char> terminals, vector<char> nonterminals_vector)
{
    char c = nonterminals_vector[m];

    // checking if we are going in a loop

    set<char> current;
    for (int i = 0; i < rules.size(); i++)
    {
        if (rules[i].first == c && rules[i].second.empty())
        {
            current.insert('^');
        }
        else if (rules[i].first == c && rules[i].second[0] != c)
        {
            if (terminals.find(rules[i].second[0]) != terminals.end())
            {
                current.insert(rules[i].second[0]);
            }
            else
            {
                int pos = find(nonterminals_vector.begin(), nonterminals_vector.end(), rules[i].second[0]) - nonterminals_vector.begin();
                set<char> temp = find_first(pos, rules, terminals, nonterminals_vector);
                int a = 0;
                while (a < rules[i].second.size() && temp.find('^') != temp.end())
                {
                    a++;
                    if (terminals.find(rules[i].second[0]) != terminals.end())
                    {
                        current.insert(rules[i].second[0]);
                        break;
                    }
                    if (a == rules[i].second.size())
                    {
                        current.insert('^');
                        break;
                    }
                    temp.erase('^');
                    current.insert(temp.begin(), temp.end());

                    pos = find(nonterminals_vector.begin(), nonterminals_vector.end(), rules[i].second[a]) - nonterminals_vector.begin();
                    temp = find_first(pos, rules, terminals, nonterminals_vector);
                }
                temp.erase('^');
                current.insert(temp.begin(), temp.end());
            }
        }
    }

    return current;
}

set<char> find_follow(int m, vector<pair<char, string>> &rules, vector<set<char>> first, set<char> terminals, vector<char> nonterminals_vector)
{
    char c = nonterminals_vector[m];
    set<char> current;
    if (c == 's')
    {
        current.insert('$');
    }

    for (int i = 0; i < rules.size(); i++)
    {
        for (int j = 0; j < rules[i].second.size(); j++)
        {

            if (rules[i].second[j] == c)
            {
                if (j == rules[i].second.size() - 1)
                {
                    if (rules[i].first != c)
                    {
                        int pos = find(nonterminals_vector.begin(), nonterminals_vector.end(), rules[i].first) - nonterminals_vector.begin();
                        set<char> temp = find_follow(pos, rules, first, terminals, nonterminals_vector);
                        current.insert(temp.begin(), temp.end());
                    }
                }
                else if (terminals.find(rules[i].second[j + 1]) != terminals.end())
                {
                    current.insert(rules[i].second[j + 1]);
                }
                else
                {
                    int pos = find(nonterminals_vector.begin(), nonterminals_vector.end(), rules[i].second[j + 1]) - nonterminals_vector.begin();
                    set<char> temp = first[pos];
                    temp.erase('^');
                    current.insert(temp.begin(), temp.end());

                    int a = j + 1;
                    while (a < rules[i].second.size() && first[pos].find('^') != first[pos].end())
                    {

                        a++;
                        if (terminals.find(rules[i].second[a]) != terminals.end())
                        {
                            current.insert(rules[i].second[a]);
                            break;
                        }

                        if (a == rules[i].second.size())
                        {
                            if (rules[i].first != c)
                            {
                                int pos = find(nonterminals_vector.begin(), nonterminals_vector.end(), rules[i].first) - nonterminals_vector.begin();
                                temp = find_follow(pos, rules, first, terminals, nonterminals_vector);
                                current.insert(temp.begin(), temp.end());
                            }
                            break;
                        }

                        pos = find(nonterminals_vector.begin(), nonterminals_vector.end(), rules[i].second[a]) - nonterminals_vector.begin();
                        temp = first[pos];
                        temp.erase('^');
                        current.insert(temp.begin(), temp.end());
                    }
                    temp = first[pos];
                    temp.erase('^');
                    current.insert(temp.begin(), temp.end());
                }
            }
        }
    }
    return current;
}

int main()
{
    int n;
    cout << "Enter The number of rules in the grammar: ";
    cin >> n;
    vector<pair<char, string>> rules;
    string x;
    cout << "Enter the start symbol" << endl;
    cin >> x;

    rules.push_back(make_pair('s', x));

    set<char> terminals;
    set<char> nonterminals;
    nonterminals.insert('s');

    cout << "Enter the rules (in the form s->E): " << endl;
    for (int i = 1; i <= n; i++)
    {
        cout << "Rule " << i << ":   ";

        string r;
        cin >> r;
        rules.push_back(make_pair(r[0], r.substr(3)));
        nonterminals.insert(r[0]);
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j < rules[i].second.length(); j++)
        {
            if (nonterminals.find(rules[i].second[j]) == nonterminals.end())
            {
                terminals.insert(rules[i].second[j]);
            }
        }
    }

    queue<pair<int, char>> buffer;
    vector<vector<pair<char, string>>> collection;
    vector<pair<char, string>> I0;

    // will store Goto(I, X) for all I and X ans the resultant
    // goto(connection[0] , connection[1]) = connection[2]
    vector<pair<pair<int, char>, int>> connections;

    // making the first collection
    for (int i = 0; i <= n; i++)
    {
        I0.push_back(make_pair(rules[i].first, "." + rules[i].second));
        if (!rules[i].second.empty())
        {
            buffer.push(make_pair(0, rules[i].second[0]));
        }
    }

    collection.push_back(I0);

    while (!buffer.empty())
    {

        pair<int, char> p = buffer.front();
        buffer.pop();
        if (p == buffer.front())
        {
            buffer.pop();
        }

        queue<char> closure;

        // will store the new collection
        vector<pair<char, string>> temp;
        for (int i = 0; i < collection[p.first].size(); i++)
        {
            // finding the dot
            int pos = collection[p.first][i].second.find('.');
            string s = collection[p.first][i].second;

            // if the dot is not at the end of the string and the next character is X
            if (pos + 1 < s.size() && s[pos + 1] == p.second)
            {
                string s = collection[p.first][i].second;
                s[pos] = s[pos + 1];
                s[pos + 1] = '.';
                temp.push_back(make_pair(collection[p.first][i].first, s));
                if (pos + 2 < s.size() && nonterminals.find(s[pos + 2]) != nonterminals.end())
                {
                    closure.push(s[pos + 2]);
                }
            }
        }

        while (!closure.empty())
        {
            char c = closure.front();
            closure.pop();
            for (int i = 0; i <= n; i++)
            {
                if (rules[i].first == c)
                {
                    temp.push_back(make_pair(rules[i].first, "." + rules[i].second));
                    if (rules[i].second[0] != c && nonterminals.find(rules[i].second[0]) != nonterminals.end())
                    {
                        closure.push(rules[i].second[0]);
                    }
                }
            }
        }

        // checking if the new collection is already present
        int flag = 0;
        for (int i = 0; i < collection.size(); i++)
        {
            vector<pair<char, string>> a = temp;
            vector<pair<char, string>> b = collection[i];
            // comparing with previous collections
            sort(a.begin(), a.end());
            sort(b.begin(), b.end());
            if (a == b)
            {
                flag = 1;
                connections.push_back(make_pair(p, i));
                break;
            }
        }

        // if the new collection is not present
        if (flag == 0)
        {
            collection.push_back(temp);
            connections.push_back(make_pair(p, collection.size() - 1));
            for (int i = 0; i < temp.size(); i++)
            {
                int pos = temp[i].second.find('.');

                // if the dot is not at the end of the string and the next character is X
                if (pos + 1 < temp[i].second.size())
                {
                    buffer.push(make_pair(collection.size() - 1, temp[i].second[pos + 1]));
                }
            }
        }
    }

    terminals.insert('$');
    // printing the terminals

    // converting sets to vector
    vector<char> terminals_vector(terminals.begin(), terminals.end());
    vector<char> nonterminals_vector(nonterminals.begin(), nonterminals.end());

    // making action table
    int states = collection.size();
    vector<vector<int>> action(states, vector<int>(terminals.size(), 0));

    // making goto table
    vector<vector<int>> goto_table(states, vector<int>(nonterminals.size(), 0));

    // the shift operation would be shown as positive numbers
    // the reduce operation would be shown as negative numbers

    for (int i = 0; i < connections.size(); i++)
    {
        int current = connections[i].first.first;
        char middle = connections[i].first.second;
        int next = connections[i].second;

        // if the middle is a non-ternimal it would be a goto operation
        if (nonterminals.find(middle) != nonterminals.end())
        {
            for (int a = 0; a < nonterminals_vector.size(); a++)
            {
                if (nonterminals_vector[a] == middle)
                {
                    goto_table[current][a] = next;
                    break;
                }
            }
        }
        else
        {
            // if the middle is a ternimal it would be a shift operation
            for (int a = 0; a < terminals_vector.size(); a++)
            {
                if (terminals_vector[a] == middle)
                {
                    action[current][a] = next;
                    break;
                }
            }
        }
    }

    // finding the first and follow of the non-ternimals
    vector<set<char>> first(nonterminals.size(), set<char>());
    vector<set<char>> follow(nonterminals.size(), set<char>());

    // finding the first of the non-ternimals
    for (int i = 0; i < nonterminals.size(); i++)
    {
        first[i] = find_first(i, rules, terminals, nonterminals_vector);
    }

    // printing the first of the non-ternimals
    cout << "The first of the non-ternimals are: " << endl;
    for (int i = 0; i < nonterminals.size(); i++)
    {
        cout << nonterminals_vector[i] << ": ";
        for (auto it = first[i].begin(); it != first[i].end(); it++)
        {
            cout << *it << " ";
        }
        cout << endl;
    }

    // finding the follow of non terminals
    follow[0].insert('$');

    for (int i = 0; i < nonterminals.size(); i++)
    {
        follow[i] = find_follow(i, rules, first, terminals, nonterminals_vector);
    }

    // printing the follow of the non-ternimals
    cout << "The follow of the non-ternimals are: " << endl;
    for (int i = 0; i < nonterminals.size(); i++)
    {
        cout << nonterminals_vector[i] << ": ";
        for (auto it = follow[i].begin(); it != follow[i].end(); it++)
        {
            cout << *it << " ";
        }
        cout << endl;
    }

    int acceptance_state;

    // making the reduce operations
    for (int i = 0; i < collection.size(); i++)
    {
        for (int j = 0; j < collection[i].size(); j++)
        {
            int n = collection[i][j].second.size();
            if (collection[i][j].second[n - 1] == '.')
            {
                if (collection[i][j].first == 's')
                {
                    acceptance_state = i;
                }
                else
                {
                    int pos = find(nonterminals_vector.begin(), nonterminals_vector.end(), collection[i][j].first) - nonterminals_vector.begin();
                    for (auto it = follow[pos].begin(); it != follow[pos].end(); it++)
                    {
                        string rulzzz = collection[i][j].second;
                        rulzzz.pop_back();
                        int pos2 = find(terminals_vector.begin(), terminals_vector.end(), *it) - terminals_vector.begin();
                        int red;
                        for (int h = 0; h < rules.size(); h++)
                        {
                            if (rules[h].first == collection[i][j].first && rules[h].second == rulzzz)
                            {
                                red = h;
                                break;
                            }
                        }
                        if (action[i][pos2] == 0)
                        {
                            action[i][pos2] = -red;
                        }
                        else
                        {
                            cout << "Grammar is no LR(1)" << endl;
                            return 0;
                        }
                    }
                }
            }
        }
    }

    // printing the action table
    cout << "The action table is: " << endl;
    cout << " ";
    for (int i = 0; i < terminals_vector.size(); i++)
    {
        cout << "   " << terminals_vector[i];
    }
    cout << endl;
    for (int i = 0; i < action.size(); i++)
    {
        cout << i;
        for (int j = 0; j < action[i].size(); j++)
        {
            cout << "   " << action[i][j];
        }
        cout << endl;
    }

    // printing the goto table
    cout << "The goto table is: " << endl;
    cout << " ";
    for (int i = 0; i < nonterminals_vector.size(); i++)
    {
        cout << "   " << nonterminals_vector[i];
    }
    cout << endl;
    for (int i = 0; i < goto_table.size(); i++)
    {
        cout << i;
        for (int j = 0; j < goto_table[i].size(); j++)
        {
            cout << "   " << goto_table[i][j];
        }
        cout << endl;
    }

    // making the parser
    cout << "Enter the string" << endl;
    string s;
    cin >> s;
    s += '$';
    stack<char> st;
    st.push('0');
    while (true)
    {

        int current = st.top() - '0';

        int pos2 = find(terminals_vector.begin(), terminals_vector.end(), s[0]) - terminals_vector.begin();
        int act = action[current][pos2];

        // printing stack
        cout << "Stack: ";
        stack<char> temp = st;
        while (!temp.empty())
        {
            cout << temp.top();
            temp.pop();
        }
        cout << " action: ";

        if (current == acceptance_state && s[0] == '$')
        {
            cout << "acceptance" << endl;
            break;
        }
        if (act > 0)
        {

            cout << "s" << act;
            st.push(s[0]);
            st.push(act + '0');
            s = s.substr(1);
        }
        else if (act < 0)
        {

            // reduce
            cout << "r" << -act;
            int rul = -act;
            int n = rules[rul].second.size();
            string ru;
            for (int i = 0; i < 2 * n; i++)
            {
                if (i % 2 != 0)
                {
                    ru += st.top();
                }
                st.pop();
            }
            reverse(ru.begin(), ru.end());
            if (rules[rul].second != ru)
            {
                cout << "String is not accepted" << endl;
                return 0;
            }

            int current = st.top() - '0';
            char nt = rules[rul].first;
            int top_state = st.top();
            st.push(nt);
            int px = find(nonterminals_vector.begin(), nonterminals_vector.end(), nt) - nonterminals_vector.begin();

            st.push(goto_table[top_state - '0'][px] + '0');
            stack<char> temp = st;
        }
        else
        {

            cout << "String is not accepted" << endl;
            return 0;
        }
        cout << endl;
    }

    cout << "String is accepted" << endl;
    return 0;
}
