// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef DEC_Logger_ABC_h
#define DEC_Logger_ABC_h

#include <boost/noncopyable.hpp>

namespace sword
{
// =============================================================================
/** @class  DEC_Logger_ABC
    @brief  DEC_Logger_ABC
*/
// Created: SLI 2013-02-07
// =============================================================================
class DEC_Logger_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Logger_ABC() {}
    virtual ~DEC_Logger_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Log( const char* name ) = 0;
    //@}
};

}

#endif // DEC_Logger_ABC_h
