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
    static id = id_++;
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
    interfaces_.push_back( &i );

    for( CIT_ImplementationMatrix it = implementations_.begin(); it != implementations_.end(); ++it )
    {
        T_Implementations* imps = *it;
        if( imps )
        {
            T_Caster caster = imps->first;
            void* imp = caster( &i );
            if( imp )
                imps->second.push_back( imp );
        };
    };
}

// -----------------------------------------------------------------------------
// Name: InterfaceContainer< BaseType >::Remove
// Created: AGE 2006-02-07
// -----------------------------------------------------------------------------
template< typename BaseType >
void InterfaceContainer< BaseType >::Remove( BaseType& i )
{
    interfaces_.erase( std::find( interfaces_.begin(), interfaces_.end(), &i ) );
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
    for( CIT_Interfaces it = interfaces_.begin(); it != interfaces_.end(); ++it )
    {
        BaseType* ext = *it;
        Interface* imp = caster( ext );
        if( imp )
            implementations->second.push_back( imp );
    }
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
    T_Implementations*& implementations = FindImplementations< Member >();
    if( ! implementations )
        implementations = InitializeImplementations< Member >();

    for( CIT_UntypedInterfaces it = implementations->second.begin(); it != implementations->second.end(); ++it )
    {
        void* ext = *it;
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
    T_Implementations*& implementations = FindImplementations< Member >();
    if( ! implementations )
        implementations = InitializeImplementations< Member >();

    for( CIT_UntypedInterfaces it = implementations->second.begin(); it != implementations->second.end(); ++it )
    {
        void* ext = *it;
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
    T_Implementations*& implementations = FindImplementations< Member >();
    if( ! implementations )
        implementations = InitializeImplementations< Member >();

    for( CIT_UntypedInterfaces it = implementations->second.begin(); it != implementations->second.end(); ++it )
    {
        void* ext = *it;
        Member* imp = reinterpret_cast< Member* >( ext );
        (*imp.*method)( arg1, arg2 );
    }
    return implementations->second.size();
}
