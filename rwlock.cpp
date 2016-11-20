#include <cassert>
#include <iostream>
#include <thread>
#include <mutex>

// Given a mutex, implement a reader-writer lock.
// Assertions in the acquire / release functions validate correctness.
class rwlock
{
public:
    rwlock() 
        : readCount(0), 
          writeCount(0) 
    {
    }

    void acquireRead()
    {   
        internal.lock();
        if (readCount == 0)
        {   
            content.lock();
        }
        ++readCount;
        assert(readCount > 0);
        assert(writeCount == 0);
        std::cout << "readCount = " << readCount << "\n";
        internal.unlock();
    }
    
    void acquireWrite()
    {   
        content.lock();
        ++writeCount;
        assert(writeCount == 1);
        assert(readCount == 0);
        std::cout << "writeCount = " << writeCount << "\n";
    }
    
    void releaseRead()
    {   
        internal.lock();
        --readCount;
        assert(readCount >= 0);
        assert(writeCount == 0);
        std::cout << "readCount = " << readCount << "\n";
        if (readCount == 0)
        {   
            content.unlock();
        }
        internal.unlock();
    }

    void releaseWrite()
    {   
        --writeCount;
        assert(writeCount == 0);
        assert(readCount == 0);
        std::cout << "writeCount = " << writeCount << "\n";
        content.unlock();
    }
    
private:
    std::mutex content;
    std::mutex internal;
    int readCount;
    int writeCount;
};    

rwlock rwl;

void reader()
{
    for (size_t i = 0; i < 10; ++i)
    {
        rwl.acquireRead();
        rwl.releaseRead();
    }
}

void writer()
{
    for (size_t i = 0; i < 10; ++i)
    {
        rwl.acquireWrite();
        rwl.releaseWrite();
    }
}
int main()
{
    // Create multiple reader and writer threads, asserts validate correctness.
    std::thread r1(reader);
    std::thread r2(reader);
    std::thread r3(reader);
    std::thread r4(reader);

    std::thread w1(writer);
    std::thread w2(writer);

    w1.join();
    w2.join();
    r1.join();
    r2.join();
    r3.join();
    r4.join();
}
