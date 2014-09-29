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

BOOST_CLASS_EXPORT_IMPLEMENT( FormationFactory )

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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationFactory destructor
// Created: MGD 2009-10-22
// -----------------------------------------------------------------------------
FormationFactory::~FormationFactory()
{
    DeleteAll();
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

// -----------------------------------------------------------------------------
// Name: FormationFactory::Create
// Created: LDC 2010-10-21
// -----------------------------------------------------------------------------
MIL_Formation& FormationFactory::Create( int level, const std::string& name, const std::string& logLevelStr, MIL_Army_ABC& army, MIL_Formation* parent )
{
    MIL_Formation& formation = *new MIL_Formation( level, name, logLevelStr, army, parent );
    Register( formation.GetID(), formation );
    return formation;
}

// -----------------------------------------------------------------------------
// Name: FormationFactory::load
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void FormationFactory::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object < FormationFactory_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: FormationFactory::save
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void FormationFactory::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object <FormationFactory_ABC >( *this );
}