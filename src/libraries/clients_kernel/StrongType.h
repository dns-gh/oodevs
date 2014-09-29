// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StrongType_h_
#define __StrongType_h_

namespace kernel
{

// =============================================================================
/** @class  StrongType
    @brief  StrongType
*/
// Created: SBO 2006-10-25
// =============================================================================
template< typename T, typename U >
class StrongType
{
public:
    typedef T BaseType;
    //! @name Constructors/Destructor
    //@{
                   StrongType();
                   StrongType( const StrongType& );
    /* implicit */ StrongType( const T& );
          virtual ~StrongType();
    //@}

    //! @name GetValue
    //@{
    operator const T&() const;
    operator T&();
    T operator->() const;
    T operator()() const;
    //@}

    //! @name Operators
    //@{
    StrongType& operator=( const StrongType& );
    StrongType& operator=( const T& );
    //@}

private:
    T value_;
};

// -----------------------------------------------------------------------------
// Name: StrongType constructor
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
template< typename T, typename U >
StrongType< T, U >::StrongType()
    : value_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StrongType constructor
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
template< typename T, typename U >
StrongType< T, U >::StrongType( const T& element )
    : value_( element )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StrongType constructor
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
template< typename T, typename U >
StrongType< T, U >::StrongType( const StrongType& rhs )
    : value_( rhs.value_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StrongType destructor
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
template< typename T, typename U >
StrongType< T, U >::~StrongType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StrongType::operator const T&
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
template< typename T, typename U >
StrongType< T, U >::operator const T&() const
{
    return value_;
}

// -----------------------------------------------------------------------------
// Name: StrongType::operator T&
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
template< typename T, typename U >
StrongType< T, U >::operator T&()
{
    return value_;
}

// -----------------------------------------------------------------------------
// Name: StrongType::operator==
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
template< typename T, typename U >
T StrongType< T, U >::operator->() const
{
    return value_;
}

// -----------------------------------------------------------------------------
// Name: StrongType::operator==
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
template< typename T, typename U >
T StrongType< T, U >::operator()() const
{
    return value_;
}

// -----------------------------------------------------------------------------
// Name: StrongType::operator=
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
template< typename T, typename U >
StrongType< T, U >& StrongType< T, U >::operator=( const StrongType& rhs )
{
    value_ = rhs.value_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: StrongType::operator=
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
template< typename T, typename U >
StrongType< T, U >& StrongType< T, U >::operator=( const T& value )
{
    value_ = value;
    return *this;
}

}

#endif // __StrongType_h_
