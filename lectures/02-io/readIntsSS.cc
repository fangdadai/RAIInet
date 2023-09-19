import <iostream>;
import <sstream>;
using namespace std;

int main () {
  string s;
  while (cin >> s) {    
    int n; 
    if (istringstream ss{s}; ss >> n) cout << n << endl;
  }
}
