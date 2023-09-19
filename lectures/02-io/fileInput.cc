import <iostream>;
import <fstream>;
using namespace std;

int main () {
  ifstream file{"suite.txt"};
  string s;
  while (file >> s) {
    cout << s << endl;
  }
}
