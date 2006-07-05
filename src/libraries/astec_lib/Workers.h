// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Workers_h_
#define __Workers_h_

#undef Yield
#include "tools/thread/ThreadPool.h"

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
    template< typename Functor >
    void Enqueue( const Functor& functor )
    {
        pool_.Enqueue( functor );
    }
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Workers( const Workers& );            //!< Copy constructor
    Workers& operator=( const Workers& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    tools::thread::ThreadPool pool_;
    //@}
};

#endif // __Workers_h_
