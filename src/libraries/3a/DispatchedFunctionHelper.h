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

struct DispatchedFunctionFactory
{
    template< typename K, typename BaseFactory >
    std::auto_ptr< ModelFunction_ABC > operator()( const K& key, ValueHandler_ABC< K >& keyHandler, BaseFactory baseFactory ) const
    {
        return std::auto_ptr< ModelFunction_ABC >( new DispatchedFunction< K >( key, keyHandler, baseFactory() ) );
    }
};

template< typename KeyValue >
struct DispatcherFactoryHelper
{
    template< typename FunctionFactory >
    std::auto_ptr< ModelFunction_ABC > operator()( const FunctionFactory& factory ) const
    {
        return std::auto_ptr< ModelFunction_ABC >( new Dispatcher< KeyValue, FunctionFactory >( factory ) );
    }
};

template< typename Value >
struct ValueFunctionFactory
{
    typedef typename Value::Type Type;
    std::auto_ptr< ModelFunction_ABC > operator()( ValueHandler_ABC< Type >& valueHandler ) const
    {
        return std::auto_ptr< ModelFunction_ABC >( new ModelFunction< Value >( valueHandler ) );
    }
};

template< typename KeyValue, typename Value >
struct DispatcherFactory
{
    typedef typename KeyValue::Type K;
    typedef typename Value   ::Type T;

    std::auto_ptr< ModelFunction_ABC > operator()( ValueHandler_ABC< K >& keyHandler, ValueHandler_ABC< T >& valueHandler ) const
    {
        return DispatcherFactoryHelper< KeyValue >()( 
                   boost::bind< std::auto_ptr< ModelFunction_ABC > >(
                       DispatchedFunctionFactory(), 
                       _1,
                       boost::ref( keyHandler ),
                       boost::protect( boost::bind< std::auto_ptr< ModelFunction_ABC > >( ValueFunctionFactory< Value >(), boost::ref( valueHandler ) ) )
                   )
               );
    }
};

#endif // __DispatchedFunctionHelper_h_
