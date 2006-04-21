// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InterfaceContainer_h_
#define __InterfaceContainer_h_

#include <vector>

// =============================================================================
/** @class  InterfaceContainer
    @brief  InterfaceContainer. Basically, just caches dynamic_casts
*/
// Created: AGE 2006-02-06
// =============================================================================
template< typename BaseType >
class InterfaceContainer
{

public:
    //! @name Constructors/Destructor
    //@{
             InterfaceContainer() {};
    virtual ~InterfaceContainer() {};
    //@}

    //! @name Operations
    //@{
    void Register( BaseType& i );
    void Remove  ( BaseType& i );

    template< typename Method >
    unsigned Apply( Method method );

    template< typename Method, typename T >
    unsigned Apply( Method method, T& argument );

    template< typename Method, typename T1, typename T2 >
    unsigned Apply( Method method, T1& arg1, T2& arg2 );

    template< typename Method, typename T1, typename T2, typename T3>
    unsigned Apply( Method method, T1& arg1, T2& arg2, T3& arg3 );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    InterfaceContainer( const InterfaceContainer& );            //!< Copy constructor
    InterfaceContainer& operator=( const InterfaceContainer& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< BaseType* >      T_Interfaces;
    typedef std::vector< void* >          T_UntypedInterfaces;
    typedef void* (*T_Caster)( BaseType* );
    typedef std::pair< T_Caster, T_UntypedInterfaces >          T_Implementations;
    typedef std::vector< T_Implementations*  >                  T_ImplementationMatrix;

    template< typename Method >        struct Extract { };
    template< typename M >             struct Extract< void (M::*)()       >     { typedef M Member; };
    template< typename M >             struct Extract< void (M::*)() const >     { typedef M Member; };
    template< typename M, typename A > struct Extract< void (M::*)( A& )       > { typedef M Member; };
    template< typename M, typename A > struct Extract< void (M::*)( A& ) const > { typedef M Member; };
    template< typename M, typename A1, typename A2 > struct Extract< void (M::*)( A1&, A2& )       > { typedef M Member; };
    template< typename M, typename A1, typename A2 > struct Extract< void (M::*)( A1&, A2& ) const > { typedef M Member; };
    template< typename M, typename A1, typename A2, typename A3 > struct Extract< void (M::*)( A1&, A2&, A3& )       > { typedef M Member; };
    template< typename M, typename A1, typename A2, typename A3 > struct Extract< void (M::*)( A1&, A2&, A3& ) const > { typedef M Member; };
    //@}

    //! @name Helpers
    //@{
    template< typename Interface >
    static unsigned int GetInterfaceId();

    template< typename Interface >
    static Interface* Cast( BaseType* imp );

    template< typename Interface >
    T_Implementations*& FindImplementations();

    template< typename Interface >
    T_Implementations* InitializeImplementations();
    //@}

private:
    //! @name Member data
    //@{
    static unsigned int id_;
    T_Interfaces           interfaces_;
    T_ImplementationMatrix implementations_;
    //@}
};

#include "InterfaceContainer.inl"

#endif // __InterfaceContainer_h_
