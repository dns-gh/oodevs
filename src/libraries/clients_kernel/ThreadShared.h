// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ThreadShared_h_
#define __ThreadShared_h_

#include "boost/thread/mutex.hpp"
#include "boost/thread/condition.hpp"

namespace kernel
{

// =============================================================================
/** @class  ThreadShared
    @brief  ThreadShared
    // $$$$ AGE 2006-08-28: proablement a moitié foireux. Voir s'il n'y a pas ca dans boost
*/
// Created: AGE 2006-08-28
// =============================================================================
class ThreadShared
{

public:
    //! @name Constructors/Destructor
    //@{
             ThreadShared();
    virtual ~ThreadShared();
    //@}

    //! @name Types
    //@{
    struct Locker
    {
         explicit Locker( const ThreadShared& shared );
                  Locker( const Locker& rhs );
                 ~Locker();
    private:
        const ThreadShared* shared_;
    };
    //@}

protected:
    //! @name Operations
    //@{
    void Join() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ThreadShared( const ThreadShared& );            //!< Copy constructor
    ThreadShared& operator=( const ThreadShared& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void AddLock() const;
    void RemoveLock() const;
    //@}

private:
    //! @name Member data
    //@{
    mutable boost::mutex mutex_;
    mutable boost::condition condition_;
    mutable volatile unsigned count_;
    //@}
};

}

#endif // __ThreadShared_h_
