#include <stdio.h>

#define MAX_STUDENTS 20
#define TIME_QUANTUM 2

/* Structure to represent a student process */
typedef struct {
    int studentId;
    int yearLevel;
    int burstTime;
    int remainingTime;
    int waitingTime;
} Student;


/*
 Function: roundRobinScheduler
 Purpose : Executes Round Robin scheduling for a given queue
 Input   : studentQueue[] - array of students in the queue
           count - number of students in the queue
 Output  : returns average waiting time
*/
float roundRobinScheduler(Student studentQueue[], int count) {

    int currentTime = 0;
    int completedProcesses = 0;
    float totalWaitingTime = 0;

    /* Continue until all processes are completed */
    while (completedProcesses < count) {

        for (int i = 0; i < count; i++) {

            if (studentQueue[i].remainingTime > 0) {

                if (studentQueue[i].remainingTime > TIME_QUANTUM) {

                    /* Execute for the time quantum */
                    currentTime += TIME_QUANTUM;
                    studentQueue[i].remainingTime -= TIME_QUANTUM;

                } else {

                    /* Process finishes execution */
                    currentTime += studentQueue[i].remainingTime;

                    studentQueue[i].waitingTime =
                        currentTime - studentQueue[i].burstTime;

                    totalWaitingTime += studentQueue[i].waitingTime;

                    studentQueue[i].remainingTime = 0;
                    completedProcesses++;
                }
            }
        }
    }

    /* Avoid division by zero */
    if (count == 0)
        return 0;

    return totalWaitingTime / count;
}


int main() {

    Student queueSenior[MAX_STUDENTS];
    Student queueJunior[MAX_STUDENTS];
    Student queueSophomore[MAX_STUDENTS];

    int totalStudents;
    int seniorCount = 0;
    int juniorCount = 0;
    int sophomoreCount = 0;

    printf("Enter number of students (max %d): ", MAX_STUDENTS);
    scanf("%d", &totalStudents);

    /* Error handling for invalid input */
    if (totalStudents <= 0 || totalStudents > MAX_STUDENTS) {
        printf("Error: Number of students must be between 1 and %d\n", MAX_STUDENTS);
        return 1;
    }

    /* Input student data */
    for (int i = 0; i < totalStudents; i++) {

        Student tempStudent;

        printf("\nStudent %d\n", i + 1);

        printf("Student ID: ");
        scanf("%d", &tempStudent.studentId);

        printf("Year (4=Senior, 3=Junior, 2=Sophomore): ");
        scanf("%d", &tempStudent.yearLevel);

        printf("Registration Time (1-10): ");
        scanf("%d", &tempStudent.burstTime);

        /* Validate input values */
        if (tempStudent.burstTime < 1 || tempStudent.burstTime > 10) {
            printf("Invalid registration time! Must be between 1 and 10.\n");
            i--;
            continue;
        }

        if (tempStudent.yearLevel != 4 &&
            tempStudent.yearLevel != 3 &&
            tempStudent.yearLevel != 2) {

            printf("Invalid year level! Enter 4, 3, or 2.\n");
            i--;
            continue;
        }

        /* Initialize remaining time */
        tempStudent.remainingTime = tempStudent.burstTime;
        tempStudent.waitingTime = 0;

        /* Assign student to appropriate queue */
        if (tempStudent.yearLevel == 4) {
            queueSenior[seniorCount++] = tempStudent;
        }
        else if (tempStudent.yearLevel == 3) {
            queueJunior[juniorCount++] = tempStudent;
        }
        else {
            queueSophomore[sophomoreCount++] = tempStudent;
        }
    }

    /* Run Round Robin scheduling for each queue */
    float avgSeniorWaiting =
        roundRobinScheduler(queueSenior, seniorCount);

    float avgJuniorWaiting =
        roundRobinScheduler(queueJunior, juniorCount);

    float avgSophomoreWaiting =
        roundRobinScheduler(queueSophomore, sophomoreCount);


    /* Display results */
    printf("\n===== Average Waiting Time =====\n");

    if (seniorCount > 0)
        printf("Queue 1 (Year 4 - Senior): %.2f\n", avgSeniorWaiting);

    if (juniorCount > 0)
        printf("Queue 2 (Year 3 - Junior): %.2f\n", avgJuniorWaiting);

    if (sophomoreCount > 0)
        printf("Queue 3 (Year 2 - Sophomore): %.2f\n", avgSophomoreWaiting);

    return 0;
}
