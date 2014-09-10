// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/MIL_Army.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 18:13 $
// $Revision: 12 $
// $Workfile: MIL_Army.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Army.h"
#include "ArmyFactory_ABC.h"
#include "AutomateFactory_ABC.h"
#include "FormationFactory_ABC.h"
#include "InhabitantFactory_ABC.h"
#include "MIL_EntityManager.h"
#include "MIL_Formation.h"
#include "PopulationFactory_ABC.h"
#include "CheckPoints/SerializationTools.h"
#include "Entities/Inhabitants/MIL_Inhabitant.h"
#include "Entities/Objects/MIL_ObjectManager.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/KnowledgeVisitor_ABC.h"
#include "Knowledge/KnowledgeGroupFactory_ABC.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MT_Tools/MT_Logger.h"
#include "Tools/NET_AsnException.h"
#include "Network/NET_Publisher_ABC.h"
#include "Tools/MIL_DictionaryExtensions.h"
#include "Tools/MIL_Color.h"
#include "Tools/MIL_MessageParameters.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Army )

// -----------------------------------------------------------------------------
// Name: MIL_Army::save_construct_data
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void save_construct_data( Archive& archive, const MIL_Army* army, const unsigned int /*version*/ )
{
    const ArmyFactory_ABC* const armyFactory = &army->armyFactory_;
    const MT_Converter< std::string, E_Diplomacy, sCaseInsensitiveLess >* const diplomacyConverter = &army->diplomacyConverter_;
    archive << armyFactory;
    archive << diplomacyConverter;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::load_construct_data
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void load_construct_data( Archive& archive, MIL_Army* army, const unsigned int /*version*/ )
{
    ArmyFactory_ABC* armyFactory = 0;
    MT_Converter< std::string, E_Diplomacy, sCaseInsensitiveLess >* diplomacyConverter = 0;
    archive >> armyFactory;
    archive >> diplomacyConverter;
    ::new( army )MIL_Army( *armyFactory, *diplomacyConverter );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Army::MIL_Army( xml::xistream& xis, ArmyFactory_ABC& armyFactory, FormationFactory_ABC& formationFactory, AutomateFactory_ABC& automateFactory, MIL_ObjectManager& objectFactory,
                    PopulationFactory_ABC& populationFactory, InhabitantFactory_ABC& inhabitantFactory, KnowledgeGroupFactory_ABC& knowledgegroupFactory,
                    const MT_Converter< std::string, E_Diplomacy, sCaseInsensitiveLess >& diplomacyConverter, bool canCreateChildren )
    : nID_                 ( xis.attribute< unsigned int >( "id" ) )
    , strName_             ( xis.attribute< std::string >( "name") )
    , nType_               ( eDiplomacy_Inconnu )
    , armyFactory_         ( armyFactory )
    , pKnowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_Army( *this ) )
    , diplomacyConverter_  ( diplomacyConverter )
    , pColor_              ( new MIL_Color( xis ) )
{
    nType_ = diplomacyConverter_.Convert( xis.attribute< std::string >( "type" ) );
    if( canCreateChildren )
    {
        xis >> xml::start( "communication" )
                >> xml::list( "knowledge-group", *this, &MIL_Army::ReadKnowledgeGroup, knowledgegroupFactory )
            >> xml::end
            >> xml::start( "tactical" )
                >> xml::list( "formation", *this, &MIL_Army::ReadFormation, formationFactory )
            >> xml::end
            >> xml::start( "objects" )
                >> xml::list( "object", *this, &MIL_Army::ReadObject, objectFactory )
            >> xml::end
            >> xml::start( "logistics" )
                >> xml::list( "logistic-base", *this, &MIL_Army::ReadLogisticLink, automateFactory, formationFactory )
            >> xml::end
            >> xml::start( "populations" )
                >> xml::list( "population", *this, &MIL_Army::ReadPopulation, populationFactory )
            >> xml::end
            >> xml::optional >> xml::start( "inhabitants" )
                >> xml::list( "inhabitant", *this, &MIL_Army::ReadInhabitant, inhabitantFactory )
            >> xml::end;
        pExtensions_.reset( new MIL_DictionaryExtensions( xis ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Army constructor
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
MIL_Army::MIL_Army( ArmyFactory_ABC& armyFactory, const MT_Converter< std::string, E_Diplomacy, sCaseInsensitiveLess >& diplomacyConverter )
    : nID_                 ( 0 )
    , armyFactory_         ( armyFactory )
    , diplomacyConverter_  ( diplomacyConverter )
    , pKnowledgeBlackBoard_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Army destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Army::~MIL_Army()
{
    knowledgeGroups_.clear();
    delete pKnowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::load
// Created: LDC 2010-02-22
// -----------------------------------------------------------------------------
void MIL_Army::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_Army_ABC >( *this );
    file >> strName_;
    file >> nID_;
    file >> nType_;
    file >> diplomacies_;
    file >> pExtensions_;
    file >> pColor_;
    file >> knowledgeGroups_;
    file >> tools::Resolver< MIL_Population >::elements_;
    file >> tools::Resolver< MIL_Inhabitant >::elements_;
    file >> tools::Resolver< MIL_Formation >::elements_;
    file >> tools::Resolver< MIL_Object_ABC >::elements_;
    file >> pKnowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::save
// Created: LDC 2010-02-22
// -----------------------------------------------------------------------------
void MIL_Army::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_Army_ABC >( *this );
    file << strName_;
    file << nID_;
    file << nType_;
    file << diplomacies_;
    file << pExtensions_;
    file << pColor_;
    file << knowledgeGroups_;
    file << tools::Resolver< MIL_Population >::elements_;
    file << tools::Resolver< MIL_Inhabitant >::elements_;
    file << tools::Resolver< MIL_Formation >::elements_;
    file << tools::Resolver< MIL_Object_ABC >::elements_;
    file << pKnowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_Army::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "party" )
            << xml::attribute( "id", nID_ )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "type", diplomacyConverter_.RevertConvert( nType_ ) );

    pColor_->WriteODB( xos );
    if( pExtensions_.get() )
        pExtensions_->WriteODB( xos );

    xos << xml::start( "objects" );
    tools::Resolver< MIL_Object_ABC >::Apply( boost::bind( &MIL_Object_ABC::WriteODB, _1, boost::ref( xos ) ) );
    xos << xml::end;

    xos << xml::start( "tactical" );
    tools::Resolver< MIL_Formation >::Apply( boost::bind( &MIL_Formation::WriteODB, _1, boost::ref( xos ) ) );
    xos << xml::end;

    xos << xml::start( "communication" );
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        if( !it->second->IsJammed() )
            it->second->WriteODB( xos );
    xos << xml::end;

    xos << xml::start( "populations" );
    tools::Resolver< MIL_Population >::Apply( boost::bind( &MIL_Population::WriteODB, _1, boost::ref( xos ) ) );
    xos << xml::end;

    xos << xml::start( "inhabitants" );
    tools::Resolver< MIL_Inhabitant >::Apply( boost::bind( &MIL_Inhabitant::WriteODB, _1, boost::ref( xos ) ) );
    xos << xml::end;

    xos << xml::start( "logistics" );
    tools::Resolver< MIL_Formation >::Apply( boost::bind( &MIL_Formation::WriteLogisticLinksODB, _1, boost::ref( xos ) ) );
    xos << xml::end;

    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::WriteDiplomacyODB
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Army::WriteDiplomacyODB( xml::xostream& xos ) const
{
    xos << xml::start( "party" )
        << xml::attribute( "id", nID_ );
    for( auto it = diplomacies_.begin(); it != diplomacies_.end(); ++it )
    {
        xos << xml::start( "relationship" )
                << xml::attribute( "party", it->first->GetID() )
                << xml::attribute( "diplomacy", diplomacyConverter_.RevertConvert(it->second) )
            << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::WriteKnowledges
// Created: NPT 2012-08-08
// -----------------------------------------------------------------------------
void MIL_Army::WriteKnowledges( xml::xostream& xos ) const
{
    xos << xml::start( "army" )
            << xml::attribute( "id", GetID() )
            << xml::attribute( "name", GetName() );

    xos << xml::start( "knowledge-groups" );
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        it->second->WriteKnowledges( xos );
    xos     << xml::end;

    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::InitializeDiplomacy
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_Army::InitializeDiplomacy( xml::xistream& xis )
{
    xis >> xml::list( "relationship", *this, &MIL_Army::ReadDiplomacy );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::ReadFormation
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
void MIL_Army::ReadFormation( xml::xistream& xis, FormationFactory_ABC& formationFactory )
{
    formationFactory.Create( xis, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::ReadObject
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
void MIL_Army::ReadObject( xml::xistream& xis, MIL_ObjectManager& objectFactory )
{
    try
    {
        xml::xisubstream sub( xis );
        objectFactory.CreateObject( sub, this );
    }
    catch( const std::exception& e)
    {
        MT_LOG_ERROR_MSG( "could not read object in party " << strName_ << "[" << nID_ << "]"
                << ": " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::ReadPopulation
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
void MIL_Army::ReadPopulation( xml::xistream& xis, PopulationFactory_ABC& populationFactory )
{
    try
    {
        xml::xisubstream sub( xis );
        populationFactory.Create( sub, *this );
    }
    catch( const std::exception& e)
    {
        MT_LOG_ERROR_MSG( "could not read crowd in party " << strName_ << "[" << nID_ << "]"
                << ": " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::ReadInhabitant
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Army::ReadInhabitant( xml::xistream& xis, InhabitantFactory_ABC& inhabitantFactory )
{
    try
    {
        xml::xisubstream sub( xis );
        inhabitantFactory.Create( sub, *this );
    }
    catch( const std::exception& e)
    {
        MT_LOG_ERROR_MSG( "could not read population in party " << strName_ << "[" << nID_ << "]"
                << ": " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::ReadDiplomacy
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void MIL_Army::ReadDiplomacy( xml::xistream& xis )
{
    E_Diplomacy nDiplomacy = diplomacyConverter_.Convert( xis.attribute< std::string >( "diplomacy" ) );
    if( nDiplomacy == eDiplomacy_Inconnu )
        throw MASA_EXCEPTION( xis.context() + "Unknown diplomacy relation between armies" );
    MIL_Army_ABC* pArmy = armyFactory_.Find( xis.attribute< unsigned int >( "party" ) );
    if( !pArmy )
        throw MASA_EXCEPTION( xis.context() + "Unknown army" );
    if( diplomacies_.find( pArmy ) != diplomacies_.end() )
        throw MASA_EXCEPTION( xis.context() + "Diplomacy between armies already exist" );
    if( pArmy->GetID() == GetID() )
        throw MASA_EXCEPTION( xis.context() + "Self diplomacy not allowed" );
    diplomacies_[ pArmy ] = nDiplomacy;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::ReadKnowledgeGroup
// -----------------------------------------------------------------------------
void MIL_Army::ReadKnowledgeGroup( xml::xistream& xis, KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
{
    knowledgeGroupFactory.Create( xis, *this, 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::ReadLogisticLink
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void MIL_Army::ReadLogisticLink( xml::xistream& xis, AutomateFactory_ABC& automateFactory, FormationFactory_ABC& formationFactory )
{
    unsigned int id = xis.attribute< unsigned int >( "id" );
    MIL_AutomateLOG* pLogisticSuperior = FindBrainLogistic( id, automateFactory, formationFactory );
    if( !pLogisticSuperior )
    {
        MT_LOG_ERROR_MSG( xis.context() + "Unknown logistic superior" );
        return;
    }

    if( pLogisticSuperior->GetArmy() != *this )
    {
        MT_LOG_ERROR_MSG( xis.context() + "Invalid logistic superior (not in specified side)" );
        return;
    }

    xis >> xml::list( "subordinate", *this, &MIL_Army::ReadLogisticLinkSubordinate, automateFactory, formationFactory, *pLogisticSuperior );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::ReadLogisticLinkSubordinate
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void MIL_Army::ReadLogisticLinkSubordinate( xml::xistream& xis, AutomateFactory_ABC& automateFactory, FormationFactory_ABC& formationFactory, MIL_AutomateLOG& superior )
{
    unsigned int subordinateID = xis.attribute< unsigned int >( "id" );

    MIL_AutomateLOG* pSubordinate = FindBrainLogistic( subordinateID, automateFactory, formationFactory );
    if( pSubordinate )
        pSubordinate->ReadLogisticLink( superior, xis );
    else
    {
        MIL_Automate* pSubordinate = automateFactory.Find( subordinateID );
        if( !pSubordinate )
        {
            MT_LOG_ERROR_MSG( xis.context() + "Unknown subordinate" );
        }
        else
            pSubordinate->ReadLogisticLink( superior, xis );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::RegisterKnowledgeGroup
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Army::RegisterKnowledgeGroup( const boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup )
{
    if( ! knowledgeGroups_.insert( std::make_pair( knowledgeGroup->GetId(), knowledgeGroup ) ).second )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::UnregisterKnowledgeGroup
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Army::UnregisterKnowledgeGroup( const boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup )
{
    if( knowledgeGroups_.erase( knowledgeGroup->GetId() ) != 1 )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::UpdateKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_Army::UpdateKnowledges( int currentTimeStep )
{
    // Update population and agent knowledge (in knowledge groups)
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        it->second->UpdateKnowledges( currentTimeStep );
    assert( pKnowledgeBlackBoard_ );
    // Update objects (at army level plus those in jammed groups)
    pKnowledgeBlackBoard_->Update( currentTimeStep );
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        it->second->UpdateObjectKnowledges( currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::CleanKnowledges
// Created: NLD 2005-09-01
// -----------------------------------------------------------------------------
void MIL_Army::CleanKnowledges()
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->Clean();
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        it->second->CleanKnowledges();
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::CleanDeletedAgentKnowledges
// Created: JSR 2013-02-21
// -----------------------------------------------------------------------------
void MIL_Army::CleanDeletedAgentKnowledges()
{
    pKnowledgeBlackBoard_->CleanDeletedAgentKnowledges();
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        it->second->CleanDeletedAgentKnowledges();
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::IsPerceived
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool MIL_Army::IsPerceived( const DEC_Knowledge_Object& knowledge ) const
{
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        if( !it->second->IsJammed() && it->second->IsPerceived( knowledge ) )
            return true;
    return false;
}

namespace
{

E_Tristate IsSomeDiplomacyValue( E_Diplomacy value, E_Diplomacy expected )
{
    if( value == expected )
        return eTristate_True;
    if( value >= 0 && value < eNbrDiplomacy && value != eDiplomacy_Inconnu )
        return eTristate_False;
    return eTristate_DontKnow;
}

} // namespace

// -----------------------------------------------------------------------------
// Name: MIL_Army::IsAnEnemy
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
E_Tristate MIL_Army::IsAnEnemy( const MIL_Army_ABC& army ) const
{
    return IsSomeDiplomacyValue( GetDiplomacy( army ), eDiplomacy_Ennemi );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::IsAnEnemy
// Created: HME 2005-12-29
// -----------------------------------------------------------------------------
E_Tristate MIL_Army::IsAnEnemy( const DEC_Knowledge_Population& knowledge ) const
{
    return IsAnEnemy( knowledge.GetArmy() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::IsAFriend
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
E_Tristate MIL_Army::IsAFriend( const MIL_Army_ABC& army ) const
{
    return IsSomeDiplomacyValue( GetDiplomacy( army ), eDiplomacy_Ami );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::IsAFriend
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
E_Tristate MIL_Army::IsAFriend( const DEC_Knowledge_Agent& knowledge ) const
{
    const MIL_Army_ABC* pArmy = knowledge.GetArmy();
    if( !pArmy )
        return eTristate_DontKnow;
    return IsAFriend( *pArmy );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::IsNeutral
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
E_Tristate MIL_Army::IsNeutral( const MIL_Army_ABC& army ) const
{
    return IsSomeDiplomacyValue( GetDiplomacy( army ), eDiplomacy_Neutre );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::Finalize
// Created: LMT 2011-07-21
// -----------------------------------------------------------------------------
void MIL_Army::Finalize()
{
    tools::Resolver< MIL_Formation >::Apply( boost::bind( &MIL_Formation::Finalize, _1 ) );
    boost::shared_ptr< MIL_KnowledgeGroup > crowdKnowledgeGroup = FindCrowdKnowledgeGroup();
    if( !crowdKnowledgeGroup )
    {
        const MIL_KnowledgeGroupType* knowledgeGroupType = MIL_KnowledgeGroupType::FindTypeOrAny( "Standard" );
        if( !knowledgeGroupType )
            throw MASA_EXCEPTION( "No Knowledge group types defined in physical database." );
        crowdKnowledgeGroup.reset( new MIL_KnowledgeGroup( *knowledgeGroupType, *this, true ) );
        RegisterKnowledgeGroup( crowdKnowledgeGroup );
    }
    tools::Resolver< MIL_Population >::Apply( boost::bind( &MIL_Population::SetKnowledgeGroup, _1, crowdKnowledgeGroup ) );
    if( const DEC_KnowledgeBlackBoard_KnowledgeGroup* blackboard = crowdKnowledgeGroup->GetKnowledge() )
    {
        const MIL_Army_ABC::T_Objects& objects = GetObjects();
        for( auto it = objects.begin(); it != objects.end(); ++it )
            blackboard->GetKsObjectKnowledgeSynthetizer().AddEphemeralObjectKnowledge( *it->second );
    }
    pKnowledgeBlackBoard_->Finalize();
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::SendCreation
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_Army::SendCreation() const
{
    client::PartyCreation asn;
    asn().mutable_party()->set_id( nID_ );
    asn().set_name( strName_.c_str() );
    asn().set_type( sword::EnumDiplomacy( nType_ ) );
    pColor_->SendFullState( asn );
    if( pExtensions_.get() )
        pExtensions_->SendFullState( asn );
    asn.Send( NET_Publisher_ABC::Publisher() );
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        it->second->SendCreation();
    tools::Resolver< MIL_Formation >::Apply( boost::bind( &MIL_Formation::SendCreation, _1, 0 ) );
    tools::Resolver< MIL_Population >::Apply( boost::bind( &MIL_Population::SendCreation, _1, 0 ) );
    tools::Resolver< MIL_Inhabitant >::Apply( boost::bind( &MIL_Inhabitant::SendCreation, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::SendFullState
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MIL_Army::SendFullState() const
{
    for( auto it = diplomacies_.begin(); it != diplomacies_.end(); ++it )
    {
        client::ChangeDiplomacy asn;
        asn().mutable_party1()->set_id( nID_ );
        asn().mutable_party2()->set_id( it->first->GetID() );
        asn().set_diplomacy( sword::EnumDiplomacy( it->second ) );
        asn.Send( NET_Publisher_ABC::Publisher() );
    }
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        it->second->SendFullState();
    tools::Resolver< MIL_Formation >::Apply( boost::bind( &MIL_Formation::SendFullState, _1, 0 ) );
    tools::Resolver< MIL_Population >::Apply( boost::bind( &MIL_Population::SendFullState, _1 ) );
    tools::Resolver< MIL_Inhabitant >::Apply( boost::bind( &MIL_Inhabitant::SendFullState, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::SendKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_Army::SendKnowledge() const
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->SendFullState( 0 );
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        it->second->SendKnowledge( 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::OnReceiveChangeDiplomacy
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_Army::ChangeDiplomacy( MIL_Army_ABC& other, E_Diplomacy diplomacy )
{
    diplomacies_[ &other ] = diplomacy;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::RegisterObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
void MIL_Army::RegisterObject( MIL_Object_ABC& object )
{
    tools::Resolver< MIL_Object_ABC >::Register( object.GetID(), object );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::UnregisterObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
void MIL_Army::UnregisterObject( MIL_Object_ABC& object )
{
    tools::Resolver< MIL_Object_ABC >::Remove( object.GetID() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::RegisterFormation
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Army::RegisterFormation( MIL_Formation& formation )
{
    tools::Resolver< MIL_Formation >::Register( formation.GetID(), formation );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::UnregisterFormation
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Army::UnregisterFormation( MIL_Formation& formation )
{
    tools::Resolver< MIL_Formation >::Remove( formation.GetID() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::RegisterPopulation
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Army::RegisterPopulation( MIL_Population& population )
{
   tools::Resolver< MIL_Population >::Register( population.GetID(), population );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::UnregisterPopulation
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Army::UnregisterPopulation( MIL_Population& population )
{
    tools::Resolver< MIL_Population >::Remove( population.GetID() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::RegisterInhabitant
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Army::RegisterInhabitant( MIL_Inhabitant& inhabitant )
{
    tools::Resolver< MIL_Inhabitant >::Register( inhabitant.GetID(), inhabitant );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::UnregisterInhabitant
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Army::UnregisterInhabitant( MIL_Inhabitant& inhabitant )
{
    tools::Resolver< MIL_Inhabitant >::Remove( inhabitant.GetID() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::FindKnowledgeGroup
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > MIL_Army::FindKnowledgeGroup( unsigned int nID ) const
{
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
    {
        if( it->second->GetId() == nID )
            return it->second;
        if( boost::shared_ptr< MIL_KnowledgeGroup > group = it->second->FindKnowledgeGroup( nID ) )
            return group;
    }
    return boost::shared_ptr< MIL_KnowledgeGroup >();
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MIL_KnowledgeGroup > MIL_Army::FindCrowdKnowledgeGroup
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > MIL_Army::FindCrowdKnowledgeGroup() const
{
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        if( it->second->IsCrowd() )
            return it->second;
    return boost::shared_ptr< MIL_KnowledgeGroup >();
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::GetID
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
unsigned int MIL_Army::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::GetKnowledgeGroups
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
const MIL_Army_ABC::T_KnowledgeGroups& MIL_Army::GetKnowledgeGroups() const
{
    return knowledgeGroups_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::GetObjects
// Created: JSR 2013-07-11
// -----------------------------------------------------------------------------
const MIL_Army_ABC::T_Objects& MIL_Army::GetObjects() const
{
    return tools::Resolver< MIL_Object_ABC >::elements_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::GetKnowledge
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Army& MIL_Army::GetKnowledge() const
{
    assert( pKnowledgeBlackBoard_ );
    return *pKnowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::GetColor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
const MIL_Color& MIL_Army::GetColor() const
{
    return *pColor_;
}

//-----------------------------------------------------------------------------
// Name: MIL_Army::GetDiplomacy
// Created: NLD 2004-03-24
//-----------------------------------------------------------------------------
E_Diplomacy MIL_Army::GetDiplomacy( const MIL_Army_ABC& otherArmy ) const
{
    if( &otherArmy == this )
        return eDiplomacy_Ami;
    auto it = diplomacies_.find( &otherArmy );
    if( it == diplomacies_.end() )
        return eDiplomacy_Inconnu;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::GetName
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
const std::string& MIL_Army::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::ApplyOnKnowledgeGroup
// Created: SLG 2010-04-07
// -----------------------------------------------------------------------------
void MIL_Army::ApplyOnKnowledgeGroup( KnowledgeVisitor_ABC& fct )
{
    for( auto it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        fct.visit( *it->second );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::FindBrainLogistic
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
MIL_AutomateLOG* MIL_Army::FindBrainLogistic( unsigned int nID, AutomateFactory_ABC& automateFactory, FormationFactory_ABC& formationFactory ) const
{
    MIL_Formation* pFormation = formationFactory.Find( nID );
    if( pFormation )
        return pFormation->GetBrainLogistic();
    MIL_Automate* pAutomate = automateFactory.Find( nID );
    if( pAutomate  )
        return pAutomate->GetBrainLogistic();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::OnReceiveUnitMagicAction
// Created: ABR 2014-08-27
// -----------------------------------------------------------------------------
void MIL_Army::OnReceiveUnitMagicAction( const sword::UnitMagicAction& msg )
{
    switch( msg.type() )
    {
    case sword::UnitMagicAction::rename:
        OnReceiveRename( msg.parameters() );
        break;
    default:
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode,
            sword::UnitActionAck::error_invalid_unit );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::OnReceiveRename
// Created: ABR 2014-08-27
// -----------------------------------------------------------------------------
void MIL_Army::OnReceiveRename( const sword::MissionParameters& parameters )
{
    protocol::CheckCount( parameters, 1 );
    const auto& name = protocol::GetString( parameters, 0 );
    strName_ = name;
    client::PartyUpdate asn;
    asn().mutable_party()->set_id( nID_ );
    asn().set_name( name.c_str() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}
