import <iostream>;
import <iomanip>;
using namespace std;

int main () {
  for (int i=0; i < 20; ++i) {
    cout << dec << setw(3) << i << oct << setw(3) << i << hex << setw(3) << i << endl;
  }
}
