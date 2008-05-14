// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Workers_h_
#define __Workers_h_

#pragma warning( push )
#pragma warning( disable : 4275 4244 )
#include <boost/thread/mutex.hpp>
#pragma warning( pop )

#undef GetMessage
namespace tools {
    namespace thread {
        class ThreadPool;
    }
}

namespace kernel
{
    class WorkerTask_ABC;

// =============================================================================
/** @class  Workers
    @brief  Workers
*/
// Created: AGE 2006-04-20
// =============================================================================
class Workers
{

public:
    //! @name Constructors/Destructor
    //@{
             Workers();
    virtual ~Workers();
    //@}

    //! @name Operations
    //@{
    template< typename Functor, typename Commiter >
    void Enqueue( const Functor& functor, const Commiter& commiter )
    {
        std::auto_ptr< WorkerTask_ABC > task( new WorkerTask< Functor, Commiter >( functor, commiter ) );
        Enqueue( task );
    }
    void Enqueue( std::auto_ptr< WorkerTask_ABC > task );

    void CommitTasks();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Workers( const Workers& );            //!< Copy constructor
    Workers& operator=( const Workers& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< WorkerTask_ABC* > T_Tasks;
    typedef T_Tasks::const_iterator      CIT_Tasks;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< tools::thread::ThreadPool > pool_;
    boost::mutex mutex_;
    T_Tasks finished_;
    //@}
};

}

#endif // __Workers_h_
