// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CORE_USER_DATA_RETRIEVER_ABC_H
#define CORE_USER_DATA_RETRIEVER_ABC_H

namespace core
{
// =============================================================================
/** @class  UserDataRetriever
    @brief  User data retriever
*/
// Created: MCO 2012-04-24
// =============================================================================
template< typename T >
class UserDataRetriever
{
public:
    //! @name Types
    //@{
    typedef T& Result;
    //@}

    //! @name Operators
    //@{
    Result operator()( void* data ) const
    {
        return *static_cast< T* >( data );
    }
    //@}
};

// =============================================================================
/** @class  UserDataRetriever
    @brief  User data retriever specialization for pointers
*/
// Created: MCO 2012-04-24
// =============================================================================
template< typename T >
class UserDataRetriever< T* >
{
public:
    //! @name Types
    //@{
    typedef T* Result;
    //@}

    //! @name Operators
    //@{
    Result operator()( void* data ) const
    {
        return static_cast< T* >( data );
    }
    //@}
};

}

#endif // CORE_USER_DATA_RETRIEVER_ABC_H
