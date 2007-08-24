// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MessageDispatcher_ABC_h_
#define __MessageDispatcher_ABC_h_

#include "ObjectMessageCallback.h"
#include "MessageIdentifierFactory.h"
#include <memory>

namespace DIN
{
    class DIN_Link;
}

namespace tools
{
// =============================================================================
/** @class  MessageDispatcher_ABC
    @brief  Message dispatcher definition
*/
// Created: AGE 2007-05-28
// =============================================================================
class MessageDispatcher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MessageDispatcher_ABC() {}
    virtual ~MessageDispatcher_ABC() {}
    //@}

    //! @name Operations
    //@{
    template< typename C, typename T >
    void RegisterMessage( C& instance, void (C::*callback)( DIN::DIN_Link& link, const T& object ) );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Register( unsigned long id, std::auto_ptr< ObjectMessageCallback_ABC > callback ) = 0;
    //@}
};

// -----------------------------------------------------------------------------
// Name: MessageDispatcher_ABC::RegisterMessage
// Created: AGE 2007-03-07
// -----------------------------------------------------------------------------
template< typename C, typename T >
void MessageDispatcher_ABC::RegisterMessage( C& instance, void (C::*callback)( DIN::DIN_Link& link, const T& object ) )
{
    const unsigned int objectClassId = MessageIdentifierFactory::GetIdentifier< T >();
    std::auto_ptr< ObjectMessageCallback_ABC > callbackObject( new ObjectMessageCallback< C, T >( instance, callback ) );
    Register( objectClassId, callbackObject );
}

}

#endif // __MessageDispatcher_ABC_h_
