// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "simulation_kernel/DefaultComposantesAbleToBeFiredComputer.h"

using namespace firing;

// -----------------------------------------------------------------------------
// Name: DefaultComposantesAbleToBeFiredComputer::DefaultComposantesAbleToBeFiredComputer
// Created: MGD 2009-09-15
// -----------------------------------------------------------------------------
DefaultComposantesAbleToBeFiredComputer::DefaultComposantesAbleToBeFiredComputer( bool bFireOnlyOnMajorComposantes )
    : bFireOnlyOnMajorComposantes_( bFireOnlyOnMajorComposantes )
{

}

// -----------------------------------------------------------------------------
// Name: DefaultComposantesAbleToBeFiredComputer::~DefaultComposantesAbleToBeFiredComputer
// Created: MGD 2009-09-15
// -----------------------------------------------------------------------------
DefaultComposantesAbleToBeFiredComputer::~DefaultComposantesAbleToBeFiredComputer()
{

}

// -----------------------------------------------------------------------------
// Name: DefaultComposantesAbleToBeFiredComputer::Execute
// Created: MGD 2009-09-15
// -----------------------------------------------------------------------------
void DefaultComposantesAbleToBeFiredComputer::ApplyOnComposante( PHY_ComposantePion& fired )
{
    availableTargets_.clear();
    if( fired.CanBeFired() )
    {
        if( !bFireOnlyOnMajorComposantes_ || fired.IsMajor() )
            availableTargets_.push_back( &fired );
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultComposantesAbleToBeFiredComputer::Execute
// Created: MGD 2009-09-15
// -----------------------------------------------------------------------------
PHY_RoleInterface_Composantes::T_ComposanteVector& DefaultComposantesAbleToBeFiredComputer::ResultLimited( unsigned int nNbrWeaponsUsable )
{
    if( availableTargets_.empty() )
        return availableTargets_;


    while( targets_.size() < nNbrWeaponsUsable )
    {
        std::random_shuffle( availableTargets_.begin(), availableTargets_.end() );
        std::copy( availableTargets_.begin(), availableTargets_.end(), std::back_inserter( targets_ ) );
    }
    targets_.resize( nNbrWeaponsUsable );
    return targets_;
}

// -----------------------------------------------------------------------------
// Name: DefaultComposantesAbleToBeFiredComputer::Execute
// Created: MGD 2009-09-15
// -----------------------------------------------------------------------------
PHY_RoleInterface_Composantes::T_ComposanteVector& DefaultComposantesAbleToBeFiredComputer::ResultAll()
{
   return availableTargets_;
}

