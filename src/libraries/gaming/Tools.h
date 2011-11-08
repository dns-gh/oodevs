// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Gaming_Tools_h_
#define __Gaming_Tools_h_

#include "reports/Tools.h"
#include <protocol/Protocol.h>
#define BOOST_TYPEOF_SILENT
#include <boost/typeof/typeof.hpp>
#undef BOOST_TYPEOF_SILENT

namespace
{
    template< typename T, typename Message, typename Message2 >
    bool UpdateProperty( T& value, const Message& message, bool( Message::*checker )() const,
                         const T&( Message::*retriever )() const )
    {
        if( !( message.*checker )() )
            return false;
        const T& newValue = ( message.*retriever )();
        if( newValue == value )
            return false;
        value = newValue;
        return true;
    }
    template< typename T, typename U, typename Message >
    bool UpdateProperty( T& value, const Message& message, bool( Message::*checker )() const,
                         U( Message::*retriever )() const )
    {
        if( !( message.*checker )() )
            return false;
        T newValue = static_cast< T >( ( message.*retriever )() );
        if( newValue == value )
            return false;
        value = newValue;
        return true;
    }

#   define UPDATE_PROPERTY( message, value, field, property, container )\
    if( UpdateProperty( value, message, &BOOST_TYPEOF( message )::has_##field, &BOOST_TYPEOF( message )::##field ) )\
        container.insert( property );
}

#endif // __Gaming_Tools_h_
