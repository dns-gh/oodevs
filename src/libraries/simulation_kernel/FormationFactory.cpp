// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "simulation_kernel/FormationFactory.h"
#include "simulation_kernel/Entities/MIL_Formation.h"
#include "simulation_kernel/AutomateFactory_ABC.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( FormationFactory, "FormationFactory" )

template< typename Archive >
void save_construct_data( Archive& archive, const FormationFactory* factory, const unsigned int /*version*/ )
{
    const AutomateFactory_ABC* const automateFactory = &factory->automateFactory_;
    archive << automateFactory;
}
template< typename Archive >
void load_construct_data( Archive& archive, FormationFactory* factory, const unsigned int /*version*/ )
{
    AutomateFactory_ABC* automateFactory;
    archive >> automateFactory;
    ::new( factory )FormationFactory( *automateFactory );
}

// -----------------------------------------------------------------------------
// Name: FormationFactory constructor
// Created: MGD 2009-10-22
// -----------------------------------------------------------------------------
FormationFactory::FormationFactory( AutomateFactory_ABC& automateFactory )
    : automateFactory_( automateFactory )
{

}

// -----------------------------------------------------------------------------
// Name: FormationFactory destructor
// Created: MGD 2009-10-22
// -----------------------------------------------------------------------------
FormationFactory::~FormationFactory()
{

}

// -----------------------------------------------------------------------------
// Name: FormationFactory::Create
// Created: MGD 2009-10-22
// -----------------------------------------------------------------------------
MIL_Formation& FormationFactory::Create( xml::xistream& xis, MIL_Army_ABC& army, MIL_Formation* parent )
{
    MIL_Formation& formation = *new MIL_Formation( xis, army, parent, *this, automateFactory_ );
    Register( formation.GetID(), formation );
    return formation;
}