#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <iostream>

#include "course.h"
#include "graph.h"
#include "limitations.h"

using namespace std;

const vector<string> TIMES = {
    "8-8:50am", "9-9:50am", "10-10:50am", "11-11:50am",
    "12-12:50pm", "1-1:50pm", "2-2:50pm", "3-3:50pm",
    "4-4:50pm", "5-5:50pm", "6-6:50pm", "7-7:50pm",
};
const vector<int> ROOMS = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 
    31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 
    51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 
    61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 
    71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 
    81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 
    91, 92, 93, 94, 95, 96, 97, 98, 99, 100
};

vector<Course*> loadData(ifstream& file) {
    vector<Course*> courses; 
    string line;

    while (getline(file, line)) {
        vector<string> fields;
        string item = "";
        bool inQuotes = false;

        for (int i = 0; i < line.length(); i++) {
            char ch = line[i];

            if (ch == '"') {
                if (inQuotes && i + 1 < line.length() && line[i+1] == '"') {
                    item += '"';
                    i++;
                } else {
                    inQuotes = !inQuotes;
                }
            } else if (ch == ',' && !inQuotes) {
                fields.push_back(item);
                item = "";
            } else {
                item += ch;
            }
        }
        fields.push_back(item);

        if (fields.size() != 3) {
            cout << "Invalid Input File! Incorrect number of input fields." << endl; 
            return {}; 
        }

        string name = fields[0];
        string prof = fields[1]; 

        vector<string> students; 
        stringstream ss(fields[2]);
        string stud; 

        while(getline(ss, stud, ',')) {
            stringstream trimmer; 
            trimmer << stud; 
            string cleaned;
            trimmer >> ws; 
            getline(trimmer, cleaned);

            students.push_back(cleaned);
        }

        Course* newCourse = new Course(name, prof, students);
        courses.push_back(newCourse);
    }

    // move this to the main function 
    if (courses.empty()) {
        cout << "File is empty!" << endl;
    }

    return courses; 
}

Graph makeGraphFromCourses(const vector<Course*>& courses) {
    int numVertecies = courses.size();
    Graph eventGraph(numVertecies);

    for(int i = 0; i < numVertecies - 1; i++) {
        for(int j = i + 1; j < numVertecies; j++) {
            if (courses[i]->professor == courses[j]->professor) {
                eventGraph.AddEdge(i, j);
            } else {
                const vector<string>* check; 
                unordered_set<string> set; 
                if (courses[i]->students.size() > courses[j]->students.size()) {
                    check = &courses[i]->students;
                    set.insert(courses[j]->students.begin(), courses[j]->students.end());
                } else {
                    set.insert(courses[i]->students.begin(), courses[i]->students.end());
                    check = &courses[j]->students;
                }

                for (const auto& item: *check) {
                    if (set.count(item)) {
                        eventGraph.AddEdge(i, j);
                        break; 
                    }
                }
            }
        }
    }

    return eventGraph; 
}

void assignRoomsAndTimes(vector<Course*>& courses, vector<int> graphColors) {
    vector<int> nextOpenRoom(NUM_TIME_SLOTS, 0);
    
    if (courses.size() != graphColors.size()) {
        cout << "Error in Graph Coloring!" << endl;
        return; 
    }

    for (int i = 0; i < courses.size(); i++) {
        int courseColor = graphColors[i];
        if (courseColor >= NUM_TIME_SLOTS) {
            cout << "Not enough time slots for a conflict-free schedule!" << endl;
            return;
        }
        courses[i]->AssignTime(TIMES[courseColor]);

        if (nextOpenRoom[courseColor] >= TOTAL_ROOMS) {
            cout << "Not enough rooms in slot " << TIMES[courseColor] << "!" << endl;
            return;
        }
        int room = ROOMS[nextOpenRoom[courseColor]]; 
        courses[i]->AssignRoom(room);
        nextOpenRoom[courseColor]++;
    }
}

void writeFile(vector<Course*>& courses) {
    ofstream saveFile("schedule.csv");
     if (!saveFile.is_open()) {
            cout << "Could Not Open File." << endl;
            return;
    }

    for (const Course* c : courses) {
        saveFile << c->name << "," << c->professor << ",\"";

        for (int i = 0; i < c->students.size(); i++) {
            saveFile << c->students[i];

            if (i != c->students.size() - 1) {
                saveFile << ", ";
            }
        }

        saveFile << "\"," << c->time << "," << c->room << "\n";
    }

    saveFile.close();

    cout << "Your generated schedule can be found in \"schedule.csv\" file!" << endl; 
}

int main() {
    cout << "Welcome to Course and Exam Scheduler!" << endl; 
    cout << "Select a test case to generate a schedule from: " << endl;
    cout << "\t1: Normal Input" << endl; 
    cout << "\t2: Boundary Input" << endl; 
    cout << "\t3: Invalid Input" << endl; 
    cout << "\t4: Empty Input" << endl; 
    cout << "\t5: Large Input" << endl;

    int numCase; 
    string fileName;

    while (fileName.empty()) {
        if (!(cin >> numCase)) {           
            cin.clear();                   
            cin.ignore(10000, '\n');       
            numCase = 0;                   
        }

        switch (numCase) {
            case 1: fileName = "data/normal.csv";   break;
            case 2: fileName = "data/boundary.csv"; break;
            case 3: fileName = "data/invalid.csv";  break;
            case 4: fileName = "data/empty.csv";    break;
            case 5: fileName = "data/large.csv";    break;
            default:
                cout << "Invalid Selection. Please Select (1-5): " << endl;
        }
    }

    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Could Not Open Input File." << endl;
        return 1;
    }

    vector<Course*> courses = loadData(file);
    file.close();

    if (courses.empty()) {
        cout << "Error in Data Load. Exiting Program." << endl;
        return 1; 
    }

    Graph g = makeGraphFromCourses(courses);
    vector<int> colors = g.GreedyColor();

    if (colors.empty()) {
        cout << "Could not find a solution with the current number of rooms and timeslots available." << endl; 
        return 1; 
    }

    assignRoomsAndTimes(courses, colors);
    writeFile(courses);
    
    for (Course* c : courses) {
        delete c;
    }
    courses.clear();

    return 0;
}