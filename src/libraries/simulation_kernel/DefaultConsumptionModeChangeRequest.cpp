// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************
#include "simulation_kernel_pch.h"

#include "DefaultConsumptionModeChangeRequest.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"

namespace dotation
{

// -----------------------------------------------------------------------------
// Name: DefaultConsumptionModeChangeRequest constructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultConsumptionModeChangeRequest::DefaultConsumptionModeChangeRequest( const PHY_ConsumptionType& newMode ) :
    newMode_( &newMode ),
    failed_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultConsumptionModeChangeRequest destructor
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
DefaultConsumptionModeChangeRequest::~DefaultConsumptionModeChangeRequest()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultConsumptionModeChangeRequest::GetType
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
const PHY_ConsumptionType& DefaultConsumptionModeChangeRequest::GetType() const
{
    return *newMode_;
}

// -----------------------------------------------------------------------------
// Name: DefaultConsumptionModeChangeRequestt::ConsumptionModeChanged
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultConsumptionModeChangeRequest::ConsumptionModeChanged(bool changed, PHY_RoleInterface_Dotations* const dotation)
{
    if(!failed_)
    {
        if(!changed)
        {
            Rollback();
            failed_ = true;
        }
        else
        {
            changedDotations_.push_back(dotation);
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultConsumptionModeChangeRequest::AllChanged
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
bool DefaultConsumptionModeChangeRequest::AllChanged() const
{
    return !failed_;
}

// -----------------------------------------------------------------------------
// Name: DefaultConsumptionModeChangeRequest::Rollback
// Created: AHC 2009-10-01
// -----------------------------------------------------------------------------
void DefaultConsumptionModeChangeRequest::Rollback()
{
    std::for_each( changedDotations_.begin(), changedDotations_.end(), std::mem_fun( &PHY_RoleInterface_Dotations::RollbackConsumptionMode ) );
}
}
