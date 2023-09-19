import <iostream>;
import <string>;
import <sstream>;
using namespace std;

int main () {
  int n;
  while (true) {
    cout << "Enter a number:" << endl;
    string s;
    cin >> s;
    if (istringstream {s} >> n) break;
    cout << "I said, ";
  }
  cout << "You entered " << n << endl;
}
