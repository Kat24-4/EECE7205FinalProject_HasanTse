#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <cstdlib>

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

struct MemTracker {
    size_t curr_allo = 0;
    size_t peak_allo = 0;
    size_t baseline = 0; 

    void reset() {
        baseline = curr_allo;
        peak_allo = curr_allo;
    }

    size_t peakSinceReset() const {return peak_allo - baseline;}
} tracker; 

struct alignas(max_align_t) AllocHeader {size_t size;};

void* operator new(size_t size) {
    void* raw = malloc(size + sizeof(AllocHeader));
    if (!raw) throw bad_alloc();
    static_cast<AllocHeader*>(raw)->size = size; 
    tracker.curr_allo += size; 
    tracker.peak_allo = max(tracker.peak_allo, tracker.curr_allo);
    return static_cast<AllocHeader*>(raw) + 1; 
}

void operator delete(void* ptr) noexcept {
    if (!ptr) return;
    AllocHeader* h = static_cast<AllocHeader*>(ptr) - 1;
    tracker.curr_allo -= h->size; 
    free(h);
}

void operator delete(void* ptr, size_t) noexcept { operator delete(ptr); }

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

void writeFile(vector<Course*>& courses, string fileName) {
    ofstream saveFile(fileName);
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

    cout << "Your generated schedule can be found in the \"" << fileName << "\" file!" << endl; 
}

int algoExperiment() {
    cout << "==Welcome to the Algorithm Comparison Experiement==" << endl; 
    
    // 5o Course Experiment
    cout << "50 Courses: " << endl; 
    ifstream file50("data/TestCase_50.csv");
    if (!file50.is_open()) {
        cout << "Could Not Open Test Case 50 File." << endl;
        return 1;
    } 
    vector<Course*> course50G = loadData(file50);
    file50.clear();
    file50.seekg(0);
    vector<Course*> course50B = loadData(file50); 
    file50.close();

    if (course50G.empty() || course50B.empty()) {
        cout << "Error in 50 Data Load. Exiting Program." << endl;
        return 1; 
    }

    Graph g50G = makeGraphFromCourses(course50G);
    Graph g50B = makeGraphFromCourses(course50B);

    tracker.reset();
    auto start = chrono::steady_clock::now();
    vector<int> colors50G = g50G.GreedyColor();
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start);
    cout << "\tTime of Greedy 50 Courses: " << duration.count() << " us" << endl;
    cout << "\tPeak Heap Memory of Greedy 50 Courses: " << tracker.peakSinceReset() << " bytes" << endl;

    if (!colors50G.empty()) {
        assignRoomsAndTimes(course50G, colors50G);
        writeFile(course50G, "greedy50.csv");
    } else {
        cout << "Valid Solution Not Found Using Greedy on 50 Courses" << endl;
    }
    
    tracker.reset();
    start = chrono::steady_clock::now();
    vector<int> colors50B = g50B.BacktrackingColor();
    end = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end-start);
    cout << "\tTime of Backtracking 50 Courses: " << duration.count() << " us" << endl;
    cout << "\tPeak Heap Memory of Backtracking 50 Courses: " << tracker.peakSinceReset() << " bytes" << endl;
    cout << "\tBacktracking Util Calls: " << g50B.lastCallCount << endl; 

    if (!colors50B.empty()) {
        assignRoomsAndTimes(course50B, colors50B);
        writeFile(course50B, "backtracking50.csv");
    } else {
        cout << "Valid Solution Not Found Using Backtracking on 50 Courses" << endl; 
    }

    for (auto* c: course50G) { delete c; }
    for (auto* c: course50B) { delete c; }
    course50G.clear();
    course50B.clear();

    // 500 Course Experiment 
    cout << endl << "500 Courses: " << endl; 
    ifstream file500("data/TestCase_500.csv");
    if (!file500.is_open()) {
        cout << "Could Not Open Test Case 500 File." << endl;
        return 1;
    } 
    vector<Course*> course500G = loadData(file500);
    file500.clear();
    file500.seekg(0);
    vector<Course*> course500B = loadData(file500); 
    file500.close();

    if (course500G.empty() || course500B.empty()) {
        cout << "Error in 500 Data Load. Exiting Program." << endl;
        return 1; 
    }

    Graph g500G = makeGraphFromCourses(course500G);
    Graph g500B = makeGraphFromCourses(course500B);

    tracker.reset();
    start = chrono::steady_clock::now();
    vector<int> colors500G = g500G.GreedyColor();
    end = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end-start);
    cout << "\tTime of Greedy 500 Courses: " << duration.count() << " us" << endl;
    cout << "\tPeak Heap Memory of Greedy 500 Courses: " << tracker.peakSinceReset() << " bytes" << endl;

    if (!colors500G.empty()) {
        assignRoomsAndTimes(course500G, colors500G);
        writeFile(course500G, "greedy500.csv");
    } else {
        cout << "Valid Solution Not Found Using Greedy on 500 Courses" << endl; 
    }

    tracker.reset();
    start = chrono::steady_clock::now();
    vector<int> colors500B = g500B.BacktrackingColor();
    end = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end-start);
    cout << "\tTime of Backtracking 500 Courses: " << duration.count() << " us" << endl;
    cout << "\tPeak Heap Memory of Backtracking 500 Courses: " << tracker.peakSinceReset() << " bytes" << endl;
    cout << "\tBacktracking Util Calls: " << g500B.lastCallCount << endl; 

    if (!colors500B.empty()) {
        assignRoomsAndTimes(course500B, colors500B);
        writeFile(course500B, "backtracking500.csv");
    } else {
        cout << "Valid Solution Not Found Using Backtracking on 500 Courses" << endl; 
    }

    for (auto* c: course500G) { delete c; }
    for (auto* c: course500B) { delete c; }
    course500G.clear();
    course500B.clear();

    // 1000 Course Experiement
    cout << endl << "1000 Courses: " << endl; 
    ifstream file1000("data/large.csv");
    if (!file1000.is_open()) {
        cout << "Could Not Open Test Case 1000 File." << endl;
        return 1;
    }
    vector<Course*> course1000G = loadData(file1000);
    file1000.clear();
    file1000.seekg(0);        
    vector<Course*> course1000B = loadData(file1000);
    file1000.close();
    
    if ( course1000G.empty() || course1000B.empty()) {
        cout << "Error in 1000 Data Load. Exiting Program." << endl;
        return 1; 
    }

    Graph g1000G = makeGraphFromCourses(course1000G);
    Graph g1000B = makeGraphFromCourses(course1000B);

    tracker.reset();
    start = chrono::steady_clock::now();
    vector<int> colors1000G = g1000G.GreedyColor();
    end = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end-start);
    cout << "\tTime of Greedy 1000 Courses: " << duration.count() << " us" << endl;
    cout << "\tPeak Heap Memory of Greedy 1000 Courses: " << tracker.peakSinceReset() << " bytes" << endl;

    if (!colors1000G.empty()) {
        assignRoomsAndTimes(course1000G, colors1000G);
        writeFile(course1000G, "greedy1000.csv");
    } else {
        cout << "Valid Solution Not Found Using Greedy on 1000 Courses" << endl; 
    }

    tracker.reset();
    start = chrono::steady_clock::now();
    vector<int> colors1000B = g1000B.BacktrackingColor();
    end = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end-start);
    cout << "\tTime of Backtracking 1000 Courses: " << duration.count() << " us" << endl;
    cout << "\tPeak Heap Memory of Backtracking 1000 Courses: " << tracker.peakSinceReset() << " bytes" << endl;
    cout << "\tBacktracking Util Calls: " << g1000B.lastCallCount << endl; 

    if (!colors1000B.empty()) {
        assignRoomsAndTimes(course1000B, colors1000B);
        writeFile(course1000B, "backtracking1000.csv");
    } else {
        cout << "Valid Solution Not Found Using Backtracking on 1000 Courses" << endl; 
    }
    
    for (auto* c: course1000G) { delete c; }
    for (auto* c: course1000B) { delete c; }
    course1000G.clear();
    course1000B.clear();

    return 0; 
}

