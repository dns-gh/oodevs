// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectMessageCallback_h_
#define __ObjectMessageCallback_h_

#include "ObjectMessageCallback_ABC.h"
#include "MessageCallback_ABC.h"
#include "Message.h"
#include "ConnectionError.h"
#include <tools/Exception.h>
#pragma warning( push, 0 )
#include <google/protobuf/descriptor.h>
#pragma warning( pop )
#include <boost/lexical_cast.hpp>
#include <vector>

namespace tools
{
// =============================================================================
/** @class  ObjectMessageCallback
    @brief  Callback wrapper
*/
// Created: AGE 2007-03-07
// =============================================================================
template< typename T >
class ObjectMessageCallback : public ObjectMessageCallback_ABC
{
public:
    //! @name Types
    //@{
    typedef std::function< void( const std::string&, const T& ) > T_ConstCallback;
    typedef std::function< void( const std::string&, T& ) >       T_Callback;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ObjectMessageCallback() {}
    virtual ~ObjectMessageCallback() {}
    //@}

    //! @name Operations
    //@{
    void AddCallback( const T_ConstCallback& callback )
    {
        constCallbacks_.push_back( callback );
    }
    void AddCallback( const T_Callback& callback )
    {
        callbacks_.push_back( callback );
    }
    void OnMessage( const std::string& link, const T& message ) const
    {
        try
        {
            for( auto it = constCallbacks_.begin(); it != constCallbacks_.end(); ++it )
                (*it)( link, message );
        }
        catch( const ConnectionError& )
        {
            throw;
        }
        catch( const std::exception& e )
        {
            throw MASA_EXCEPTION( tools::GetExceptionMsg( e ) + (" " + message.ShortDebugString()) );
        }
    }
    void OnMessage( const std::string& link, T& message ) const
    {
        try
        {
            for( auto it = callbacks_.begin(); it != callbacks_.end(); ++it )
                (*it)( link, message );
        }
        catch( const ConnectionError& )
        {
            throw;
        }
        catch( const std::exception& e )
        {
            throw MASA_EXCEPTION( tools::GetExceptionMsg( e ) + (" " + message.ShortDebugString()) );
        }
    }
    virtual void OnMessage( const std::string& link, Message& message, MessageCallback_ABC& callback ) const
    {
        T t;
        if( ! t.ParseFromArray( message.Data(), static_cast< int >( message.Size() ) ) )
            throw MASA_EXCEPTION( "Error deserializing message of type \"" + t.GetDescriptor()->full_name() + '"' );
        static const unsigned long threshold = 32 * 1024; // 32 kB
        if( message.Size() > threshold )
            callback.OnWarning( link, "Long message detected (" +
                boost::lexical_cast< std::string >( message.Size() ) + " bytes): " +
                t.ShortDebugString() );
        OnMessage( link, t );
        OnMessage( link, static_cast< const T& >( t ) );
    }
    virtual void OnMessage( const std::string& link, google::protobuf::Message& message ) const
    {
        OnMessage( link, static_cast< const T& >( message ) );
        OnMessage( link, static_cast< T& >( message ) );
    }
    //@}

private:
    //! @name Copy / Assignment
    //@{
    ObjectMessageCallback( const ObjectMessageCallback& );
    ObjectMessageCallback& operator=( const ObjectMessageCallback& );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< T_ConstCallback >              T_ConstCallbacks;
    typedef typename T_ConstCallbacks::const_iterator CIT_ConstCallbacks;

    typedef std::vector< T_Callback >              T_Callbacks;
    typedef typename T_Callbacks::const_iterator CIT_Callbacks;
    //@}

private:
    //! @name Member data
    //@{
    T_ConstCallbacks constCallbacks_;
    T_Callbacks callbacks_;
    //@}
};

}

#endif // __ObjectMessageCallback_h_
