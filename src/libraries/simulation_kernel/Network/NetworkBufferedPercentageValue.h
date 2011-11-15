// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __NetworkBufferedPercentageValue_h_
#define __NetworkBufferedPercentageValue_h_

// =============================================================================
/** @class  NetworkBufferedPercentageValue
    @brief  NetworkBufferedPercentageValue
*/
// Created: NLD 2010-10-25
// =============================================================================
template< typename T >
class NetworkBufferedPercentageValue
{
public:
    //! @name Constructors/Destructor
    //@{
    NetworkBufferedPercentageValue( const T& value, const T& threshold, const T& minValue, const T& maxValue );
    NetworkBufferedPercentageValue( const NetworkBufferedPercentageValue& rhs );
    //@}

    //! @name Main
    //@{
          bool Set         ( const T& value ); // Return true if the value has been modified
    const T&   Get         () const;
          bool NeedToBeSent() const;
    const T&   Send        ();
    //@}

    //! @name Operators
    //@{
    NetworkBufferedPercentageValue< T >& operator=( const NetworkBufferedPercentageValue& rhs );
    //@}

private:
    //! @name Member data
    //@{
    T currentValue_;
    T lastSentValue_;

    T threshold_;
    T minValue_;
    T maxValue_;
    //@}
};

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: NetworkBufferedPercentageValue< T > constructor
// Created: NLD 2010-10-25
// -----------------------------------------------------------------------------
template< typename T >
NetworkBufferedPercentageValue< T >::NetworkBufferedPercentageValue( const T& value, const T& threshold, const T& minValue, const T& maxValue )
    : currentValue_ ( value )
    , lastSentValue_( std::numeric_limits< T >::min() )
    , threshold_    ( threshold )
    , minValue_     ( minValue )
    , maxValue_     ( maxValue )
{
}

// -----------------------------------------------------------------------------
// Name: NetworkBufferedPercentageValue< T > constructor
// Created: NLD 2010-10-25
// -----------------------------------------------------------------------------
template< typename T >
NetworkBufferedPercentageValue< T >::NetworkBufferedPercentageValue( const NetworkBufferedPercentageValue& rhs )
    : currentValue_ ( rhs.currentValue_ )
    , lastSentValue_( rhs.lastSentValue_ )
    , threshold_    ( rhs.threshold_ )
    , minValue_     ( rhs.minValue_ )
    , maxValue_     ( rhs.maxValue_ )
{
}

// -----------------------------------------------------------------------------
// Name: NetworkBufferedPercentageValue< T >::Set
// Created: NLD 2010-10-25
// -----------------------------------------------------------------------------
template< typename T >
bool NetworkBufferedPercentageValue< T >::Set( const T& value )
{
    currentValue_ = std::max( minValue_, std::min( maxValue_, value ) ) ;
    return NeedToBeSent();
}

// -----------------------------------------------------------------------------
// Name: NetworkBufferedPercentageValue< T >::Get
// Created: NLD 2010-10-25
// -----------------------------------------------------------------------------
template< typename T >
const T& NetworkBufferedPercentageValue< T >::Get() const
{
    return currentValue_;
}

// -----------------------------------------------------------------------------
// Name: NetworkBufferedPercentageValue< T >::NeedToBeSent
// Created: NLD 2010-10-25
// -----------------------------------------------------------------------------
template< typename T >
bool NetworkBufferedPercentageValue< T >::NeedToBeSent() const
{
    if( currentValue_ == lastSentValue_ )
        return false;
    if( currentValue_ == minValue_ || currentValue_ == maxValue_ )
        return true;
    return std::abs( lastSentValue_ - currentValue_ ) >= threshold_;
}

// -----------------------------------------------------------------------------
// Name: NetworkBufferedPercentageValue< T >::Send
// Created: NLD 2010-10-25
// -----------------------------------------------------------------------------
template< typename T >
const T& NetworkBufferedPercentageValue< T >::Send()
{
    lastSentValue_ = currentValue_;
    return currentValue_;
}

// -----------------------------------------------------------------------------
// Name: NetworkBufferedPercentageValue< T >::operator=
// Created: NLD 2010-10-25
// -----------------------------------------------------------------------------
template< typename T >
NetworkBufferedPercentageValue< T >& NetworkBufferedPercentageValue< T >::operator=( const NetworkBufferedPercentageValue& rhs )
{
    currentValue_  = rhs.currentValue_;
    lastSentValue_ = rhs.lastSentValue_;
    threshold_     = rhs.threshold_;
    minValue_      = rhs.minValue_;
    maxValue_      = rhs.maxValue_;
    return *this;
}

#endif __NetworkBufferedPercentageValue_h_