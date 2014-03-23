// 유한 결정 오토마타 생성기 (Deterministic Finite Automata Generator)
// 박연오 (bakyeono@gmail.com)

#include <iostream>
#include <vector>
using namespace std;

// alphabet_array 클래스
// 이 클래스는 오토마타에 적용할 사용자 정의 알파벳 목록을 관리합니다.
// <주요 함수>
// 알파벳 목록의 출력: print_table, print_tabs, print_line
// 목록 초기화: reset
// 목록 참조: aton(알파벳 -> 숫자), ntoa(숫자 -> 알파벳), size(목록 크기)
class alphabet_array {
private:
  vector<char> coll;

public:
  void reset(string new_alphabets) {
    coll.clear();
    for (int i = 0; i < new_alphabets.size(); ++i) {
      coll.push_back(new_alphabets[i]);
    }
  }
  void print_table(void) {
    cout << "[ALPHABET TABLE]" << endl;
    cout << "================" << endl;
    cout << "index\tcode" << endl;
    cout << "----------------" << endl;
    for (int i = 0; i < coll.size(); ++i) {
      cout << i << "\t'" << coll[i] << "'" << endl;
    }
    cout << "================" << endl;
  }
  void print_tabs(void) {
    for (int i = 0; i < coll.size(); ++i) {
      cout << "'" << coll[i] << "'\t";
    }
  }
  void print_line(void) {
    for (int i = 0; i < coll.size(); ++i) {
      cout << coll[i];
    }
  }
  int aton(char alphabet) {
    for (int i = 0; i < coll.size(); ++i) {
      if (coll[i] == alphabet) return i;
    }
    cerr << "wrong alphabet input : " << alphabet << endl;
    return -1;
  }
  char ntoa(int number) {
    if (!(0 <= number && number < coll.size())) {
      cerr << "wrong alphabet search : " << number << endl;
      return -1;
    }
    return coll[number];
  }
  int size(void) {
    return coll.size();
  }
};

// state 구조체
// 이 구조체는 오토마타에 적용되는 각 상태를 표현합니다.
// is_acceptable은 상태의 accept 여부를 나타냅니다.
// state_by_alphabet 배열은 사용자 정의 알파벳의 개수만큼 동적 할당되어,
//                   각 알파벳에 따라 전환될 상태를 저장합니다.
// 이 구조체는 아래에서 정의할 state_array 클래스를 통해 관리됩니다.
struct state {
public:
  bool is_acceptable;
  vector<int> state_by_alphabet;
  state() : is_acceptable(false) {}
  void reset(int n, int def) {
    state_by_alphabet.clear();
    for(int i = 0; i < n; ++i) {
      state_by_alphabet.push_back(def);
    }
  }
};

