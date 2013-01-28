// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Logger_ABC_h_
#define __Logger_ABC_h_

#include <boost/noncopyable.hpp>

namespace kernel
{

// =============================================================================
/** @class  Logger_ABC
    @brief  Logger definition
*/
// Created: AGE 2008-05-16
// =============================================================================
class Logger_ABC : private boost::noncopyable
{
public:
    //! @name Operations
    //@{
    virtual void Info( const std::string& message ) = 0;
    virtual void Warning( const std::string& message ) = 0;
    virtual void Error( const std::string& message ) = 0;
    //@}
};

}

#endif // __Logger_ABC_h_
