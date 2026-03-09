activity2.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Number of students
#define STUDENTS 5
#define COURSES 3

// Course seat limits
int seats[COURSES] = {10, 15, 8};

// Mutex lock for seat allocation
pthread_mutex_t seat_mutex;

// Semaphores for seat counting
sem_t course_sem[COURSES];

// Function executed by each student thread
void* bookSeat(void* student_id)
{
    int id = *(int*)student_id;

    // Each student randomly chooses a course
    int course = rand() % COURSES;

    printf("Student %d attempting to book Course %d\n", id, course + 1);

    // Wait on semaphore (decrease seat count)
    if (sem_wait(&course_sem[course]) != 0)
    {
        perror("Semaphore wait failed");
        pthread_exit(NULL);
    }

    // Lock mutex before modifying seat data
    pthread_mutex_lock(&seat_mutex);

    if (seats[course] > 0)
    {
        seats[course]--;
        printf("Student %d successfully booked Course %d. Remaining seats: %d\n",
               id, course + 1, seats[course]);
    }
    else
    {
        printf("Student %d failed to book Course %d (No seats left)\n",
               id, course + 1);
    }

    // Unlock mutex
    pthread_mutex_unlock(&seat_mutex);

    pthread_exit(NULL);
}

int main()
{
    pthread_t students[STUDENTS];
    int student_ids[STUDENTS];

    // Initialize mutex
    if (pthread_mutex_init(&seat_mutex, NULL) != 0)
    {
        perror("Mutex initialization failed");
        return 1;
    }

    // Initialize semaphores with seat limits
    for (int i = 0; i < COURSES; i++)
    {
        if (sem_init(&course_sem[i], 0, seats[i]) != 0)
        {
            perror("Semaphore initialization failed");
            return 1;
        }
    }

    // Create student threads
    for (int i = 0; i < STUDENTS; i++)
    {
        student_ids[i] = i + 1;

        if (pthread_create(&students[i], NULL, bookSeat, &student_ids[i]) != 0)
        {
            perror("Thread creation failed");
            return 1;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < STUDENTS; i++)
    {
        pthread_join(students[i], NULL);
    }

    // Destroy mutex and semaphores
    pthread_mutex_destroy(&seat_mutex);

    for (int i = 0; i < COURSES; i++)
    {
        sem_destroy(&course_sem[i]);
    }

    printf("\nFinal Remaining Seats:\n");
    for (int i = 0; i < COURSES; i++)
    {
        printf("Course %d: %d seats left\n", i + 1, seats[i]);
    }

    return 0;
}
