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
float StaticModel::ComputePower( const sword::UnitAttributes& message, const extractors::PowerExtractor_ABC& extractor ) const
{
    double res = 0;
    for( int i = 0; i < message.equipment_dotations().elem().size(); ++i )
    {
        const sword::EquipmentDotations_EquipmentDotation equipmentMsg = message.equipment_dotations().elem( i );
        unsigned int powerValue = extractor.GetPowerValue( powerIndicators_->tools::Resolver< PowerIndicator, unsigned long >::Get( equipmentMsg.type().id() ) );
        double equipmentRes = 0;
        equipmentRes += equipmentMsg.available() * powerModificators_->GetAvailableModificator();
        equipmentRes += equipmentMsg.repairable() * powerModificators_->GetRepairableModificator();
        equipmentRes += equipmentMsg.repairing() * powerModificators_->GetRepairingModificator();
        equipmentRes += equipmentMsg.captured() * powerModificators_->GetCapturedModificator();
        equipmentRes += equipmentMsg.unavailable() * powerModificators_->GetUnavailableModificator();
        res += equipmentRes * powerValue;
    }
    return static_cast< float >( res );
}
