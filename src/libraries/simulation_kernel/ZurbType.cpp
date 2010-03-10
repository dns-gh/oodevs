// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ZurbType.h"
#include <urban/StaticModel.h>
#include <xeumeuleu/xml.h>

ZurbType* ZurbType::zurbType_ = 0;
// -----------------------------------------------------------------------------
// Name: PHY_Volume::Initialize
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ZurbType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing zurb" );
    zurbType_ = new ZurbType( xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_Volume::Terminate
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ZurbType::Terminate()
{
    delete zurbType_;
}

// -----------------------------------------------------------------------------
// Name: ZurbType constructor
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
ZurbType::ZurbType( xml::xistream& xis )
    : staticModel_( new urban::StaticModel() )
{
    staticModel_->Load( xis );
}

// -----------------------------------------------------------------------------
// Name: ZurbType destructor
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
ZurbType::~ZurbType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ZurbType::GetStaticModel
// Created: SLG 2010-03-05
// -----------------------------------------------------------------------------
urban::StaticModel& ZurbType::GetStaticModel()
{
    return *staticModel_;
}

// -----------------------------------------------------------------------------
// Name: ZurbType::GetZurbType
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
ZurbType& ZurbType::GetZurbType()
{
    return *zurbType_;
}