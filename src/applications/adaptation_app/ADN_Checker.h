// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Checker_h_
#define __ADN_Checker_h_

#include <boost/noncopyable.hpp>

enum ADN_ErrorStatus
{
    eNoError = 0,
    eWarning = 1,
    eError   = 2
};

// =============================================================================
/** @class  ADN_Checker
    @brief  ADN_Checker
*/
// Created: ABR 2013-01-16
// =============================================================================
class ADN_Checker : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    ADN_Checker( ADN_ErrorStatus status, const QString& msg )
        : status_( status )
        , msg_( msg )
    {
        // NOTHING
    }
    virtual ~ADN_Checker()
    {
        // NOTHING
    }
    //@}

public:
    //! @name Member data
    //@{
    const ADN_ErrorStatus status_;
    const QString msg_;
    //@}
};

#endif // __ADN_Checker_h_