int main() {
    cout << "Welcome to Course and Exam Scheduler!" << endl; 
    cout << "Select a scenario to generate a schedule from: " << endl;
    cout << "\t1: Small Normal Input" << endl; 
    cout << "\t2: Boundary Input" << endl; 
    cout << "\t3: Invalid Input" << endl; 
    cout << "\t4: Empty Input" << endl; 
    cout << "\t5: Large Input" << endl;
    cout << "\t6: Upload Your Own File" << endl; 
    cout << "\t7: Algorithm Comparison Experiement" << endl; 

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
            case 6: {
                string temp; 
                cout << "Create a CSV file or ensure that your current CSV file's lines have the following structure -" << endl;
                cout << "\t[name of class,name of professor,\"list of student names seperated by comma\"]" << endl; 
                cout << "\tExample -> (BB101 Intro to Biology,John Doe,\"Jane Doe, Bob Doe, Alice Doe\")" << endl;
                cout << "Then, drag and drop your CSV file into the \"data\" folder of this project." << endl; 
                cout << "Press Enter when complete." << endl; 
                cin.clear();
                cin.ignore(10000, '\n');  
                cin.get();
                cout << "Enter the name of the file (include the .csv at the end, \"example.csv\"): "; 
                cin >> temp; 
                fileName = "data/" + temp; 
                break; 
            }
            case 7: return algoExperiment();
            default:
                cout << "Invalid Selection. Please Select (1-6): " << endl;
        }
    }

    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Could Not Find or Open Input File." << endl;
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
        cout << "Could not find a solution with the current number of rooms and time slots available." << endl; 
        return 1; 
    }

    assignRoomsAndTimes(courses, colors);
    writeFile(courses, "schedule.csv");
    
    for (Course* c : courses) {
        delete c;
    }
    courses.clear();

    return 0;
}