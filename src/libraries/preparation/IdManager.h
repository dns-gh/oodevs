// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IdManager_h_
#define __IdManager_h_

#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  IdManager
    @brief  IdManager
*/
// Created: SBO 2006-09-26
// =============================================================================
class IdManager : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             IdManager();
    virtual ~IdManager();
    //@}

    //! @name Operations
    //@{
    unsigned long GetNextId();
    void Reset();
    void Lock( unsigned long id );
    //@}

private:
    //! @name Member data
    //@{
    unsigned long max_;
    //@}
};

#endif // __IdManager_h_
