//*****************************************************************************
//
// $Created: JDY 03-02-10 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Synchronized.h $
// $Author: Jdy $
// $Modtime: 9/04/03 16:22 $
// $Revision: 6 $
// $Workfile: MT_Synchronized.h $
//
//*****************************************************************************

#ifndef __MT_Synchronized_h_
#define __MT_Synchronized_h_

//#define MT_USE_MUTEX
#define MT_USE_CRITIC

#ifdef MT_USE_MUTEX
#   include "MT/MT_Thread/MT_Mutex.h"
#else
#   include "MT/MT_Thread/MT_CriticalSection.h"
#endif

/**
 * This class ensure that the Value is accessed by only one thread. First you have to create a MT_Synchronized class with you type.
 * Then, if a thread want to modify or do anything on it, you create a MT_Accessor in a \b sub \b scope. You can modify the value
 * of the MT_Synchronized using the value() function \b until the end of the scope. So you have to put the smaller scope as you can.
 *
 * Internally, this class uses a MT_Mutex or a MT_CriticalSection object.
 *
 *\code
 // the value that you want to be thread safe.
 MT_Synchronized<int> val;
 { // create a new scope for the access
   // get an access to the value
   MT_Synchronized<int>::MT_Accessor acces(&val);
   // now, you have a thread safe access until the end of the scope, so you can do whatever you want. for example, change the value
   acces.value () = 10;
 } // end of the access
 *\endcode
 * \author JDY
 * \author Masa
 * \date 2003
 */
template <class T>
class MT_Synchronized
{
public:

	MT_Synchronized(){ }

    /**
	 * This class give you a thread safe access to the MT_Synchronized Value. Look at the example in MT_Synchronized.
	 */
	class MT_Accessor
	{
	public:

		/// get the mutex or wait
		MT_Accessor(MT_Synchronized<T> *cs)
		{
			synchronized_ = cs;
#ifdef MT_USE_MUTEX
            const_cast<MT_Mutex&>(synchronized_->ms_).Lock();
#else
            const_cast<MT_CriticalSection&>(synchronized_->cs_).Enter();
#endif
        }

		/// release the mutex
		~MT_Accessor()
		{
#ifdef MT_USE_MUTEX
            const_cast<MT_Mutex&>(synchronized_->ms_).Unlock();
#else
            const_cast<MT_CriticalSection&>(synchronized_->cs_).Leave();
#endif
			
		}

		/// access to the Value
		T &value()
		{
			return const_cast<T&>(synchronized_->value_);
		}
    
    private:
        MT_Synchronized<T> *synchronized_;
	};

    class MT_Locker
    {
        public:

		/// get the mutex or wait
		MT_Locker(MT_Synchronized<T> *cs)
		{
            bIsLocked_    =false;
			synchronized_ = cs;
        }

		/// release the mutex
		~MT_Locker()
		{
            Unlock();
            synchronized_=0;
		    
        }
        
        void Lock()
        {
            if (!bIsLocked_)
#ifdef MT_USE_MUTEX
            const_cast<MT_Mutex&>(synchronized_->ms_).Lock();
#else
            const_cast<MT_CriticalSection&>(synchronized_->cs_).Enter();
#endif
            bIsLocked_=true;
            
        }

        void Unlock()
        {
            if (bIsLocked_)
#ifdef MT_USE_MUTEX
            const_cast<MT_Mutex&>(synchronized_->ms_).Unlock();
#else
            const_cast<MT_CriticalSection&>(synchronized_->cs_).Leave();
#endif
            bIsLocked_=false;
        }

		/// access to the Value
		T &value()
		{
			return const_cast<T&>(synchronized_->value_);
		}
    
    private:
        bool                bIsLocked_;
        MT_Synchronized<T> *synchronized_;
    };

private:

	friend class MT_Synchronized::MT_Accessor;
    friend class MT_Synchronized::MT_Locker;

	/// The mutex of the synchronized value.
#ifdef MT_USE_MUTEX
    volatile MT_Mutex	ms_;
#else
    volatile MT_CriticalSection cs_;
#endif

    /// The synchronized value.
	volatile T			value_;

};

#endif
