// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Logger_ABC_h_
#define __Logger_ABC_h_

#include <boost/noncopyable.hpp>
#include <string>

namespace mission_tester
{
// =============================================================================
/** @class  Logger_ABC
    @brief  Logger_ABC
*/
// Created: MCO 2011-05-25
// =============================================================================
class Logger_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Logger_ABC() {}
    virtual ~Logger_ABC() {}
    //@}

public:
    //! @name Operations
    //@{
    virtual void Write( const std::string& input ) = 0;
    //@}
};

}

#endif // __Logger_ABC_h_
