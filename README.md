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
- **Expected Inputs**: A datafile containing Information on exams and courses that include the professor and student list; the number of available rooms and the time slot range for scheduling. 
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
- Working Prototype (see [Final_Project_Iteration3_HasanTse.pdf]() for further details)
  - load input data
  - greedy algorithm
  - build graph
  - assigning rooms and times
  - save output data
  - menu for running tests
 
## Program Limitations
At this time, we recognize the following limitations with our current implementation: 

- The program requires that professor and student names be typed exactly the same in order to find a match. This means that if there is an extra space between a first and a last name, or if capitalization is off, the program will not flag the overlap.
- The program does not optimize the time table. This means that the courses will be assigned to the earliest time possible which creates a morning heavy schedule.
- The program currently only runs with a CSV file input. Other file formats are not supported.
- Since the program uses the greedy algorithm there is a chance that the program can report not finding a solution when there is one.

## Build Instructions 
To build the project, run the following command in the terminal while in main folder of the project: `g++ src/*.cpp -o scheduler`

## Run Instructions 
To run the project, run the following command in the terminal after building and follow the prompts: `./scheduler`

## Example Execution
### Small Sample Execution
Input:

<img width="1358" height="168" alt="normal" src="https://github.com/user-attachments/assets/784219bb-c868-4149-b205-0a4f55f07de1" />

Run:

<img width="617" height="202" alt="sampleSession1" src="https://github.com/user-attachments/assets/cd5c34fd-42a8-4590-97b8-06c4125669a2" />

Output:

<img width="1456" height="137" alt="normalOutput" src="https://github.com/user-attachments/assets/16091098-71aa-4627-8519-ac96e58bf8a8" />

### Large Sample Execution 
Input: see "large.csv" in the `data` folder for large input data 

Run:

<img width="606" height="220" alt="sampleSession2" src="https://github.com/user-attachments/assets/603cbdc4-cac8-4384-a5d6-36b402a4c332" />


Output: see "largeOutput.csv" in the `data` for the output

