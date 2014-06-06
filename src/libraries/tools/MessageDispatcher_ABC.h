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
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace tools
{
// =============================================================================
/** @class  MessageDispatcher_ABC
    @brief  Message dispatcher declaration
*/
// Created: AGE 2007-05-28
// =============================================================================
class MessageDispatcher_ABC : boost::noncopyable
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
    void RegisterMessage( C& instance, void (C::*callback)( const std::string& link, const T& object ) )
    {
        RegisterMessage< T >( boost::bind( callback, &instance, _1, _2 ) );
    }
    template< typename C, typename T >
    void RegisterMessage( C& instance, void (C::*callback)( const T& object ) )
    {
        RegisterMessage< T >( boost::bind( callback, &instance, _2 ) );
    }
    template< typename T >
    void RegisterMessage( boost::function< void( const std::string&, const T& ) > callback )
    {
        const unsigned int id = MessageIdentifierFactory::GetIdentifier< T >();
        ObjectMessageCallback< T >* composite = static_cast< ObjectMessageCallback< T >* >( Retrieve( id ) );
        if( ! composite )
        {
            composite = new ObjectMessageCallback< T >();
            Register( id, std::unique_ptr< ObjectMessageCallback_ABC >( composite ) );
        }
        composite->AddCallback( callback );
    }

    template< typename T >
    void RegisterMessage( boost::function< void( const std::string&, T& ) > callback )
    {
        const unsigned int id = MessageIdentifierFactory::GetIdentifier< T >();
        ObjectMessageCallback< T >* composite = static_cast< ObjectMessageCallback< T >* >( Retrieve( id ) );
        if( ! composite )
        {
            composite = new ObjectMessageCallback< T >();
            Register( id, std::unique_ptr< ObjectMessageCallback_ABC >( composite ) );
        }
        composite->AddCallback( callback );
    }
    template< typename T >
    void Receive( const std::string& link, const T& object )
    {
        const unsigned int id = tools::MessageIdentifierFactory::GetIdentifier< T >();
        tools::ObjectMessageCallback< T >* composite =
            static_cast< tools::ObjectMessageCallback< T >* >( Retrieve( id ) );
        if( ! composite )
            throw MASA_EXCEPTION( "Unknown message" );
        composite->OnMessage( link, object );
    }
    template< typename T >
    void Receive( const std::string& link, unsigned long id, const T& object )
    {
        tools::ObjectMessageCallback_ABC* composite = Retrieve( id );
        if( ! composite )
            throw MASA_EXCEPTION( "Unknown message" );
        composite->OnMessage( link, object );
    }
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetNbMessagesReceived() const = 0;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Register( unsigned long id, std::unique_ptr< ObjectMessageCallback_ABC > callback ) = 0;
    virtual ObjectMessageCallback_ABC* Retrieve( unsigned long id ) = 0;
    //@}
};

}

#endif // __MessageDispatcher_ABC_h_
