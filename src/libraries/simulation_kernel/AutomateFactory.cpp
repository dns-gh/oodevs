// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "AutomateFactory.h"
#include "Decision/DEC_Representations.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Tools/MIL_IDManager.h"
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_IMPLEMENT( AutomateFactory )

// -----------------------------------------------------------------------------
// Name: AutomateFactory constructor
// Created: MGD 2009-08-17
// -----------------------------------------------------------------------------
AutomateFactory::AutomateFactory( MIL_IDManager& idManager, unsigned int gcPause, unsigned int gcMult )
    : idManager_( idManager )
    , gcPause_( gcPause )
    , gcMult_( gcMult )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomateFactory destructor
// Created: MGD 2009-08-17
// -----------------------------------------------------------------------------
AutomateFactory::~AutomateFactory()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: AutomateFactory::Create
// Created: MGD 2009-08-17
// -----------------------------------------------------------------------------
MIL_Automate& AutomateFactory::Create( xml::xistream& xis, MIL_Entity_ABC& parent )
{
    unsigned int id;
    std::string strType;

    xis >> xml::attribute( "id", id )
        >> xml::attribute( "type", strType );

    const MIL_AutomateType* pType = MIL_AutomateType::FindAutomateType( strType );
    if( !pType )
        xis.error( "Unknown automat type" );

    MIL_Automate& automate = pType->InstanciateAutomate( id, parent, xis, gcPause_, gcMult_ );
    automate.ReadOverloading( xis );
    tools::Resolver< MIL_Automate >::Register( automate.GetID(), automate );

    return automate;
}

// -----------------------------------------------------------------------------
// Name: AutomateFactory::Create
// Created: LDC 2010-10-05
// -----------------------------------------------------------------------------
MIL_Automate& AutomateFactory::Create( const MIL_AutomateType& type, unsigned int knowledgeGroup, const std::string& name, MIL_Entity_ABC& parent, unsigned int context )
{
    MIL_Automate& automate = type.InstanciateAutomate( idManager_.GetFreeId(), parent, knowledgeGroup, name, gcPause_, gcMult_, context );
    tools::Resolver< MIL_Automate >::Register( automate.GetID(), automate );

    return automate;
}

// -----------------------------------------------------------------------------
// Name: AutomateFactory::load
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void AutomateFactory::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object < AutomateFactory_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: AutomateFactory::save
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void AutomateFactory::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object < AutomateFactory_ABC >( *this );
}
