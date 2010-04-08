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
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/KnowledgeVisitor_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectManager.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "MIL_Formation.h"
#include "MIL_EntityManager.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

#include "simulation_kernel/ArmyFactory_ABC.h"
#include "simulation_kernel/AutomateFactory_ABC.h"
#include "simulation_kernel/FormationFactory_ABC.h"
#include "simulation_kernel/PopulationFactory_ABC.h"
#include "simulation_kernel/Knowledge/KnowledgeGroupFactory_ABC.h" // LTO
#include <boost/bind.hpp>
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Army )

// -----------------------------------------------------------------------------
// Name: MIL_Army::save_construct_data
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void save_construct_data( Archive& archive, const MIL_Army* army, const unsigned int /*version*/ )
{
    const ArmyFactory_ABC* const armyFactory = &army->armyFactory_;
    const MT_Converter< std::string, MIL_Army_ABC::E_Diplomacy >* const diplomacyConverter = &army->diplomacyConverter_;
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
    MT_Converter< std::string, MIL_Army_ABC::E_Diplomacy >* diplomacyConverter = 0;
    archive >> armyFactory 
            >> diplomacyConverter;
    ::new( army )MIL_Army( *armyFactory, *diplomacyConverter );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Army::MIL_Army( xml::xistream& xis, ArmyFactory_ABC& armyFactory, FormationFactory_ABC& formationFactory, AutomateFactory_ABC& automateFactory, MIL_ObjectManager& objectFactory
                  , PopulationFactory_ABC& populationFactory, KnowledgeGroupFactory_ABC& knowledgegroupFactory, const MT_Converter< std::string, E_Diplomacy >& diplomacyConverter )
    : nID_                 ( xml::attribute< unsigned int >( xis, "id" ) )
    , strName_             ( xml::attribute< std::string>( xis, "name") )
    , nType_               ( eUnknown )
    , armyFactory_         ( armyFactory )
    , pKnowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_Army( *this ) )
    , diplomacyConverter_  ( diplomacyConverter )
{
    std::string strType;
    xis >> xml::attribute( "type", strType );

    nType_ = diplomacyConverter_.Convert( strType );

    xis >> xml::start( "communication" )
            >> xml::list( "knowledge-group", *this, &MIL_Army::ReadLogistic, knowledgegroupFactory ) // LTO
        >> xml::end()
        >> xml::start( "tactical" )
            >> xml::list( "formation", *this, &MIL_Army::ReadFormation, formationFactory )
        >> xml::end()
        >> xml::start( "objects" )
            >> xml::list( "object", *this, &MIL_Army::ReadObject, objectFactory )
        >> xml::end()
        >> xml::start( "logistic" )
            >> xml::list( "automat", *this, &MIL_Army::ReadAutomat, automateFactory  )
        >> xml::end()
        >> xml::start( "populations" )
            >> xml::list( "population", *this, &MIL_Army::ReadPopulation, populationFactory )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: MIL_Army constructor
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
MIL_Army::MIL_Army( ArmyFactory_ABC& armyFactory, const MT_Converter< std::string, E_Diplomacy >& diplomacyConverter )
    : nID_( 0 )
    , armyFactory_( armyFactory )
    , diplomacyConverter_( diplomacyConverter )
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

// =============================================================================
// CHECKPOINTS
// =============================================================================
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
            unsigned size = map.size();
            file << size;
            for ( MIL_Army::CIT_DiplomacyMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first
                     << it->second;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, MIL_Army::T_DiplomacyMap& map, const unsigned int )
        {
            unsigned int nNbr;
            file >> nNbr;
            while ( nNbr-- )
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
void MIL_Army::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MIL_Army_ABC >( *this );
    file >> const_cast< std::string& >( strName_ );
    file >> const_cast< uint& >( nID_ );
    file >> nType_;
    file >> diplomacies_;
    {
        unsigned int nNbr;
        file >> nNbr;
        while ( nNbr-- )
        {
            unsigned long index;
            file >> index;
            file >> knowledgeGroups_[ index ];
        }
    }
    {
        unsigned int nNbr;
        file >> nNbr;
        while ( nNbr-- )
        {
            unsigned long index;
            file >> index;
            file >> tools::Resolver< MIL_Population >::elements_[ index ];
        }
    }
    {
        unsigned int nNbr;
        file >> nNbr;
        while ( nNbr-- )
        {
            unsigned long index;
            file >> index;
            file >> tools::Resolver< MIL_Formation >::elements_[ index ];
        }
    }
    {
        unsigned int nNbr;
        file >> nNbr;
        while ( nNbr-- )
        {
            unsigned long index;
            file >> index;
            file >> tools::Resolver< MIL_Object_ABC >::elements_[ index ];
        }
    }
    file >> pKnowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::save
// Created: LDC 2010-02-22
// -----------------------------------------------------------------------------
void MIL_Army::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_Army_ABC >( *this );
    file << const_cast< std::string& >( strName_ );
    file << const_cast< uint& >( nID_ );
    file << nType_;
    file << diplomacies_;
    {
        unsigned int size = knowledgeGroups_.size();
        file << size;
        for ( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        {
            file << it->first
                 << it->second;
        }
    }
    {
        unsigned int size = tools::Resolver< MIL_Population >::elements_.size();
        file << size;
        for ( std::map< unsigned long, MIL_Population* >::const_iterator it = tools::Resolver< MIL_Population >::elements_.begin(); it != tools::Resolver< MIL_Population >::elements_.end(); ++it )
        {
            file << it->first
                 << it->second;
        }
    }
    {
        unsigned int size = tools::Resolver< MIL_Formation >::elements_.size();
        file << size;
        for ( std::map< unsigned long, MIL_Formation* >::const_iterator it = tools::Resolver< MIL_Formation >::elements_.begin(); it != tools::Resolver< MIL_Formation >::elements_.end(); ++it )
        {
            file << it->first
                 << it->second;
        }
    }
    {
        unsigned int size = tools::Resolver< MIL_Object_ABC >::elements_.size();
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
    xos << xml::start( "side" )
            << xml::attribute( "id", nID_ )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "type", diplomacyConverter_.RevertConvert( nType_ ) );

    xos     << xml::start( "communication" );
    for( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        it->second->WriteODB( xos );
    xos     << xml::end();
    
    xos     << xml::start( "tactical" );
    tools::Resolver< MIL_Formation >::Apply( boost::bind( &MIL_Formation::WriteODB, _1, boost::ref(xos) ) );
    xos     << xml::end();

    xos     << xml::start( "logistic" );
    tools::Resolver< MIL_Formation >::Apply( boost::bind( &MIL_Formation::WriteLogisticLinksODB, _1, boost::ref(xos) ) );
    xos     << xml::end();

    xos     << xml::start( "objects" );
    tools::Resolver< MIL_Object_ABC >::Apply( boost::bind( &MIL_Object_ABC::WriteODB, _1, boost::ref(xos) ) );   
    xos     << xml::end();

    xos     << xml::start( "populations" );
    tools::Resolver< MIL_Population >::Apply( boost::bind( &MIL_Population::WriteODB, _1, boost::ref(xos) ) );
    xos     << xml::end();

    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::WriteDiplomacyODB
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Army::WriteDiplomacyODB( xml::xostream& xos ) const
{
    xos << xml::start( "side" )
        << xml::attribute( "id", nID_ );
    for( CIT_DiplomacyMap it = diplomacies_.begin(); it != diplomacies_.end(); ++it )
    {
        xos << xml::start( "relationship" )
            << xml::attribute( "side", it->first->GetID() )
            << xml::attribute( "diplomacy", diplomacyConverter_.RevertConvert(it->second) )
        << xml::end();
    }
    xos << xml::end();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

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
    objectFactory.CreateObject( xis, *this );
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
// Name: MIL_Army::ReadDiplomacy
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void MIL_Army::ReadDiplomacy( xml::xistream& xis )
{
    unsigned int        nTeam;
    std::string strDiplomacy;

    xis >> xml::attribute( "side", nTeam )
        >> xml::attribute( "diplomacy", strDiplomacy );

    E_Diplomacy nDiplomacy = diplomacyConverter_.Convert( strDiplomacy );
    if( nDiplomacy == eUnknown )
        xis.error( "Unknown diplomacy relation between armies" );

    MIL_Army_ABC* pArmy = armyFactory_.Find( nTeam );
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
// Created: ABL 2007-07-09
// LTO
// -----------------------------------------------------------------------------
void MIL_Army::ReadLogistic( xml::xistream& xis, KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
{
    knowledgeGroupFactory.Create( xis, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::ReadAutomat
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void MIL_Army::ReadAutomat( xml::xistream& xis, AutomateFactory_ABC& automateFactory )
{
    unsigned int id;
    xis >> xml::attribute( "id", id );

    MIL_Automate* pSuperior = automateFactory.Find( id );
    if( !pSuperior )
        xis.error( "Unknown automat" );
    if( pSuperior->GetArmy() != *this )
        xis.error( "Invalid automat (not in specified side)" );
    if( !pSuperior->GetType().IsLogistic() )
        xis.error( "Automat isn't a logistic automat" );

    xis >> xml::list( "subordinate" , *this, &MIL_Army::ReadSubordinate, automateFactory, pSuperior );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::ReadSubordinate
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void MIL_Army::ReadSubordinate( xml::xistream& xis, AutomateFactory_ABC& automateFactory, MIL_Automate* pSuperior )
{
    unsigned int        nSubordinateID;
    std::string strLink;
    xis >> xml::attribute( "id", nSubordinateID );

    MIL_Automate* pSubordinate = automateFactory.Find( nSubordinateID );
    if( !pSubordinate )
        xis.error( "Unknown automat" );
    if( pSubordinate->GetArmy() != *this )
        xis.error( "Invalid automat (not in specified side)" );

    pSubordinate->ReadLogisticLink( static_cast< MIL_AutomateLOG& >( *pSuperior ), xis );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Army::RegisterKnowledgeGroup
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Army::RegisterKnowledgeGroup( MIL_KnowledgeGroup& knowledgeGroup )
{
    if( ! knowledgeGroups_.insert( std::make_pair( knowledgeGroup.GetID(), &knowledgeGroup ) ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::UnregisterKnowledgeGroup
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Army::UnregisterKnowledgeGroup( MIL_KnowledgeGroup& knowledgeGroup )
{
    if( knowledgeGroups_.erase( knowledgeGroup.GetID() ) != 1 )
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
        if( itKnowledgeGroup->second->IsPerceived( knowledge ) )
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

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Army::SendCreation
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_Army::SendCreation() const
{
    client::TeamCreation asn;
    asn().set_oid( nID_ );
    asn().set_nom( strName_.c_str() );
    asn().set_type( Common::EnumDiplomacy( nType_ ) );
    asn.Send( NET_Publisher_ABC::Publisher() );

    for( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        it->second->SendCreation();

    tools::Resolver< MIL_Formation >::Apply( boost::bind( &MIL_Formation::SendCreation, _1 ) );
    tools::Resolver< MIL_Object_ABC >::Apply( boost::bind( &MIL_Object_ABC::SendCreation, _1 ) ); 
    tools::Resolver< MIL_Population >::Apply( boost::bind( &MIL_Population::SendCreation, _1 ) );
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
        asn().set_oid_camp1( nID_ );
        asn().set_oid_camp2( it->first->GetID() );
        asn().set_diplomatie( Common::EnumDiplomacy( it->second ) );
        asn.Send( NET_Publisher_ABC::Publisher() );
    }

    for( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        it->second->SendFullState();

    tools::Resolver< MIL_Formation >::Apply( boost::bind( &MIL_Formation::SendFullState, _1 ) );
    tools::Resolver< MIL_Object_ABC >::Apply( boost::bind( &MIL_Object_ABC::SendFullState, _1 ) );
    tools::Resolver< MIL_Population >::Apply( boost::bind( &MIL_Population::SendFullState, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::SendKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_Army::SendKnowledge() const
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->SendFullState();
    for( CIT_KnowledgeGroupMap itKnowledgeGroup = knowledgeGroups_.begin(); itKnowledgeGroup != knowledgeGroups_.end(); ++itKnowledgeGroup )
        itKnowledgeGroup->second->SendKnowledge();
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::OnReceiveMsgChangeDiplomacy
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_Army::OnReceiveMsgChangeDiplomacy( const Common::MsgChangeDiplomacy& asnMsg )
{
    MIL_Army_ABC* pArmy2 = armyFactory_.Find( asnMsg.oid_camp2() );
    if( !pArmy2 || *pArmy2 == *this )
        throw NET_AsnException< MsgsSimToClient::MsgChangeDiplomacyAck_EnumChangeDiplomacyErrorCode >( MsgsSimToClient::MsgChangeDiplomacyAck_EnumChangeDiplomacyErrorCode_error_invalid_camp_diplomacy );
    E_Diplomacy nDiplomacy = eUnknown;
    if( asnMsg.diplomatie() == Common::unknown_diplo )
        nDiplomacy = eUnknown;
    else if( asnMsg.diplomatie() == Common::friend_diplo )
        nDiplomacy = eFriend;
    else if( asnMsg.diplomatie() == Common::enemy_diplo )
        nDiplomacy = eEnemy;
    else if( asnMsg.diplomatie() == Common::neutral_diplo )
        nDiplomacy = eNeutral;
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
    {
        fct.visit( *(it->second ) );
    }
}
