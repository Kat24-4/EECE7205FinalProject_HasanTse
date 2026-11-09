#pragma once 

#include <vector>
#include <string>

using namespace std;

class Course {
    public:
        string name;
        string professor;
        vector<string> students;
        string time;
        int room;

        Course(string n, string prof, vector<string> studs) {
            name = n;
            professor = prof;
            students = studs;
            time = "";
            room = 0; 
        }

        void AssignTime(string t) {
            time = t; 
        }

        void AssignRoom(int r) {
            room = r;
        }
};