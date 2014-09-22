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
#include "MissionController_ABC.h"
#include "Decision/DEC_Representations.h"
#include "Decision/DEC_Logger.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Tools/MIL_IDManager.h"
#include <boost/serialization/export.hpp>

template< typename Archive >
void save_construct_data( Archive& archive, const AutomateFactory* factory, const unsigned int /*version*/ )
{
    const MIL_IDManager* const idManager = &factory->idManager_;
    const MissionController_ABC* const controller = &factory->controller_;
    archive << idManager;
    archive << controller;
    archive << factory->gcPause_;
    archive << factory->gcMult_;
    archive << factory->logger_;
}

template< typename Archive >
void load_construct_data( Archive& archive, AutomateFactory* factory, const unsigned int /*version*/ )
{
    MIL_IDManager* idManager;
    MissionController_ABC* controller;
    unsigned int gcPause;
    unsigned int gcMult;
    std::unique_ptr< sword::DEC_Logger > logger;
    archive >> idManager;
    archive >> controller;
    archive >> gcPause;
    archive >> gcMult;
    archive >> logger;
    ::new( factory )AutomateFactory( *idManager, *controller, gcPause, gcMult, std::move( logger ) );
}

BOOST_CLASS_EXPORT_IMPLEMENT( AutomateFactory )

// -----------------------------------------------------------------------------
// Name: AutomateFactory constructor
// Created: MGD 2009-08-17
// -----------------------------------------------------------------------------
AutomateFactory::AutomateFactory( MIL_IDManager& idManager,
                                  MissionController_ABC& controller,
                                  unsigned int gcPause,
                                  unsigned int gcMult,
                                  bool logEnabled )
    : idManager_ ( idManager )
    , controller_( controller )
    , gcPause_   ( gcPause )
    , gcMult_    ( gcMult )
    , logger_    ( logEnabled ? new sword::DEC_Logger( "Automate" ) : 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomateFactory constructor
// Created: SLI 2013-02-22
// -----------------------------------------------------------------------------
AutomateFactory::AutomateFactory( MIL_IDManager& idManager,
                                  MissionController_ABC& controller,
                                  unsigned int gcPause,
                                  unsigned int gcMult,
                                  std::unique_ptr< sword::DEC_Logger > logger )
    : idManager_ ( idManager )
    , controller_( controller )
    , gcPause_   ( gcPause )
    , gcMult_    ( gcMult )
    , logger_    ( std::move( logger ) )
{
    // NOTHING
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
        throw MASA_EXCEPTION( xis.context() + "Unknown automat type" );

    MIL_Automate& automate = pType->InstanciateAutomate( id, parent, controller_, xis, gcPause_, gcMult_, logger_.get() );
    automate.ReadOverloading( xis );
    tools::Resolver< MIL_Automate >::Register( automate.GetID(), automate );

    return automate;
}

// -----------------------------------------------------------------------------
// Name: AutomateFactory::Create
// Created: LDC 2010-10-05
// -----------------------------------------------------------------------------
MIL_Automate& AutomateFactory::Create( const MIL_AutomateType& type, unsigned int knowledgeGroup, const std::string& name, MIL_Entity_ABC& parent, unsigned int context, const MIL_DictionaryExtensions& extensions )
{
    MIL_Automate& automate = type.InstanciateAutomate( idManager_.GetId(), parent, controller_, knowledgeGroup, name, gcPause_, gcMult_, logger_.get(), context, extensions );
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