// state_array 클래스
// 이 클래스는 오토마타에 적용할 각 상태(state 구조체)들을 묶어서 관리합니다.
// <주요 함수>
// 상태 목록의 출력: print_table
// 목록 초기화: reset
// 목록 참조 및 수정: set_start_state, get_start_state, create_state_by_alphabet_tables,
//                    set_acceptable, is_acceptable, set_next_state, get_next_state, size
// 내부 함수: is_valid_idx, print_out_of_state_idx_error, 
class state_array {
private:
  vector<state> coll;
  int start_state_idx;

public:
  void reset(int size) {
    coll.clear();
    for (int i = 0; i < size; ++i) {
      coll.push_back(state());
    }
  }
  bool is_valid_idx(int state_idx) {
    if (0 <= state_idx && state_idx < coll.size()) return true;
    else return false;
  }
  void print_out_of_state_idx_error(int state_idx) {
    cerr << "out of state array : " << state_idx << endl;
  }
  void set_start_state(int state_idx) {
    if (!is_valid_idx(state_idx)) {
      print_out_of_state_idx_error(state_idx);
      return;
    }
    start_state_idx = state_idx;
  }
  int get_start_state(void) {
    return start_state_idx;
  }
  void create_state_by_alphabet_tables(alphabet_array* user_alphabets) {
    for (int i = 0; i < coll.size(); ++i) {
      coll[i].reset(user_alphabets->size(), start_state_idx);
    }
  }
  void set_acceptable(int state_idx, bool val) {
    if (!is_valid_idx(state_idx)) {
      print_out_of_state_idx_error(state_idx);
      return;
    }
    coll[state_idx].is_acceptable = val;
  }
  bool is_acceptable(int state_idx) {
    if (!is_valid_idx(state_idx)) {
      print_out_of_state_idx_error(state_idx);
      return false;
    }
    return coll[state_idx].is_acceptable;
  }
  int set_next_state(int state_idx, int alphabet_idx, int val) {
    coll[state_idx].state_by_alphabet[alphabet_idx] = val;
  }
  int get_next_state(int state_idx, char alphabet) {
    if (!is_valid_idx(state_idx)) {
      print_out_of_state_idx_error(state_idx);
      return 0;
    }
    return coll[state_idx].state_by_alphabet[alphabet];
  }
  int size(void) {
    return coll.size();
  }
  void print_table(alphabet_array* user_alphabets) {
    cout << "[STATE-TRANSITION TABLE]" << endl;
    for (int i = -3; i < user_alphabets->size(); ++i) cout << "========"; cout << endl;
    cout << "index\taccept?\t\t"; user_alphabets->print_tabs(); cout << endl;
    for (int i = -3; i < user_alphabets->size(); ++i) cout << "--------"; cout << endl;
    for (int i = 0; i < coll.size(); ++i) {
      cout << i << "\t";
      if (coll[i].is_acceptable) cout << "YES";
      else cout << "NO";
      cout << "\t\t";
      for (int j = 0; j < user_alphabets->size(); ++j) {
	cout << coll[i].state_by_alphabet[j] << "\t";
      }
      cout << endl;
    }
    cout << "* start state : " << start_state_idx << endl;
    for (int i = -3; i < user_alphabets->size(); ++i) cout << "========"; cout << endl;
  }
};

// press_enter 함수
// 출력을 보여주기 위해 키 입력을 대기함.
void press_enter(void) {
  cout << "--- Press [enter] to continue ---" << endl;
  cin.ignore();
  cin.get();
}

// welcome 함수
// 프로그램 이름을 출력함.
void welcome(void) {
  cout << "=============================" << endl;
  cout << "Deterministic Finite Automata" << endl;
  cout << "=============================" << endl;
}

// input_number_of_states 함수
// 오토마타가 유지할 상태의 종류가 몇 가지인지 입력받음.
void input_number_of_states(state_array* user_states) {
  int number_of_states = 0;
  while (1) {
    cout << "1. How many states do you need?\n: ";
    cin >> number_of_states;
    if (number_of_states <= 0) {
      cout << "- Must be more than 0." << endl;
      continue;
    } else {
      cout << "- Okay." << endl << endl;
      break;
    }
  }
  user_states->reset(number_of_states);
}

// input_start_state 함수
// 오토마타의 초기 상태의 번호를 입력받음.
void input_start_state(state_array* user_states) {
  int start_state = 0;
  while (1) {
    cout << "2. From where does the state start?(0-" << user_states->size()-1 << ")\n: ";
    cin >> start_state;
    if (0 <= start_state && start_state < user_states->size()) {
      cout << "- Okay." << endl << endl;
      break;
    }
  }
  user_states->set_start_state(start_state);
}

// input_alphabets_to_use 함수
// 오토마타에서 사용할 알파벳의 종류를 입력받음.
void input_alphabets_to_use(state_array* user_states, alphabet_array* user_alphabets) {
  string temp_alphabets;
  cout << "3. Type the alphabets to use, without any blank character. (ex : abcdefg)\n: ";
  cin >> temp_alphabets;
  user_alphabets->reset(temp_alphabets);
  user_states->create_state_by_alphabet_tables(user_alphabets);
  user_alphabets->print_table();
  cout << "- Alphabet table created." << endl << endl;
  press_enter();
}

// modify_state 함수
// input_state_settings 및 modify_state_settings 함수에서 사용되는 개별 수정 함수.
void modify_state(state_array* user_states, alphabet_array* user_alphabets, int idx) {
  char yn;
  int val;
  while (1) {
    cout << "(state " << idx << ") is accept state? (y/n) ";
    cin >> yn;
    if (yn == 'y' || yn == 'Y') {
      user_states->set_acceptable(idx, true);
      break;
    } else if (yn == 'n' || yn == 'N') {
      user_states->set_acceptable(idx, false);
      break;
    } else {
      cout << "- 'Y'es or 'N'o only." << endl;
    }
  }
  for (int i = 0; i < user_alphabets->size(); ++i) {
    while (1) {
      cout << "(state " << idx << ") transition of '" << user_alphabets->ntoa(i) << "' -> ";
      cin >> val;
      if (0 <= val && val < user_states->size()) {
	user_states->set_next_state(idx, i, val);
	break;
      } else {
	cout << "- Wrong value. (0-" << user_states->size()-1 << "), not " << val << "." << endl;
      }
    }
  }
}

