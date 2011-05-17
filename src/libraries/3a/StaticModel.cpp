// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "StaticModel.h"
#include "tools/ExerciseConfig.h"
#include "PowerIndicators.h"
#include "PowerModificator.h"
#include "Extractors.h"

using namespace aar;

// -----------------------------------------------------------------------------
// Name: StaticModel constructor
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
StaticModel::StaticModel( const tools::ExerciseConfig& config )
    : powerIndicators_  ( new PowerIndicators() )
    , powerModificators_( new PowerModificator() )
{
    powerIndicators_->Load( config );
    powerModificators_->Load( config );
}

// -----------------------------------------------------------------------------
// Name: StaticModel destructor
// Created: ABR 2011-02-10
// -----------------------------------------------------------------------------
StaticModel::~StaticModel()
{
    powerIndicators_->Purge();
}

// -----------------------------------------------------------------------------
// Name: StaticModel::ComputePower
// Created: ABR 2011-02-11
// -----------------------------------------------------------------------------
float StaticModel::ComputePower( const std::map< int, sword::EquipmentDotations_EquipmentDotation >& equipments, const extractors::PowerExtractor_ABC& extractor ) const
{
    double res = 0;
    for( std::map< int, sword::EquipmentDotations_EquipmentDotation >::const_iterator it = equipments.begin(); it != equipments.end(); ++it )
    {
        unsigned int powerValue = extractor.GetPowerValue( powerIndicators_->tools::Resolver< PowerIndicator, unsigned long >::Get( it->second.type().id() ) );
        double equipmentRes = 0;
        equipmentRes += it->second.available() * powerModificators_->GetAvailableModificator();
        equipmentRes += it->second.repairable() * powerModificators_->GetRepairableModificator();
        equipmentRes += it->second.repairing() * powerModificators_->GetRepairingModificator();
        equipmentRes += it->second.captured() * powerModificators_->GetCapturedModificator();
        equipmentRes += it->second.unavailable() * powerModificators_->GetUnavailableModificator();
        res += equipmentRes * powerValue;
    }
    return static_cast< float >( res );
}
