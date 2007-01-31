// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "master_pch.h"
#include "PhysicalModel.h"

using namespace master;

// -----------------------------------------------------------------------------
// Name: PhysicalModel constructor
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
PhysicalModel::PhysicalModel( const DataManager& /*dataManager*/, const Config& /*config*/, const std::string& name )
    : name_( name )
{
    MT_LOG_INFO_MSG( "Physical model loaded : '" << name_ << "'" );
}
    
// -----------------------------------------------------------------------------
// Name: PhysicalModel destructor
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
PhysicalModel::~PhysicalModel()
{
    // NOTHING
}
