#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <ctime>
#include <stdio.h>

using namespace std;

//Class for a generic DataBase with capacity 100
template<typename T>
class DataBase{
    public:
        static const int capacity = 100;
        T data[capacity];
};

//Common database for readers and writers
DataBase<int> database;

sem_t mutex;            //ensures mutual exclusion
sem_t write_read_sem;   //both reader and writer semaphore
int read_count = 0;     //number of processes reading
int total_loops;        //number of loops in thread functions

//Writer thread function to work with databases of <int> type
void* writer_int_transaction(void* param) {
	int index = *((int*)param);
    for(int j = 0; j < total_loops; j++) {
        // writer requests for critical section
        sem_trywait(&write_read_sem);  
    
        // performs the write
        int i = rand() % database.capacity;
        database.data[i] += 1;
        printf("Time: %d. Writer #%d -- wrote value %d at [%d] in database.\n", 
        clock(), index + 1, database.data[i], i);

        // leaves the critical section
        sem_post(&write_read_sem);
    }
    return nullptr;
}

//Reader thread function to work with databases of <int> type
void* reader_int_transaction(void* param) {
    int index = *((int*)param);
    for(int j = 0; j < total_loops; j++) {
        // Reader waiting
        sem_trywait(&mutex);

        // Increase number of readers
        read_count++;

        // there is atleast one reader in the critical section
        // this blocks all writers
        if (read_count == 1)     
            sem_trywait(&write_read_sem);

        // the critical section
        sem_post(&mutex);  
        int i = rand() % database.capacity;
        printf("Time: %d. Reader #%d -- read value %d at [%d] in database.\n", clock(), index + 1, database.data[i], i);
        sem_trywait(&mutex);   // a reader wants to leave

        read_count--;

        // no reader is left in the critical section
        if (read_count == 0) 
            sem_post(&write_read_sem);  // writers can enter

        sem_post(&mutex); // reader leaves
    }
    return nullptr;
}

//Gets integer input from user
int get_int_input(string message, int lowerBound, int upperBound){
	int input;
	cout << message;
	cin >> input;
	if (input < lowerBound || input > upperBound)
	{
		printf("Wrong input. It must be in this bounds: [%d, %d]\n", lowerBound, upperBound);
		return get_int_input(message, lowerBound, upperBound);
	}
	else
		return input;
}

int main()
{
    printf("Readers-Writers Problem!\nThread functions work on infinite loops, so abort the program in order to quit.\n");
	srand(time(NULL));
    //Initialize array
	for(int i = 0; i < database.capacity; i++) {
        database.data[i] = rand() % 100;
    }

    //Get user input for thread numbers
    int writers_number = get_int_input("Enter number of writer threads: ", 1, 5);
	int readers_number = get_int_input("Enter number of reader threads: ", 1, 5);
    total_loops = get_int_input("Enter number of loops in thread functions: ", 1, 500);

	//Initialize semaphores
	sem_init(&mutex, 0, 1);
	sem_init(&write_read_sem, 0, 1);

	//Create writer threads
	pthread_t* w_threads = new pthread_t[writers_number];
    int* writers = new int[writers_number];
	for (int i = 0; i < writers_number; ++i) {
        writers[i] = i;
		pthread_create(&w_threads[i], NULL, writer_int_transaction, (void*)(writers+i));
	}

	//Create reader threads
	pthread_t* r_threads = new pthread_t[readers_number];
    int* readers = new int[readers_number];
	for (int i = 0; i < readers_number; ++i) {
        readers[i] = i;
		pthread_create(&r_threads[i], NULL, reader_int_transaction, (void*)(readers+i));
	}

    //Start with writer thread as main
    int mNum = 0;
    reader_int_transaction((void*)&mNum);

    delete[] w_threads;
    delete[] r_threads;
    delete[] readers;
    delete[] writers;

    return 0;
}