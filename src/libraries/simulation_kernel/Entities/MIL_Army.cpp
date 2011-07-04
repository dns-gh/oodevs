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
#include "Entities/Inhabitants/MIL_Inhabitant.h"
#include "Entities/Objects/MIL_ObjectManager.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/KnowledgeVisitor_ABC.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MIL_EntityManager.h"
#include "MIL_Formation.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_ScipioException.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_Publisher_ABC.h"
#include "Tools/MIL_DictionaryExtensions.h"
#include "Tools/MIL_Color.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/ArmyFactory_ABC.h"
#include "simulation_kernel/AutomateFactory_ABC.h"
#include "simulation_kernel/FormationFactory_ABC.h"
#include "simulation_kernel/InhabitantFactory_ABC.h"
#include "simulation_kernel/Knowledge/KnowledgeGroupFactory_ABC.h" // LTO
#include "simulation_kernel/PopulationFactory_ABC.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Army )

// -----------------------------------------------------------------------------
// Name: MIL_Army::save_construct_data
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void save_construct_data( Archive& archive, const MIL_Army* army, const unsigned int /*version*/ )
{
    const ArmyFactory_ABC* const armyFactory = &army->armyFactory_;
    const MT_Converter< std::string, MIL_Army_ABC::E_Diplomacy, sCaseInsensitiveLess >* const diplomacyConverter = &army->diplomacyConverter_;
    archive << armyFactory
            << diplomacyConverter;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::load_construct_data
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void load_construct_data( Archive& archive, MIL_Army* army, const unsigned int /*version*/ )
{
    ArmyFactory_ABC* armyFactory = 0;
    MT_Converter< std::string, MIL_Army_ABC::E_Diplomacy, sCaseInsensitiveLess >* diplomacyConverter = 0;
    archive >> armyFactory
            >> diplomacyConverter;
    ::new( army )MIL_Army( *armyFactory, *diplomacyConverter );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Army::MIL_Army( xml::xistream& xis, ArmyFactory_ABC& armyFactory, FormationFactory_ABC& formationFactory, AutomateFactory_ABC& automateFactory, MIL_ObjectManager& objectFactory
                  , PopulationFactory_ABC& populationFactory, InhabitantFactory_ABC& inhabitantFactory, KnowledgeGroupFactory_ABC& knowledgegroupFactory, const MT_Converter< std::string, E_Diplomacy, sCaseInsensitiveLess >& diplomacyConverter )
    : nID_                 ( xis.attribute< unsigned int >( "id" ) )
    , strName_             ( xis.attribute< std::string >( "name") )
    , nType_               ( eUnknown )
    , armyFactory_         ( armyFactory )
    , pKnowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_Army( *this ) )
    , diplomacyConverter_  ( diplomacyConverter )
    , pColor_              ( new MIL_Color( xis ) )
{
    nType_ = diplomacyConverter_.Convert( xis.attribute< std::string >( "type" ) );
    xis >> xml::start( "communication" )
            >> xml::list( "knowledge-group", *this, &MIL_Army::ReadLogistic, knowledgegroupFactory ) // LTO
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

// -----------------------------------------------------------------------------
// Name: MIL_Army constructor
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
MIL_Army::MIL_Army( ArmyFactory_ABC& armyFactory, const MT_Converter< std::string, E_Diplomacy, sCaseInsensitiveLess >& diplomacyConverter )
    : nID_                 ( 0 )
    , armyFactory_         ( armyFactory )
    , diplomacyConverter_  ( diplomacyConverter )
    , pKnowledgeBlackBoard_( 0 )
    , pExtensions_         ( 0 )
    , pColor_              ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Army destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Army::~MIL_Army()
{
    for( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        delete it->second;
    knowledgeGroups_.clear();
    delete pKnowledgeBlackBoard_;
}

namespace boost
{
    namespace serialization
    {
        // T_DiplomacyMap
        template< typename Archive >
        inline
        void serialize( Archive& file, MIL_Army::T_DiplomacyMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion );
        }
        template< typename Archive >
        void save( Archive& file, const MIL_Army::T_DiplomacyMap& map, const unsigned int )
        {
            std::size_t size = map.size();
            file << size;
            for( MIL_Army::CIT_DiplomacyMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first
                     << it->second;
            }
        }
        template< typename Archive >
        void load( Archive& file, MIL_Army::T_DiplomacyMap& map, const unsigned int )
        {
            std::size_t nNbr;
            file >> nNbr;
            while( nNbr-- )
            {
                MIL_Army* pArmy;
                file >> pArmy;
                file >> map[ pArmy ];
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::load
// Created: LDC 2010-02-22
// -----------------------------------------------------------------------------
void MIL_Army::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    MIL_DictionaryExtensions* pExtensions;
    MIL_Color* pColor;
    file >> boost::serialization::base_object< MIL_Army_ABC >( *this );
    file >> const_cast< std::string& >( strName_ );
    file >> const_cast< unsigned int& >( nID_ );
    file >> nType_;
    file >> diplomacies_;
    file >> pExtensions;
    file >> pColor;
    {
        std::size_t nNbr;
        file >> nNbr;
        while ( nNbr-- )
        {
            unsigned long index;
            file >> index;
            file >> knowledgeGroups_[ index ];
        }
    }
    {
        std::size_t nNbr;
        file >> nNbr;
        while ( nNbr-- )
        {
            unsigned long index;
            file >> index;
            file >> tools::Resolver< MIL_Population >::elements_[ index ];
        }
    }
    {
        std::size_t nNbr;
        file >> nNbr;
        while ( nNbr-- )
        {
            unsigned long index;
            file >> index;
            file >> tools::Resolver< MIL_Inhabitant >::elements_[ index ];
        }
    }
    {
        std::size_t nNbr;
        file >> nNbr;
        while ( nNbr-- )
        {
            unsigned long index;
            file >> index;
            file >> tools::Resolver< MIL_Formation >::elements_[ index ];
        }
    }
    {
        std::size_t nNbr;
        file >> nNbr;
        while ( nNbr-- )
        {
            unsigned long index;
            file >> index;
            file >> tools::Resolver< MIL_Object_ABC >::elements_[ index ];
        }
    }
    file >> pKnowledgeBlackBoard_;
    pExtensions_.reset( pExtensions );
    pColor_.reset( pColor );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::save
// Created: LDC 2010-02-22
// -----------------------------------------------------------------------------
void MIL_Army::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    const MIL_DictionaryExtensions* const pExtensions = pExtensions_.get();
    const MIL_Color* const pColor = pColor_.get();
    file << boost::serialization::base_object< MIL_Army_ABC >( *this );
    file << const_cast< std::string& >( strName_ );
    file << const_cast< unsigned int& >( nID_ );
    file << nType_;
    file << diplomacies_;
    file << pExtensions;
    file << pColor;
    {
        std::size_t size = knowledgeGroups_.size();
        file << size;
        for ( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        {
            file << it->first
                 << it->second;
        }
    }
    {
        std::size_t size = tools::Resolver< MIL_Population >::elements_.size();
        file << size;
        for ( std::map< unsigned long, MIL_Population* >::const_iterator it = tools::Resolver< MIL_Population >::elements_.begin(); it != tools::Resolver< MIL_Population >::elements_.end(); ++it )
        {
            file << it->first
                << it->second;
        }
    }
    {
        std::size_t size = tools::Resolver< MIL_Inhabitant >::elements_.size();
        file << size;
        for ( std::map< unsigned long, MIL_Inhabitant* >::const_iterator it = tools::Resolver< MIL_Inhabitant >::elements_.begin(); it != tools::Resolver< MIL_Inhabitant >::elements_.end(); ++it )
        {
            file << it->first
                 << it->second;
        }
    }
    {
        std::size_t size = tools::Resolver< MIL_Formation >::elements_.size();
        file << size;
        for ( std::map< unsigned long, MIL_Formation* >::const_iterator it = tools::Resolver< MIL_Formation >::elements_.begin(); it != tools::Resolver< MIL_Formation >::elements_.end(); ++it )
        {
            file << it->first
                 << it->second;
        }
    }
    {
        std::size_t size = tools::Resolver< MIL_Object_ABC >::elements_.size();
        file << size;
        for ( std::map< unsigned long, MIL_Object_ABC* >::const_iterator it = tools::Resolver< MIL_Object_ABC >::elements_.begin(); it != tools::Resolver< MIL_Object_ABC >::elements_.end(); ++it )
        {
            file << it->first
                 << it->second;
        }
    }
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
    pExtensions_->WriteODB( xos );

    xos << xml::start( "communication" );
    for( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        if( !it->second->IsJammed() )
            it->second->WriteODB( xos );
    xos << xml::end;

    xos << xml::start( "tactical" );
    tools::Resolver< MIL_Formation >::Apply( boost::bind( &MIL_Formation::WriteODB, _1, boost::ref(xos) ) );
    xos << xml::end;

    xos << xml::start( "logistics" );
    tools::Resolver< MIL_Formation >::Apply( boost::bind( &MIL_Formation::WriteLogisticLinksODB, _1, boost::ref(xos) ) );
    xos << xml::end;

    xos << xml::start( "objects" );
    tools::Resolver< MIL_Object_ABC >::Apply( boost::bind( &MIL_Object_ABC::WriteODB, _1, boost::ref(xos) ) );
    xos << xml::end;

    xos << xml::start( "populations" );
    tools::Resolver< MIL_Population >::Apply( boost::bind( &MIL_Population::WriteODB, _1, boost::ref(xos) ) );
    xos << xml::end;

    xos << xml::start( "inhabitants" );
    tools::Resolver< MIL_Inhabitant >::Apply( boost::bind( &MIL_Inhabitant::WriteODB, _1, boost::ref(xos) ) );
    xos << xml::end;

    xos << xml::start( "logistics" );
    tools::Resolver< MIL_Formation >::Apply( boost::bind( &MIL_Formation::WriteLogisticLinksODB, _1, boost::ref(xos) ) );
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
    for( CIT_DiplomacyMap it = diplomacies_.begin(); it != diplomacies_.end(); ++it )
    {
        xos << xml::start( "relationship" )
                << xml::attribute( "party", it->first->GetID() )
                << xml::attribute( "diplomacy", diplomacyConverter_.RevertConvert(it->second) )
            << xml::end;
    }
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
        objectFactory.CreateObject( xis, *this );
    }
    catch( std::exception& e)
    {
        MT_LOG_ERROR_MSG( e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::ReadPopulation
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
void MIL_Army::ReadPopulation( xml::xistream& xis, PopulationFactory_ABC& populationFactory )
{
    populationFactory.Create( xis, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::ReadInhabitant
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Army::ReadInhabitant( xml::xistream& xis, InhabitantFactory_ABC& inhabitantFactory )
{
    inhabitantFactory.Create( xis, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::ReadDiplomacy
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void MIL_Army::ReadDiplomacy( xml::xistream& xis )
{
    E_Diplomacy nDiplomacy = diplomacyConverter_.Convert( xis.attribute< std::string >( "diplomacy" ) );
    if( nDiplomacy == eUnknown )
        xis.error( "Unknown diplomacy relation between armies" );
    MIL_Army_ABC* pArmy = armyFactory_.Find( xis.attribute< unsigned int >( "party" ) );
    if( !pArmy )
        xis.error( "Unknown army" );
    if( diplomacies_.find( pArmy ) != diplomacies_.end() )
        xis.error( "Diplomacy between armies already exist" );
    if( pArmy == this )
        xis.error( "Self diplomacy not allowed" );
    diplomacies_[ pArmy ] = nDiplomacy;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::ReadLogistic
// LTO
// -----------------------------------------------------------------------------
void MIL_Army::ReadLogistic( xml::xistream& xis, KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
{
    knowledgeGroupFactory.Create( xis, *this );
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
        xis.error( "Unknown logistic superior" );

    if( pLogisticSuperior->GetArmy() != *this )
        xis.error( "Invalid logistic superior (not in specified side)" );

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
            xis.error( "Unknown subordinate" );
        pSubordinate->ReadLogisticLink( superior, xis );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::RegisterKnowledgeGroup
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Army::RegisterKnowledgeGroup( MIL_KnowledgeGroup& knowledgeGroup )
{
    if( ! knowledgeGroups_.insert( std::make_pair( knowledgeGroup.GetId(), &knowledgeGroup ) ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::UnregisterKnowledgeGroup
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Army::UnregisterKnowledgeGroup( MIL_KnowledgeGroup& knowledgeGroup )
{
    if( knowledgeGroups_.erase( knowledgeGroup.GetId() ) != 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::UpdateKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_Army::UpdateKnowledges(int currentTimeStep)
{
    for( CIT_KnowledgeGroupMap itKnowledgeGroup = knowledgeGroups_.begin(); itKnowledgeGroup != knowledgeGroups_.end(); ++itKnowledgeGroup )
        itKnowledgeGroup->second->UpdateKnowledges(currentTimeStep);
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->Update( currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::CleanKnowledges
// Created: NLD 2005-09-01
// -----------------------------------------------------------------------------
void MIL_Army::CleanKnowledges()
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->Clean();
    for( CIT_KnowledgeGroupMap itKnowledgeGroup = knowledgeGroups_.begin(); itKnowledgeGroup != knowledgeGroups_.end(); ++itKnowledgeGroup )
        itKnowledgeGroup->second->CleanKnowledges();
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::IsPerceived
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool MIL_Army::IsPerceived( const DEC_Knowledge_Object& knowledge ) const
{
    for( CIT_KnowledgeGroupMap itKnowledgeGroup = knowledgeGroups_.begin(); itKnowledgeGroup != knowledgeGroups_.end(); ++itKnowledgeGroup )
        if( !itKnowledgeGroup->second->IsJammed() && itKnowledgeGroup->second->IsPerceived( knowledge ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::IsAnEnemy
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
E_Tristate MIL_Army::IsAnEnemy( const MIL_Army_ABC& army ) const
{
    E_Diplomacy nRelation = GetDiplomacy( army );
    switch( nRelation )
    {
        case eUnknown : return eTristate_DontKnow;
        case eFriend  : return eTristate_False;
        case eEnemy   : return eTristate_True;
        case eNeutral : return eTristate_False;
        default:
            assert( false );
            return eTristate_DontKnow;
    };
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
    E_Diplomacy nRelation = GetDiplomacy( army );
    switch( nRelation )
    {
        case eUnknown : return eTristate_DontKnow;
        case eFriend  : return eTristate_True;
        case eEnemy   : return eTristate_False;
        case eNeutral : return eTristate_False;
        default:
            assert( false );
    };
    return eTristate_DontKnow;
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
    E_Diplomacy nRelation = GetDiplomacy( army );
    switch( nRelation )
    {
        case eUnknown : return eTristate_DontKnow;
        case eFriend  : return eTristate_False;
        case eEnemy   : return eTristate_False;
        case eNeutral : return eTristate_True;
        default:
            assert( false );
    };
    return eTristate_DontKnow;
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
    pExtensions_->SendFullState( asn );
    asn.Send( NET_Publisher_ABC::Publisher() );
    for( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
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
    for( CIT_DiplomacyMap it = diplomacies_.begin(); it != diplomacies_.end(); ++it )
    {
        client::ChangeDiplomacy asn;
        asn().mutable_party1()->set_id( nID_ );
        asn().mutable_party2()->set_id( it->first->GetID() );
        asn().set_diplomacy( sword::EnumDiplomacy( it->second ) );
        asn.Send( NET_Publisher_ABC::Publisher() );
    }
    for( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
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
    for( CIT_KnowledgeGroupMap itKnowledgeGroup = knowledgeGroups_.begin(); itKnowledgeGroup != knowledgeGroups_.end(); ++itKnowledgeGroup )
        itKnowledgeGroup->second->SendKnowledge( 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::OnReceiveChangeDiplomacy
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_Army::OnReceiveChangeDiplomacy( const sword::MissionParameters& parameters )
{
    int party2 ( 0 );
    if ( parameters.elem( 1 ).value().Get( 0 ).has_identifier() )
        party2 = parameters.elem( 1 ).value().Get( 0 ).identifier();
    else if ( parameters.elem( 1 ).value().Get( 0 ).has_party() )
        party2 = parameters.elem( 1 ).value().Get( 0 ).party().id();

    MIL_Army_ABC* pArmy2 = armyFactory_.Find( party2 );
    if( !pArmy2 || *pArmy2 == *this )
        throw NET_AsnException< sword::ChangeDiplomacyAck_ErrorCode >( sword::ChangeDiplomacyAck::error_invalid_party_diplomacy );
    E_Diplomacy nDiplomacy = eUnknown;
    switch( parameters.elem( 2 ).value().Get( 0 ).enumeration() )
    {
    case sword::unknown:
        nDiplomacy = eUnknown;
        break;
    case sword::friendly:
        nDiplomacy = eFriend;
        break;
    case sword::enemy:
        nDiplomacy = eEnemy;
        break;
    case sword::neutral:
        nDiplomacy = eNeutral;
        break;
    default:
        break;
    }
    diplomacies_[ pArmy2 ] = nDiplomacy;
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
MIL_KnowledgeGroup* MIL_Army::FindKnowledgeGroup( unsigned int nID ) const
{
    CIT_KnowledgeGroupMap it = knowledgeGroups_.find( nID );
    // LTO begin
    if( it == knowledgeGroups_.end() )
    {
        for( CIT_KnowledgeGroupMap itBis = knowledgeGroups_.begin(); itBis != knowledgeGroups_.end(); ++itBis )
        {
            MIL_KnowledgeGroup* pkg = itBis->second->FindKnowledgeGroup( nID );
            if( pkg )
                return pkg;
        }
        return 0;
    }
    // LTO end
    return it->second;
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
const MIL_Army::T_KnowledgeGroupMap& MIL_Army::GetKnowledgeGroups() const
{
    return knowledgeGroups_;
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
MIL_Army_ABC::E_Diplomacy MIL_Army::GetDiplomacy( const MIL_Army_ABC& otherArmy ) const
{
    if( &otherArmy == this )
        return eFriend;
    CIT_DiplomacyMap it = diplomacies_.find( &otherArmy );
    if( it == diplomacies_.end() )
        return eUnknown;
    else
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
    for( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
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
