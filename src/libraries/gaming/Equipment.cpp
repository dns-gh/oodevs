// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Equipment.h"
#include "clients_kernel/EquipmentType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Equipment constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Equipment::Equipment( const kernel::EquipmentType& type )
    : type_         ( type )
    , available_    ( 0 )
    , unavailable_  ( 0 )
    , repairable_   ( 0 )
    , onSiteFixable_( 0 )
    , inMaintenance_( 0 )
    , prisonners_   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Equipment constructor
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
Equipment::Equipment( const Equipment& rhs )
    : type_         ( rhs.type_ )
    , available_    ( rhs.available_ )
    , unavailable_  ( rhs.unavailable_ )
    , repairable_   ( rhs.repairable_ )
    , onSiteFixable_( rhs.onSiteFixable_ )
    , inMaintenance_( rhs.inMaintenance_ )
    , prisonners_   ( rhs.prisonners_ )
{
    for( unsigned int i = 0; i < rhs.breakdowns_.size(); ++i )
        breakdowns_.push_back( rhs.breakdowns_[ i ] );
}

// -----------------------------------------------------------------------------
// Name: Equipment destructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
Equipment::~Equipment()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Equipment::GetName
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
QString Equipment::GetName() const
{
    return type_.GetName().c_str();
}

// -----------------------------------------------------------------------------
// Name: Equipment::Update
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void Equipment::Update( const sword::EquipmentDotations_EquipmentDotation& message )
{
    available_     = message.available();
    unavailable_   = message.unavailable();
    repairable_    = message.repairable();
    onSiteFixable_ = message.on_site_fixable();
    inMaintenance_ = message.repairing();
    prisonners_    = message.captured();
    breakdowns_.resize( message.breakdowns_size() );
    for( int i = 0; i < message.breakdowns_size(); ++i )
        breakdowns_[ i ] = message.breakdowns( i );
    assert( static_cast< int >( breakdowns_.size() ) == repairable_ );
}

// -----------------------------------------------------------------------------
// Name: Equipment::Total
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
unsigned Equipment::Total() const
{
    return available_ + unavailable_ + repairable_ + onSiteFixable_ + inMaintenance_ + prisonners_;
}

// -----------------------------------------------------------------------------
// Name: Equipment::operator+
// Brief: Add a difference to current Equipment
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
Equipment Equipment::operator+( const Equipment& diff ) const
{
    Equipment equipment( *this );
    equipment.available_     += diff.available_;
    equipment.unavailable_   += diff.unavailable_;
    equipment.repairable_    += diff.repairable_;
    equipment.onSiteFixable_ += diff.onSiteFixable_;
    equipment.inMaintenance_ += diff.inMaintenance_;
    equipment.prisonners_    += diff.prisonners_;

    // $$$$ ABR 2011-07-28: Apply the list of difference : positive means to add, negative means to remove
    for( unsigned int i = 0; i < diff.breakdowns_.size(); ++i )
    {
        assert( diff.breakdowns_[ i ] != 0 );
        if( diff.breakdowns_[ i ] < 0 ) // Remove breakdown
        {
            std::vector< int >::iterator it = equipment.breakdowns_.begin();
            for( ; it != equipment.breakdowns_.end(); ++it )
                if( -diff.breakdowns_[ i ] == *it )
                {
                    equipment.breakdowns_.erase( it );
                    break;
                }
        }
        else // Add breakdown
            equipment.breakdowns_.push_back( diff.breakdowns_[ i ] );
    }
    assert( static_cast< int >( equipment.breakdowns_.size() ) == equipment.repairable_ );
    return equipment;
}

// -----------------------------------------------------------------------------
// Name: Equipment::operator-
// Brief: Create difference between 2 Equipment
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
Equipment Equipment::operator-( const Equipment& previous ) const
{
    Equipment diff( *this );
    diff.available_     -= previous.available_;
    diff.unavailable_   -= previous.unavailable_;
    diff.repairable_    -= previous.repairable_;
    diff.onSiteFixable_ -= previous.onSiteFixable_;
    diff.inMaintenance_ -= previous.inMaintenance_;
    diff.prisonners_    -= previous.prisonners_;

     // $$$$ ABR 2011-07-28: Create a list of difference : positive means to add, negative means to remove
    for( unsigned int i = 0; i < previous.breakdowns_.size(); ++i )
    {
        assert( previous.breakdowns_[ i ] > 0 );
        bool founded = false;
        for( std::vector< int >::iterator it = diff.breakdowns_.begin(); it != diff.breakdowns_.end(); ++it )
            if( previous.breakdowns_[ i ] == *it )
            {
                founded = true;
                diff.breakdowns_.erase( it );
                break;
            }
        if( !founded )
            diff.breakdowns_.push_back( -previous.breakdowns_[ i ] );
    }
    return diff;
}

// -----------------------------------------------------------------------------
// Name: Equipment::operator-
// Brief: Inverse a difference
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
Equipment Equipment::operator-() const
{
    Equipment equipment( *this );
    equipment.available_     = -available_;
    equipment.unavailable_   = -unavailable_;
    equipment.repairable_    = -repairable_;
    equipment.onSiteFixable_ = -onSiteFixable_;
    equipment.inMaintenance_ = -inMaintenance_;
    equipment.prisonners_    = -prisonners_;
    std::transform( equipment.breakdowns_.begin(), equipment.breakdowns_.end(), equipment.breakdowns_.begin(), std::negate< int >() );
    return equipment;
}

// -----------------------------------------------------------------------------
// Name: Equipment::operator=
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
Equipment& Equipment::operator=( const Equipment& rhs )
{
    available_     = rhs.available_;
    unavailable_   = rhs.unavailable_;
    repairable_    = rhs.repairable_;
    onSiteFixable_ = rhs.onSiteFixable_;
    inMaintenance_ = rhs.inMaintenance_;
    prisonners_    = rhs.prisonners_;
    breakdowns_.resize( rhs.breakdowns_.size() );
    for( unsigned int i = 0; i < rhs.breakdowns_.size(); ++i )
        breakdowns_[ i ] = rhs.breakdowns_[ i ];
    return *this;
}
