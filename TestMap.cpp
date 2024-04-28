#include <map>
#include <iostream>
#include <string>

using namespace std;

int main() {
    map<int, string> myMap;
    myMap[0] = "one";
    myMap[2] = "two";
    myMap[3] = "three";
    myMap[4] = "four";
    myMap[5] = "five";

    map<int, string>::iterator it = myMap.begin();

    for (it = myMap.begin(); it != myMap.end(); ++it) {
        cout << it->first << " => " << it->second << endl;
        myMap.erase(it);
    }


}