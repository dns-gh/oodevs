// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "UrbanType.h"
#include <urban/StaticModel.h>
#include <xeumeuleu/xml.h>

UrbanType* UrbanType::urbanType_ = 0;
// -----------------------------------------------------------------------------
// Name: PHY_Volume::Initialize
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void UrbanType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing urban types" );
    urbanType_ = new UrbanType( xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_Volume::Terminate
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void UrbanType::Terminate()
{
    delete urbanType_;
}

// -----------------------------------------------------------------------------
// Name: UrbanType constructor
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
UrbanType::UrbanType( xml::xistream& xis )
    : staticModel_( new urban::StaticModel() )
{
    staticModel_->Load( xis );
}

// -----------------------------------------------------------------------------
// Name: UrbanType destructor
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
UrbanType::~UrbanType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanType::GetStaticModel
// Created: SLG 2010-03-05
// -----------------------------------------------------------------------------
urban::StaticModel& UrbanType::GetStaticModel()
{
    return *staticModel_;
}

// -----------------------------------------------------------------------------
// Name: UrbanType::GetUrbanType
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
UrbanType& UrbanType::GetUrbanType()
{
    return *urbanType_;
}