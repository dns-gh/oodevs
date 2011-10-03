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
#include <google/protobuf/descriptor.h>
#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>
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
    typedef boost::function< void( const std::string&, const T& ) > T_Callback;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ObjectMessageCallback() {}
    virtual ~ObjectMessageCallback() {}
    //@}

    //! @name Operations
    //@{
    void AddCallback( const T_Callback& callback )
    {
        callbacks_.push_back( callback );
    }
    virtual void OnMessage( const std::string& link, Message& message, MessageCallback_ABC& callback )
    {
        T t;
        if( ! t.ParseFromArray( message.Data(), static_cast< int >( message.Size() ) ) )
            throw std::runtime_error( "Error deserializing message of type \"" + t.GetDescriptor()->full_name() + '"' );
        static const unsigned long threshold = 32 * 1024; // 32 kB
        if( message.Size() > threshold )
            callback.OnWarning( link,
                "Message size larger than " + boost::lexical_cast< std::string >( threshold ) + " detected" + " " + t.ShortDebugString() );
        try
        {
            for( CIT_Callbacks it = callbacks_.begin(); it != callbacks_.end(); ++it )
                (*it)( link, t );
        }
        catch( std::exception& e )
        {
            throw std::runtime_error( e.what() + (" " + t.ShortDebugString()) );
        }
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
    typedef std::vector< T_Callback >              T_Callbacks;
    typedef typename T_Callbacks::const_iterator CIT_Callbacks;
    //@}

private:
    //! @name Member data
    //@{
    T_Callbacks callbacks_;
    //@}
};

}

#endif // __ObjectMessageCallback_h_
