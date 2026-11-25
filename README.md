# EECE 7205 Final Project - Multi-Course and Exam Scheduler
Nashita Hasan, Katherine Tse

## Project Description 
### Problem Statement
- **Problem**: Courses and exams need to be scheduled so that there are not two in the same room and there are not students or professors with overlapping events.
- **Users**: The University Administration Team would use it for general course planning and professors would use it for exam scheduling.
- **Importance**: There are multiple cases and reasons why this problem is important. Scheduling conflicts affect students ability to learn fully. For example, if a student has an exam at the same time as a class, they are forced to either miss the class or the exam which compromises their learning. Professors also cannot teach and hold exams at the same time. They would have to pick between either teaching or being present to answer questions during exams. Additionally, two classes or exams cannot be held in the same space. This would create a suboptimal testing or classroom environment that would affect students' ability to learn and a professor's ability to teach. 
- **Relevance**: One single change can create a rippling effect of conflicts within a given schedule which can be time consuming if handled manually. Having an automated process, implemented in C++, will assist the administration and instructor staff in solving scheduling conflicts promptly.

### Proposed Solution
- **Main Features**:
  - Professor overlap checking (i.e. one professor cannot teach/hold an exam at the same time)
  - Student overlap checking (i.e. one student cannot attend two classes/exams at the same time)
  -  Room scheduling conflict checking (i.e. one room cannot hold two classes/exams at the same time)
- **Expected Inputs**: A datafile containing Information on exams and courses that include the professor and student list
- **Constraints**: the number of available rooms and the time slot range for scheduling. 
- **Expected Outputs**: All courses and exams with their assigned room and time displayed neatly. 

## Current Progress
### Iteration 1
- Team and topic selection

### Iteration 2
- Initial documentation (see [Final_Project_Proposal_HasanTse.pdf](https://github.com/Kat24-4/EECE7205FinalProject_HasanTse/blob/main/docs/Final_Project_Proposal_HasanTse.pdf) for further details)
  - Project title
  - Problem statement
  - Proposed solution
  - Team and individual objectives
  - Initial program design (main features, data structures, program flow, testing plan)
  - GitHub set-up and plan
  - Tools and Programming Language decisions

### Iteration 3
- Working Prototype (see [Final_Project_Iteration3_HasanTse.pdf](https://github.com/Kat24-4/EECE7205FinalProject_HasanTse/blob/main/docs/Final_Project_Iteration3_HasanTse.pdf) for further details)
  - load input data
  - greedy algorithm
  - build graph
  - assigning rooms and times
  - save output data
  - menu for running tests

### Iteration 4
- Complete Application and Algorithm Evaluation (see [Final_Project_Iteration4_HasanTse.pdf]() for further details)
  - backtracking algorithm
  - instructions and capabilites for user uploaded data files
  - algorithm comparison and evaluation

## Program Strengths and Limitations
The strengths of our program is as follows: 
- [add strenths once final algorithm decided on]

At this time, we recognize the following limitations with our current implementation: 

- The program requires that professor and student names be typed exactly the same in order to find a match. This means that if there is an extra space between a first and a last name, or if capitalization is off, the program will not flag the overlap.
- The program does not optimize the time table. This means that the courses will be assigned to the earliest time possible which creates a morning heavy schedule.
- The program currently only runs with a CSV file input. Other file formats are not supported.
- Since the program uses the greedy algorithm there is a chance that the program can report not finding a solution when there is one.

## Build Instructions 
To build the project, run the following command in the terminal while in main folder of the project: `g++ src/*.cpp -o scheduler`

## Run Instructions 
To run the project, run the following command in the terminal after building and follow the prompts: `./scheduler`

## Sample datasets
Sample datasets can be found in the `/data` folder of the project. There you can find the following datasets and scenarios: 
- `boundary.csv` : a small (3 course) example where all of the provided courses overlap
- `empty.csv` : an empty csv file to test error handling 
- `failure1000.csv` : a large example where greedy does not find a solution and backtracking functionally runs forever
- `invalid.csv` : a csv file with the incorrect number of feilds to check error handling 
- `large.csv` : a large (1000 course) example that a schedule can be generated from, also used in the algorithm comparison experiment  
- `normal.csv` : a small (3 course) example with two overlapping classes  
- `TestCase_50.csv` : a 50 course example used for the algorithm comparison experiment 
- `TestCase_500.csv` : a 500 course example used for the algorithm comparison experiment

All of these datasets can be tried by selecting the corresponding item through the menu that pops up when you first run the application. 

## Algorithm Experiment 
### About the Experiment 
To test the differences between Greedy Coloring and Backtracking Coloring, execution time and memory consumption was measured for both algorithms on three different input sizes (50 courses, 500 courses, 1000 courses).

To see full details, see "Section 3: Experimental Evaluation" of the [Final_Project_Iteration4_HasanTse.pdf]() in the `/docs` folder.

### Experiment Results 
#### Time Comparison Results 

#### Memory Consumption Results 

#### Conclusion 

### How to Recreate the Results
To run the Algorithm Experiment, follow the build and run instructions and select `8` once prompted to select from the menu. 

You should expect an output like this: 

<img width="690" height="892" alt="experimentSample" src="https://github.com/user-attachments/assets/8b0df2f2-42a5-4b1a-93fd-aef62596c8cc" />

## Example Execution
### Small Sample Execution
Input:

<img width="1358" height="168" alt="normal" src="https://github.com/user-attachments/assets/784219bb-c868-4149-b205-0a4f55f07de1" />

Run:

<img width="607" height="321" alt="normalSample" src="https://github.com/user-attachments/assets/214af6f0-b552-4862-96bc-7077d8146d83" />

Output:

<img width="1456" height="137" alt="normalOutput" src="https://github.com/user-attachments/assets/16091098-71aa-4627-8519-ac96e58bf8a8" />

### Large Sample Execution 
Input: see "large.csv" in the `data` folder for large input data 

Run:

<img width="618" height="316" alt="largeSample" src="https://github.com/user-attachments/assets/49460b3b-55e2-4eca-93a3-41f8c002f8c2" />


Output: see "largeOutput.csv" in the `data` for the output

