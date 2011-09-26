// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_ClientListener_ABC_h
#define shield_ClientListener_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace shield
{
    class DebugInfo_ABC;

// =============================================================================
/** @class  ClientListener_ABC
    @brief  ClientListener declaration
*/
// Created: MCO 2010-12-08
// =============================================================================
class ClientListener_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientListener_ABC() {}
    virtual ~ClientListener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Info( const std::string& message ) = 0;
    virtual void Error( const std::string& from, const std::string& message ) = 0;
    virtual void Warning( const std::string& from, const std::string& message ) = 0;
    virtual void Debug( const DebugInfo_ABC& info ) = 0;
    //@}
};

}

#endif // shield_ClientListener_ABC_h
