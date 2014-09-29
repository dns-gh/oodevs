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

#include "protocol/Protocol.h"
#define BOOST_TYPEOF_SILENT
#include <boost/typeof/typeof.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/identity.hpp>
#undef BOOST_TYPEOF_SILENT

namespace
{
    template< typename T, typename Message >
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
    template< typename T, typename U, typename Message, typename Message2 >
    bool UpdateSubProperty( T& value, const Message& message, bool( Message::*checker )() const,
                            const Message2&( Message::*retriever )() const, U( Message2::*retriever2 )() const )
    {
        if( !( message.*checker )() )
            return false;
        T newValue = static_cast< T >( ( ( message.*retriever )().*retriever2 )() );
        if( newValue == value )
            return false;
        value = newValue;
        return true;
    }

#   define UPDATE_PROPERTY( message, value, field, property, container )\
    if( UpdateProperty( value, message, \
                &boost::mpl::identity< BOOST_TYPEOF( message ) >::type::has_##field, \
                &boost::mpl::identity< BOOST_TYPEOF( message ) >::type::field ) )\
        container.insert( property );

#   define UPDATE_SUBPROPERTY( message, value, parent, field, property, container )\
    if( UpdateSubProperty( value, message,\
            &boost::mpl::identity< BOOST_TYPEOF( message ) >::type::has_##parent, \
            &boost::mpl::identity< BOOST_TYPEOF( message ) >::type::parent, \
            &boost::remove_const< typename boost::remove_reference< typename boost::function_types::result_type< boost::mpl::identity< BOOST_TYPEOF( &boost::mpl::identity< BOOST_TYPEOF( message ) >::type::parent ) >::type >::type >::type >::type::field ) )\
        container.insert( property );
}

#endif // __Gaming_Tools_h_
