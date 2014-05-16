// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PHY_PerceptionWithLocation_h_
#define __PHY_PerceptionWithLocation_h_

#include "PHY_Perception_ABC.h"
#pragma warning( push, 0 )
#include <boost/ptr_container/ptr_vector.hpp>
#pragma warning( pop )

// =============================================================================
/** @class  PHY_PerceptionWithLocation
    @brief  PHY_PerceptionWithLocation
*/
// Created: LDC 2009-07-29
// =============================================================================
template <typename T >
class PHY_PerceptionWithLocation : public PHY_Perception_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_PerceptionWithLocation( PHY_RoleInterface_Perceiver& perceiver );
    virtual ~PHY_PerceptionWithLocation();
    //@}

    //! @name Operations
    //@{
    std::unique_ptr< T > Remove( int id );
    int Add( T* pLocation );
    virtual void FinalizePerception();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_PerceptionWithLocation( const PHY_PerceptionWithLocation& );            //!< Copy constructor
    PHY_PerceptionWithLocation& operator=( const PHY_PerceptionWithLocation& ); //!< Assignment operator
    //@}

protected:
    //! @name Types
    //@{
    typedef boost::ptr_vector< T > T_RecoVector;
    //@}

    //! @name Member data
    //@{
    T_RecoVector recos_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionWithLocation constructor
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
PHY_PerceptionWithLocation<T>::PHY_PerceptionWithLocation( PHY_RoleInterface_Perceiver& perceiver )
    : PHY_Perception_ABC( perceiver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionWithLocation destructor
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
PHY_PerceptionWithLocation<T>::~PHY_PerceptionWithLocation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionWithLocation::Remove
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
std::unique_ptr< T > PHY_PerceptionWithLocation<T>::Remove( int id )
{
    for( auto it = recos_.begin(); it != recos_.end(); ++it )
        if ( it->Id() == id )
            return std::unique_ptr< T >( recos_.release( it ).release() );
    return std::unique_ptr< T >();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionWithLocation::AddLocation
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
int PHY_PerceptionWithLocation<T>::Add( T* pLocation )
{
    recos_.push_back( pLocation );
    return pLocation->Id();
}

// -----------------------------------------------------------------------------
// Name: PHY_PerceptionWithLocation::FinalizePerception
// Created: LDC 2010-05-05
// -----------------------------------------------------------------------------
template< class T >
void PHY_PerceptionWithLocation<T>::FinalizePerception()
{
    // NOTHING
}

#endif // __PHY_PerceptionWithLocation_h_