// modify_state_settings 함수
// 오토마타의 각 상태의 accpet 여부와 알파벳에 따른 전환을 수정함.
void modify_state_settings(state_array* user_states, alphabet_array* user_alphabets) {
  int to_edit = 0;
  cout << "Enter the state index number to edit: ";
  cin >> to_edit;
  if (0 <= to_edit && to_edit < user_states->size()) {
    modify_state(user_states, user_alphabets, to_edit);
  } else {
    cout << "- Wrong index. (0-" << user_states->size()-1 << "), not " << to_edit << "." << endl;
  }
}

// input_state_settings 함수
// 오토마타의 각 상태의 accpet 여부와 알파벳에 따른 전환을 차례로 입력받음.
void input_state_settings(state_array* user_states, alphabet_array* user_alphabets) {
  cout << "4. Update each state's information." << endl;
  for (int i = 0; i < user_states->size(); ++i) {
    user_states->print_table(user_alphabets);
    cout << "--- state " << i << " ---" << endl;
    modify_state(user_states, user_alphabets, i);
    cout << endl;
  }
  cout << "- You've finished your automata!" << endl << endl;
  press_enter();
}

// apply_automata 함수
// 사용자가 정의한 오토마타에 문자열을 적용해 결과를 출력함.
void apply_automata(state_array* user_states, alphabet_array* user_alphabets) {
  string str;
  int current_state;
  int current_alphabet;
  bool is_wrong_str;
  char yn;
  cout << "Enter a string only with ("; user_alphabets->print_line(); cout << ")\n: ";
  cin >> str;
  is_wrong_str = false;
  current_state = user_states->get_start_state();
  for (int i = 0; i < str.size(); ++i) {
    current_alphabet = user_alphabets->aton(str[i]);
    if (current_alphabet == -1) {
      is_wrong_str = true;
      break;
    }
    current_state = user_states->get_next_state(current_state, current_alphabet);
  }
  if (is_wrong_str) {
    cout << "- It was a wrong string!" << endl << endl;
  } else {
    cout << "* Result : ";
    if (user_states->is_acceptable(current_state)) {
      cout << "YES";
    } else {
      cout << "NO";
    }
    cout << endl;
    cout << "* Last state : " << current_state << endl << endl;
  }
  press_enter();
}

// run_automata 함수
// 완성된 오토마타를 어떻게 사용할지 물어보고, 오토마타를 수정하거나 문자열을 적용함.
void run_automata(state_array* user_states, alphabet_array* user_alphabets) {
  char menu_choice;
  user_states->print_table(user_alphabets);
  while (1) {
    cout << "[T] Test a string on current automata" << endl;
    cout << "[M] Modify automata's state options" << endl;
    cout << "[Q] Quit" << endl;
    cout << "Choose an item: ";
    cin >> menu_choice;
    if (menu_choice == 'm' || menu_choice == 'M') {
      modify_state_settings(user_states, user_alphabets);
      user_states->print_table(user_alphabets);
    } else if (menu_choice == 't' || menu_choice == 'T') {
      apply_automata(user_states, user_alphabets);
      user_states->print_table(user_alphabets);
    } else if (menu_choice == 'q' || menu_choice == 'Q') {
      break;
    } else {
      cout << "- wrong option selected. (t, m, q), not " << menu_choice << "." << endl;
    }
  }
}

// main 함수
int main(int argc, char** argv)
{
  state_array*    user_states    = new state_array;
  alphabet_array* user_alphabets = new alphabet_array;
  welcome();
  input_number_of_states(user_states);
  input_start_state(user_states);
  input_alphabets_to_use(user_states, user_alphabets);
  input_state_settings(user_states, user_alphabets);
  run_automata(user_states, user_alphabets);
  cout << "Goodbye!" << endl;
  return 0;
}
