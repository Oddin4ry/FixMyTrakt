class ITraktTask{
    public:
        virtual void run() = 0;
        virtual bool isRunning() = 0;
        virtual bool isStopped() = 0;
        virtual long long getTasksDone() = 0;
        virtual long long getTotalTasks() = 0;
};