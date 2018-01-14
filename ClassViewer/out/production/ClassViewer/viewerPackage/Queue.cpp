class Queue
{
public:
	Queue(void);
	bool IsEmpty();
	bool IsFull();
	void EnQueue(int);
	int DeQueue();
	~Queue(void);
private:
	int arr[10];
	int size;
	int last;
	int first;
};
Queue::Queue(void)
{
	size = 10;
	last = 0;
	first = 0;
}
Queue::~Queue(void)
{
}
bool Queue::IsEmpty()
{
	if((last)%size==first)
		return true;
	else
		return false;
}
bool Queue::IsFull()
{
	if((last+1)%size==first)
		return true;
	else
		return false;
}
void Queue::EnQueue(int data)
{
	if(!Queue::IsFull()) {
		arr[last] = data;
		last = (last+1)%size;
	}
}
int Queue::DeQueue()
{
	if(!Queue::IsEmpty()) {
		return arr[first++];
		first = (first+1)%size;
	}
}