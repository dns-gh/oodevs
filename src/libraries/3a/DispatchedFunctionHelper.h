// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DispatchedFunctionHelper_h_
#define __DispatchedFunctionHelper_h_

#include "DispatchedFunction.h"
#include "ModelFunction.h"
#include "Dispatcher.h"
#include <boost/bind.hpp>
#include <boost/bind/protect.hpp>
#include <boost/function.hpp>
#include <boost/utility.hpp>

namespace xml
{
    class xistream;
}

struct DispatchedFunctionFactory
{
    template< typename K, typename BaseFactory >
    boost::shared_ptr< ModelFunction_ABC > operator()( const K& key, ValueHandler_ABC< K >& keyHandler, BaseFactory baseFactory ) const
    {
        return boost::shared_ptr< ModelFunction_ABC >( new DispatchedFunction< K >( key, keyHandler, baseFactory() ) );
    }
};

template< typename KeyValue >
struct DispatcherFactoryHelper
{
    template< typename FunctionFactory >
    boost::shared_ptr< ModelFunction_ABC > operator()( const FunctionFactory& factory ) const
    {
        return boost::shared_ptr< ModelFunction_ABC >( new Dispatcher< KeyValue, FunctionFactory >( factory ) );
    }
};

template< typename Value >
struct ValueFunctionFactory
{
    explicit ValueFunctionFactory( const Value& value ) : value_( value ) {}
    typedef typename Value::Type Type;
    boost::shared_ptr< ModelFunction_ABC > operator()( ValueHandler_ABC< Type >& valueHandler ) const
    {
        return boost::shared_ptr< ModelFunction_ABC >( new ModelFunction< Value >( valueHandler, value_ ) );
    }
    Value value_;
};

template< typename KeyValue, typename Value >
struct DispatcherFactory
{
    explicit DispatcherFactory( xml::xistream& xis )
        : value_()
    {
        Initialize( value_, xis );
    }
    DispatcherFactory() : value_() {}
    typedef typename KeyValue::Type K;
    typedef typename Value   ::Type T;

    boost::shared_ptr< ModelFunction_ABC > operator()( ValueHandler_ABC< K >& keyHandler, ValueHandler_ABC< T >& valueHandler ) const
    {
        return DispatcherFactoryHelper< KeyValue >()( 
                   boost::bind< boost::shared_ptr< ModelFunction_ABC > >(
                       DispatchedFunctionFactory(), 
                       _1,
                       boost::ref( keyHandler ),
                       boost::protect( boost::bind< boost::shared_ptr< ModelFunction_ABC > >( ValueFunctionFactory< Value >( value_ ), boost::ref( valueHandler ) ) )
                   )
               );
    }
    Value value_;

    template< typename T >
    void Initialize( T& value, xml::xistream& xis, typename boost::enable_if_c< T::has_parameter >::type* = 0 )
    {
        T real( xis );
        value = real;
    }
    template< typename T >
    void Initialize( T& , xml::xistream& , typename boost::disable_if_c< T::has_parameter >::type* = 0 )
    {
        // NOTHING
    }
};

#endif // __DispatchedFunctionHelper_h_
