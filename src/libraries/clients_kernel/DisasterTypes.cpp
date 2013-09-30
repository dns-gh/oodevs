// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "DisasterTypes.h"
#include "DisasterType.h"
#include "tools/ExerciseConfig.h"
#include "tools/PhyLoader.h"
#include <boost/bind.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DisasterTypes constructor
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
DisasterTypes::DisasterTypes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisasterTypes destructor
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
DisasterTypes::~DisasterTypes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisasterTypes::Load
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
void DisasterTypes::Load( const tools::ExerciseConfig& config )
{
    Purge();
    config.GetPhyLoader().LoadPhysicalFile( "disasters", boost::bind( &DisasterTypes::ReadDisasters, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: DisasterTypes::ReadDisasters
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
void DisasterTypes::ReadDisasters( xml::xistream& xis )
{
    xis >> xml::start( "disasters" )
            >> xml::list( "disaster", *this, &DisasterTypes::ReadDisaster );
}

// -----------------------------------------------------------------------------
// Name: DisasterTypes::ReadDisaster
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
void DisasterTypes::ReadDisaster( xml::xistream& xis )
{
    DisasterType* type = new DisasterType( xis );
    tools::Resolver< DisasterType, std::string >::Register( type->GetName(), *type );
}

// -----------------------------------------------------------------------------
// Name: DisasterTypes::Purge
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
void DisasterTypes::Purge()
{
    tools::Resolver< DisasterType, std::string >::DeleteAll();
}
