// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_Listener_ABC_h
#define shield_Listener_ABC_h

#include <boost/noncopyable.hpp>

namespace shield
{
    class DebugInfo_ABC;

// =============================================================================
/** @class  Listener_ABC
    @brief  Listener declaration
*/
// Created: MCO 2010-12-08
// =============================================================================
class Listener_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Listener_ABC() {}
    virtual ~Listener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Info( const std::string& message ) = 0;
    virtual void Error( const std::string& message ) = 0;
    virtual void Debug( const DebugInfo_ABC& info ) = 0;
    //@}
};

}

#endif // shield_Listener_ABC_h
