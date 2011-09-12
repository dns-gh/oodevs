// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

namespace dispatcher
{

// -----------------------------------------------------------------------------
// Name: EquipmentAvailability constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
template< typename T > inline
EquipmentAvailability< T >::EquipmentAvailability( const T& asn )
    : nEquipmentType_( asn.equipment().id() )
    , nNbrTotal_     ( asn.total() )
    , nNbrAvailable_ ( asn.available() )
    , nNbrWorking_   ( asn.working() )
    , nNbrLent_      ( asn.lent() )
    , nNbrResting_   ( asn.has_resting() ? asn.resting() : std::numeric_limits< unsigned int >::max() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentAvailability destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
template< typename T > inline
EquipmentAvailability< T >::~EquipmentAvailability()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentAvailability::Send
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
template< typename T > inline
void EquipmentAvailability< T >::Send( T& asn ) const
{
    asn.mutable_equipment()->set_id( nEquipmentType_ );
    asn.set_total( nNbrTotal_ );
    asn.set_available( nNbrAvailable_ );
    asn.set_working( nNbrWorking_ );
    asn.set_lent( nNbrLent_ );
    if( nNbrResting_ != std::numeric_limits< unsigned int >::max() )
    {
        asn.set_resting( nNbrResting_ );
    }
}

}