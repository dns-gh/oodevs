// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CORE_USER_DATA_H
#define CORE_USER_DATA_H

#include "UserData_ABC.h"
#ifdef _MSC_VER
#   pragma warning( push, 0 )
#   pragma warning( disable: 4996 )
#endif
#include <boost/serialization/access.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

namespace core
{
// =============================================================================
/** @class  UserData
    @brief  User data
*/
// Created: SLI 2012-01-02
// =============================================================================
template< typename T >
class UserData : public UserData_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    UserData( T userData )
        : data_( userData )
    {}
    //@}

    //! @name Operations
    //@{
    virtual void* Get() const
    {
        return static_cast< void* >( const_cast< T* >( &data_ ) );
    }
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    UserData()
    {}
    //@}

    //! @name Operations
    //@{
    friend class boost::serialization::access;
    template< typename Archive >
    void serialize( Archive& archive, const unsigned int )
    {
        archive & data_ & boost::serialization::base_object< UserData_ABC >( *this );
    }
    //@}

private:
    //! @name Member data
    //@{
    T data_;
    //@}
};

// =============================================================================
/** @class  UserData
    @brief  User data specialization for pointers
*/
// Created: SLI 2012-01-02
// =============================================================================
template< typename T >
class UserData< T* > : public UserData_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    UserData( T* userData )
        : data_( userData )
    {}
    //@}

    //! @name Operations
    //@{
    virtual void* Get() const
    {
        return static_cast< void* >( const_cast< T* >( data_ ) );
    }
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    UserData()
        : data_( 0 )
    {}
    //@}

    //! @name Operations
    //@{
    friend class boost::serialization::access;
    template< typename Archive >
    void serialize( Archive& archive, const unsigned int )
    {
        archive & data_ & boost::serialization::base_object< UserData_ABC >( *this );
    }
    //@}

private:
    //! @name Member data
    //@{
    T* data_;
    //@}
};

template< typename T >
class UserData< const T* > : public UserData_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    UserData( const T* userData )
        : data_( userData )
    {}
    //@}

    //! @name Operations
    //@{
    virtual void* Get() const
    {
        return static_cast< void* >( const_cast< T* >( data_ ) );
    }
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    UserData()
        : data_( 0 )
    {}
    //@}

    //! @name Operations
    //@{
    friend class boost::serialization::access;
    template< typename Archive >
    void serialize( Archive& archive, const unsigned int )
    {
        archive & data_ & boost::serialization::base_object< UserData_ABC >( *this );
    }
    //@}

private:
    //! @name Member data
    //@{
    const T* data_;
    //@}
};

}

#define SWORD_USER_DATA_EXPORT( Data ) \
    BOOST_CLASS_EXPORT( core::UserData< Data > )

#endif // CORE_USER_DATA_H
