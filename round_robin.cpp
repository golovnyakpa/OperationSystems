#include <iostream>
#include <queue>
#include <map>


using namespace std;
/**
 * Функция будет вызвана перед каждым тестом, если вы
 * используете глобальные и/или статические переменные
 * не полагайтесь на то, что они заполнены 0 - в них
 * могут храниться значения оставшиеся от предыдущих
 * тестов.
 *
 * timeslice - квант времени, который нужно использовать.
 * Поток смещается с CPU, если пока он занимал CPU функция
 * timer_tick была вызвана timeslice раз.
 **/
queue <int> thread_queue;
map <int,int> IO_threads;

int time_slice;

int global_timer;

 
void scheduler_setup(int timeslice)
{
    /* Put your code here */
    time_slice = timeslice;
    queue<int> empty;
    swap(thread_queue, empty);
    map<int,int> empty_;
    swap(IO_threads, empty_);
    global_timer = 0;
}

/**
 * Функция вызывается, когда создается новый поток управления.
 * thread_id - идентификатор этого потока и гарантируется, что
 * никакие два потока не могут иметь одинаковый идентификатор.
 **/
void new_thread(int thread_id)
{
    /* Put your code here */
    int new_thread;
    new_thread = thread_id;
    thread_queue.push(new_thread);
}

/**
 * Функция вызывается, когда поток, исполняющийся на CPU,
 * завершается. Завершится может только поток, который сейчас
 * исполняется, поэтому thread_id не передается. CPU должен
 * быть отдан другому потоку, если есть активный
 * (незаблокированный и незавершившийся) поток.
 **/
void exit_thread()
{
    /* Put your code here */
    if (thread_queue.size())
		thread_queue.pop();
	global_timer = 0;
}

/**
 * Функция вызывается, когда поток, исполняющийся на CPU,
 * блокируется. Заблокироваться может только поток, который
 * сейчас исполняется, поэтому thread_id не передается. CPU
 * должен быть отдан другому активному потоку, если таковой
 * имеется.
 **/
void block_thread()
{
    /* Put your code here */
    if (thread_queue.size()){
		IO_threads[thread_queue.front()] = thread_queue.front();
		thread_queue.pop();
	}
	global_timer = 0;
}

/**
 * Функция вызывается, когда один из заблокированных потоков
 * разблокируется. Гарантируется, что thread_id - идентификатор
 * ранее заблокированного потока.
 **/
void wake_thread(int thread_id)
{
    /* Put your code here */
    //thread temp_thread = IO_threads.find(thread_id);
    //IO_threads.erase(temp_thread);
    thread_queue.push(IO_threads[thread_id]);
    IO_threads.erase(thread_id);
}

/**
 * Ваш таймер. Вызывается каждый раз, когда проходит единица
 * времени.
 **/
void timer_tick()
{
    /* Put your code here */
    global_timer++;
    if (!(global_timer % time_slice)){
		int temp_thread = thread_queue.front();
		thread_queue.pop();
		thread_queue.push(temp_thread);
		global_timer = 0;
	}
}

/**
 * Функция должна возвращать идентификатор потока, который в
 * данный момент занимает CPU, или -1 если такого потока нет.
 * Единственная ситуация, когда функция может вернуть -1, это
 * когда нет ни одного активного потока (все созданные потоки
 * либо уже завершены, либо заблокированы).
 **/
int current_thread()
{
    /* Put your code here */
    if (thread_queue.size())
        return thread_queue.front(); //thread_queue.front();
    else
        return -1;
}


int main()
{
	scheduler_setup(10);
	new_thread(1);
	new_thread(2);
	new_thread(3);

	cout << "current thread: " << current_thread() << endl;

	for (int i = 0; i < 4; i++){
		timer_tick();
		cout << "current thread: " << current_thread() << endl;
	}
	int block = current_thread();
	cout << "block thread: " << block << endl;
	block_thread();
	cout << "current thread: " << current_thread() << endl;
	for (int i = 0; i < 4; i++){
		timer_tick();
		cout << "current thread: " << current_thread() << endl;
	}
	wake_thread(block);
	cout << "wake thread: " << block << endl;
	cout << "current thread: " << current_thread() << endl;
	for (int i = 0; i < 4; i++){
		timer_tick();
		cout << "current thread: " << current_thread() << endl;
	}
	cout << "exit thread: " << current_thread() << endl;
	exit_thread();
	cout << "current thread: " << current_thread() << endl;
	for (int i = 0; i < 4; i++){
		timer_tick();
		cout << "current thread: " << current_thread() << endl;
	}
	cout << "exit thread: " << current_thread() << endl;
	exit_thread();
	cout << "current thread: " << current_thread() << endl;
	for (int i = 0; i < 4; i++){
		timer_tick();
		cout << "current thread: " << current_thread() << endl;
	}
	cout << "exit thread: " << current_thread() << endl;
	exit_thread();
	cout << "current thread: " << current_thread() << endl;
	return 0;
}
