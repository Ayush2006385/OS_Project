#include <stdio.h>

#define FRAMES 16
#define RECORDS 20
#define RECORDS_PER_PAGE 4

int main() {

    int frames[FRAMES];
    int time[FRAMES];
    int access[RECORDS];

    for(int i=0;i<FRAMES;i++){
        frames[i] = -1;
        time[i] = 0;
    }

    printf("Enter 20 student record accesses (1-20):\n");

    for(int i=0;i<RECORDS;i++)
        scanf("%d",&access[i]);

    int faults = 0;
    int clock = 0;

    printf("\nRecord\tPage\tStatus\n");

    for(int i=0;i<RECORDS;i++){

        int page = (access[i]-1)/RECORDS_PER_PAGE;

        int found = 0;

        for(int j=0;j<FRAMES;j++){
            if(frames[j] == page){
                found = 1;
                time[j] = clock++;
                break;
            }
        }

        if(!found){

            faults++;

            int lru = 0;

            for(int j=1;j<FRAMES;j++){
                if(time[j] < time[lru])
                    lru = j;
            }

            frames[lru] = page;
            time[lru] = clock++;
        }

        printf("%d\t%d\t",access[i],page);

        if(found)
            printf("Hit\n");
        else
            printf("Fault\n");
    }

    printf("\nTotal Page Faults = %d\n",faults);

    return 0;
}

