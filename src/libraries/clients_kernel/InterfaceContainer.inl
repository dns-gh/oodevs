// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include <stdexcept>
#include <string>
#include <algorithm>

namespace kernel
{

template< typename BaseType >
unsigned int InterfaceContainer< BaseType >::id_ = 0;

// -----------------------------------------------------------------------------
// Name: InterfaceContainer< BaseType >::GetInterfaceId
// Created: AGE 2006-02-06
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Interface >
unsigned int InterfaceContainer< BaseType >::GetInterfaceId()
{
    static unsigned id = id_++;
    return id;
}

// -----------------------------------------------------------------------------
// Name: InterfaceContainer< BaseType >::Cast
// Created: AGE 2006-02-06
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Interface >
Interface* InterfaceContainer< BaseType >::Cast( BaseType* ext )
{
    return dynamic_cast< Interface* >( ext );
}

// -----------------------------------------------------------------------------
// Name: InterfaceContainer< BaseType >::Register
// Created: AGE 2006-02-07
// -----------------------------------------------------------------------------
template< typename BaseType >
void InterfaceContainer< BaseType >::Register( BaseType& i )
{
    T_Interfaces::iterator it = std::find( interfaces_.begin(), interfaces_.end(), &i );
    if( it != interfaces_.end() )
        throw std::runtime_error( "Interface already registered" );

    interfaces_.push_back( &i );
    for( unsigned int index = 0; index < implementations_.size(); ++index )
    {
        T_Implementations* imps = implementations_.at( index );
        if( imps )
        {
            T_Caster caster = imps->first;
            void* imp = caster( &i );
            if( imp )
                imps->second.push_back( imp );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: InterfaceContainer< BaseType >::Remove
// Created: AGE 2006-02-07
// -----------------------------------------------------------------------------
template< typename BaseType >
void InterfaceContainer< BaseType >::Remove( BaseType& i )
{
    T_Interfaces::iterator it = std::find( interfaces_.begin(), interfaces_.end(), &i );
    if( it == interfaces_.end() )
        throw std::runtime_error( std::string( "Interface '" ) + typeid( i ).name() + "' not registered" );

    std::swap( *it, interfaces_.back() ); 
    interfaces_.pop_back();

    for( unsigned int index = 0; index < implementations_.size(); ++index )
    {
        T_Implementations* imps = implementations_.at( index );
        if( imps )
        {
            T_Caster caster = imps->first;
            void* imp = caster( &i );
            T_UntypedInterfaces::iterator impIt = std::find( imps->second.begin(), imps->second.end(), imp );
            if( impIt != imps->second.end() )
            {
                std::swap( *impIt, imps->second.back() );
                imps->second.pop_back();
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: InterfaceContainer< BaseType >::FindImplementations
// Created: AGE 2006-02-06
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Interface >
typename InterfaceContainer< BaseType >::T_Implementations*& InterfaceContainer< BaseType >::FindImplementations()
{
    const unsigned int id = GetInterfaceId< Interface >();
    if( implementations_.size() < id + 1)
        implementations_.resize( id + 1, 0 );
    return implementations_[ id ];
}

// -----------------------------------------------------------------------------
// Name: InterfaceContainer< BaseType >::InitializeImplementations
// Created: AGE 2006-02-06
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Interface >
typename InterfaceContainer< BaseType >::T_Implementations* InterfaceContainer< BaseType >::InitializeImplementations()
{
    T_Implementations* implementations = new T_Implementations();
    Interface* (*caster)( BaseType* ) = Cast< Interface >;
    implementations->first = T_Caster( caster );

    for( unsigned int index = 0; index < interfaces_.size(); ++index )
    {
        BaseType* ext = interfaces_.at( index );
        Interface* imp = caster( ext );
        if( imp )
            implementations->second.push_back( imp );
    }
    return implementations;
}

// -----------------------------------------------------------------------------
// Name: InterfaceContainer::GetImplementations
// Created: AGE 2006-07-03
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Interface >
typename InterfaceContainer< BaseType >::T_Implementations* InterfaceContainer< BaseType >::GetImplementations()
{
    T_Implementations*& implementations = FindImplementations< Interface >();
    if( ! implementations )
        implementations = InitializeImplementations< Interface >();
    return implementations;
}

// -----------------------------------------------------------------------------
// Name: InterfaceContainer< BaseType >::Apply
// Created: AGE 2006-02-06
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Method >
unsigned InterfaceContainer< BaseType >::Apply( Method method )
{
    typedef typename Extract< Method >::Member Member;
    T_Implementations* implementations = GetImplementations< Member >();

    for( unsigned int index = 0; index < implementations->second.size(); ++index )
    {
        void* ext = implementations->second.at( index );
        Member* imp = reinterpret_cast< Member* >( ext );
        (*imp.*method)();
    }
    return implementations->second.size();
}

// -----------------------------------------------------------------------------
// Name: InterfaceContainer< BaseType >::Apply
// Created: AGE 2006-02-06
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Method, typename T >
unsigned InterfaceContainer< BaseType >::Apply( Method method, T& argument )
{
    typedef typename Extract< Method >::Member Member;
    T_Implementations* implementations = GetImplementations< Member >();

    for( unsigned int index = 0; index < implementations->second.size(); ++index )
    {
        void* ext = implementations->second.at( index );
        Member* imp = reinterpret_cast< Member* >( ext );
        (*imp.*method)( argument );
    }
    return implementations->second.size();
}

// $$$$ AGE 2006-02-08: Pourrait peut etre factoriser tout ce foin hmm ?
// -----------------------------------------------------------------------------
// Name: InterfaceContainer::Apply
// Created: AGE 2006-02-07
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Method, typename T1, typename T2 >
unsigned InterfaceContainer< BaseType >::Apply( Method method, T1& arg1, T2& arg2 )
{
    typedef typename Extract< Method >::Member Member;
    T_Implementations* implementations = GetImplementations< Member >();

    for( unsigned int index = 0; index < implementations->second.size(); ++index )
    {
        void* ext = implementations->second.at( index );
        Member* imp = reinterpret_cast< Member* >( ext );
        (*imp.*method)( arg1, arg2 );
    }
    return implementations->second.size();
}

// -----------------------------------------------------------------------------
// Name: InterfaceContainer::Apply
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Method, typename T1, typename T2, typename T3 >
unsigned InterfaceContainer< BaseType >::Apply( Method method, T1& arg1, T2& arg2, T3& arg3 )
{
    typedef typename Extract< Method >::Member Member;
    T_Implementations* implementations = GetImplementations< Member >();

    for( unsigned int index = 0; index < implementations->second.size(); ++index )
    {
        void* ext = implementations->second.at( index );
        Member* imp = reinterpret_cast< Member* >( ext );
        (*imp.*method)( arg1, arg2, arg3 );
    }
    return implementations->second.size();
}

}