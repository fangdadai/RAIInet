import <iostream>;
import <sstream>;
using namespace std;

int main(int argc, char *argv[]) {
  int sum = 0;
  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    int n;
    if (istringstream iss{arg}; iss >> n) sum += n;
  }
  cout << sum << endl;
}
