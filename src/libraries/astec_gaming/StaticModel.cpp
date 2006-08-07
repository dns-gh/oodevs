// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "StaticModel.h"
#include "astec_kernel/AgentTypes.h"
#include "astec_kernel/ObjectTypes.h"
#include "astec_kernel/DetectionMap.h"
#include "astec_kernel/Controllers.h"
#include "astec_kernel/Controller.h"
#include "SurfaceFactory.h"
#include "CoordinateConverter.h"
#include "ModelLoaded.h"

// -----------------------------------------------------------------------------
// Name: StaticModel constructor
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
StaticModel::StaticModel( Controllers& controllers )
    : controllers_        ( controllers )
    , coordinateConverter_( *new CoordinateConverter() )
    , detection_          ( *new DetectionMap() )
    , types_              ( *new AgentTypes() )
    , objectTypes_        ( *new ObjectTypes() )
    , surfaceFactory_     ( *new SurfaceFactory( detection_, types_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StaticModel destructor
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
StaticModel::~StaticModel()
{
    delete &objectTypes_;
    delete &types_;
    delete &detection_;
    delete &coordinateConverter_;
}

// -----------------------------------------------------------------------------
// Name: StaticModel::Load
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
void StaticModel::Load( const std::string& scipioXml )
{
    Purge();
    types_.Load( scipioXml );
    objectTypes_.Load( scipioXml );
    coordinateConverter_.Load( scipioXml );
    detection_.Load( scipioXml );
    controllers_.controller_.Update( ModelLoaded( scipioXml ) );
}

// -----------------------------------------------------------------------------
// Name: StaticModel::Purge
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
void StaticModel::Purge()
{
    types_.Purge();
    objectTypes_.Purge();
}

    