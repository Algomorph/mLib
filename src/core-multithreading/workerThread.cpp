
/*void WorkerThreadEntry( LPVOID context )
{
	WorkerThread *thread = (WorkerThread *)context;
	thread->beginTaskLoop();
}*/

void WorkerThread::init(UINT threadIndex, ThreadLocalStorage *storage)
{
	m_threadIndex = threadIndex;
	m_storage = storage;
}

void WorkerThread::processTasks(TaskList<WorkerThreadTask*> &tasks)
{
	m_done = false;
	m_tasks = &tasks;
	m_thread = std::thread(workerThreadEntry, this);
}

void WorkerThread::workerThreadEntry( WorkerThread *context )
{
	context->enterThreadTaskLoop();
}

void WorkerThread::enterThreadTaskLoop()
{
	WorkerThreadTask* curTask;
	while(m_tasks->getNextTask(curTask))
	{
		curTask->run(m_threadIndex, m_storage);
		delete curTask;
	}
	m_done = true;
}
