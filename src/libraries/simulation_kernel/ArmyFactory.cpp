// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ArmyFactory.h"
#include "simulation_kernel/Tools/MIL_Config.h"
#include "AutomateFactory.h"
#include "Entities/MIL_Army.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectManager.h"
#include "FormationFactory_ABC.h"
#include "PopulationFactory_ABC.h"
#include "InhabitantFactory_ABC.h"
#include "Knowledge/KnowledgeGroupFactory_ABC.h" // LTO
#include "MT_Tools/MT_Logger.h"

BOOST_CLASS_EXPORT_IMPLEMENT( ArmyFactory )

// -----------------------------------------------------------------------------
// Name: ArmyFactory::save_construct_data
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void save_construct_data( Archive& archive, const ArmyFactory* factory, const unsigned int /*version*/ )
{
    const AutomateFactory_ABC* const automateFactory = &factory->automateFactory_;
    const FormationFactory_ABC* const formationFactory = &factory->formationFactory_;
    const MIL_ObjectManager* const objectFactory = &factory->objectFactory_;
    const PopulationFactory_ABC* const populationFactory = &factory->populationFactory_;
    const InhabitantFactory_ABC* const inhabitantFactory = &factory->inhabitantFactory_;
    const KnowledgeGroupFactory_ABC* const knowledgeGroupFactory = &factory->knowledgeGroupFactory_; // LTO
    archive << automateFactory;
    archive << formationFactory;
    archive << objectFactory;
    archive << populationFactory;
    archive << inhabitantFactory;
    archive << knowledgeGroupFactory;
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory::save_construct_data
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void load_construct_data( Archive& archive, ArmyFactory* factory, const unsigned int /*version*/ )
{
    AutomateFactory_ABC* automateFactory;
    FormationFactory_ABC* formationFactory;
    MIL_ObjectManager* objectFactory;
    PopulationFactory_ABC* populationFactory;
    InhabitantFactory_ABC* inhabitantFactory;
    KnowledgeGroupFactory_ABC* knowledgeGroupFactory; // LTO
    archive >> automateFactory;
    archive >> formationFactory;
    archive >> objectFactory;
    archive >> populationFactory;
    archive >> inhabitantFactory;
    archive >> knowledgeGroupFactory;
    ::new( factory )ArmyFactory( *automateFactory, *formationFactory, *objectFactory, *populationFactory, *inhabitantFactory, *knowledgeGroupFactory ); // LTO
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory constructor
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
ArmyFactory::ArmyFactory( AutomateFactory_ABC& automateFactory, FormationFactory_ABC& formationFactory, MIL_ObjectManager& objectFactory, PopulationFactory_ABC& populationFactory, InhabitantFactory_ABC& inhabitantFactory, KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
    : automateFactory_      ( automateFactory )
    , formationFactory_     ( formationFactory )
    , objectFactory_        ( objectFactory )
    , populationFactory_    ( populationFactory )
    , inhabitantFactory_    ( inhabitantFactory )
    , knowledgeGroupFactory_( knowledgeGroupFactory ) // LTO
    , diplomacyConverter_   ( new MT_Converter< std::string, E_Diplomacy, sCaseInsensitiveLess >( eDiplomacy_Inconnu ) ) // LTO
{
    InitializeDiplomacies();
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory destructor
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
ArmyFactory::~ArmyFactory()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory::Create
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
MIL_Army_ABC* ArmyFactory::Create( const std::string& tag, xml::xistream& xis, const MIL_Config& config )
{
    MIL_Army_ABC* army = 0;
    if( tag == "party" )
    {
        MIL_Army_ABC* army = new MIL_Army( xis, *this, formationFactory_, automateFactory_, objectFactory_,
                                           populationFactory_, inhabitantFactory_, knowledgeGroupFactory_, *diplomacyConverter_,
                                           config.CanCreateParty( xis.attribute< unsigned int >( "id" ) ) );
        Register( army->GetID(), *army );
    }
    else if( tag == "no-party" )
    {
        if( config.CanCreateNoPartyObjects() )
        {
            xis >> xml::optional
                >> xml::start( "objects" )
                    >> xml::list( "object", *this, &ArmyFactory::ReadNoSideObject )
                >> xml::end;
        }
    }
    else
        throw MASA_EXCEPTION( xis.context() + "Unknown tag in parties" );
    return army;
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory::ReadNoSideObject
// Created: JSR 2011-11-15
// -----------------------------------------------------------------------------
void ArmyFactory::ReadNoSideObject( xml::xistream& xis )
{
    try
    {
        objectFactory_.CreateObject( xis, 0 );
    }
    catch( const std::exception& e)
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory::Finalize
// Created: LMT 2011-07-21
// -----------------------------------------------------------------------------
void ArmyFactory::Finalize()
{
    Apply( boost::bind( &MIL_Army_ABC::Finalize, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory::InitializeDiplomacies
// Created: HBD 2009-12-22
// -----------------------------------------------------------------------------
void ArmyFactory::InitializeDiplomacies()
{
    MT_LOG_INFO_MSG( "Initializing armies diplomacies" );
    diplomacyConverter_->Register( "enemy"  , eDiplomacy_Ennemi   );
    diplomacyConverter_->Register( "friend" , eDiplomacy_Ami  );
    diplomacyConverter_->Register( "neutral", eDiplomacy_Neutre );
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory::load
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void ArmyFactory::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object < ArmyFactory_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: ArmyFactory::save
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void ArmyFactory::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object < ArmyFactory_ABC >( *this );
}
