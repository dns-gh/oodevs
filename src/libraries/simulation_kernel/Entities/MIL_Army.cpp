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
#include "Knowledge/MIL_KnowledgeGroupType.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "MIL_Formation.h"
#include "MIL_EntityManager.h"
#include "Network/NET_ASN_Messages.h"
#include "Tools/MIL_IDManager.h"

MT_Converter< std::string, MIL_Army::E_Diplomacy > MIL_Army::diplomacyConverter_( eUnknown );

BOOST_CLASS_EXPORT_GUID( MIL_Army, "MIL_Army" )

// -----------------------------------------------------------------------------
// Name: MIL_Army::Initialize
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_Army::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing armies diplomacies" );
    
    diplomacyConverter_.Register( "enemy"  , eEnemy   );
    diplomacyConverter_.Register( "friend" , eFriend  );
    diplomacyConverter_.Register( "neutral", eNeutral );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::Terminate
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_Army::Terminate()
{
}

// =============================================================================
// 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Army constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Army::MIL_Army( uint nID, MIL_InputArchive& archive )
    : nID_                 ( nID )
    , strName_             ()
    , pKnowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_Army( *this ) )
    , knowledgeGroups_     ()
    , diplomacies_         ()
    , formations_          ()
    , populations_         ()
    , objects_             ()
{
    archive.ReadAttribute( "name", strName_ );
   
    InitializeCommunication( archive );
    InitializeTactical     ( archive );
    InitializeObjects      ( archive );
    InitializeLogistic     ( archive );
    InitializePopulations  ( archive );    
}

// -----------------------------------------------------------------------------
// Name: MIL_Army constructor
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
MIL_Army::MIL_Army()
    : strName_()
    , nID_    ( (uint)-1 )
{
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
        template< typename Archive >
        inline
        void serialize( Archive& file, MIL_Army::T_DiplomacyMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const MIL_Army::T_DiplomacyMap& map, const uint )
        {
            file << map.size();
            for ( MIL_Army::CIT_DiplomacyMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first
                     << it->second;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, MIL_Army::T_DiplomacyMap& map, const uint )
        {
            uint nNbr;
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
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
template< typename Archive > 
void MIL_Army::serialize( Archive& file, const uint )
{
    file & const_cast< std::string& >( strName_ )
         & const_cast< uint& >( nID_ )
         & diplomacies_
         & knowledgeGroups_
         & formations_
         & objects_
         & populations_
         & pKnowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_Army::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "side" );
    archive.WriteAttribute( "id"  , nID_ );
    archive.WriteAttribute( "name", strName_ );

    archive.Section( "communication" );
    for( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        it->second->WriteODB( archive );
    archive.EndSection(); // communication

    archive.Section( "tactical" );
    for( CIT_FormationSet it = formations_.begin(); it != formations_.end(); ++it )
        (**it).WriteODB( archive );
    archive.EndSection(); // tactical

    archive.Section( "logistic" );
    for( CIT_FormationSet it = formations_.begin(); it != formations_.end(); ++it )
        (**it).WriteLogisticLinksODB( archive );
    archive.EndSection(); // logistic

    archive.Section( "objects" );
    for( CIT_ObjectSet it = objects_.begin(); it != objects_.end(); ++it )
        (**it).WriteODB( archive );    
    archive.EndSection(); // objects

    archive.Section( "populations" );
    for( CIT_PopulationSet it = populations_.begin(); it != populations_.end(); ++it )
        (**it).WriteODB( archive );
    archive.EndSection(); // populations

    archive.EndSection(); // side
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::WriteDiplomacyODB
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Army::WriteDiplomacyODB( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "side" );    
    archive.WriteAttribute( "id", nID_ );
    for( CIT_DiplomacyMap it = diplomacies_.begin(); it != diplomacies_.end(); ++it )
    {
        archive.Section( "relationship" );
        archive.WriteAttribute( "side"     , it->first->GetID() );
        archive.WriteAttribute( "diplomacy", diplomacyConverter_.RevertConvert( it->second ) );
        archive.EndSection(); // relationship
    }
    archive.EndSection(); // side
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Army::InitializeDiplomacy
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_Army::InitializeDiplomacy( MIL_InputArchive& archive )
{
    while( archive.NextListElement() )
    {
        archive.Section( "relationship" );

        uint        nTeam;
        std::string strDiplomacy;

        archive.ReadAttribute( "side"     , nTeam );
        archive.ReadAttribute( "diplomacy", strDiplomacy );

        E_Diplomacy nDiplomacy = diplomacyConverter_.Convert( strDiplomacy );
        if( nDiplomacy == eUnknown )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown diplomacy relation between armies", archive.GetContext() );

        MIL_Army* pArmy = MIL_AgentServer::GetWorkspace().GetEntityManager().FindArmy( nTeam );
        if( !pArmy )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown army", archive.GetContext() );

        if( diplomacies_.find( pArmy ) != diplomacies_.end() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Diplomacy between armies already exist", archive.GetContext() );
        if( pArmy == this )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Self diplomacy not allowed", archive.GetContext() );

        diplomacies_[ pArmy ] = nDiplomacy;

        archive.EndSection(); // relationship
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::InitializeCommunication
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
void MIL_Army::InitializeCommunication( MIL_InputArchive& archive )
{
    archive.BeginList( "communication" );
    while( archive.NextListElement() )
    {
        archive.Section( "knowledge-group" );

        uint        nID;
        std::string strType;
        archive.ReadAttribute( "id"  , nID     );
        archive.ReadAttribute( "type", strType );

        const MIL_KnowledgeGroupType* pType = MIL_KnowledgeGroupType::FindType( strType );
        if( !pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Knowledge group type doesn't exist", archive.GetContext() );

        if( knowledgeGroups_.find( nID ) != knowledgeGroups_.end() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Knowledge group id already defined", archive.GetContext() );
        pType->InstanciateKnowledgeGroup( nID, *this, archive ); // Auto-registration

        archive.EndSection(); // knowledge-group
    }
    archive.EndList(); // communication
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::InitializeTactical
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
void MIL_Army::InitializeTactical( MIL_InputArchive& archive )
{
    archive.BeginList( "tactical" );
    while( archive.NextListElement() )
    {
        if( archive.BeginList( "formation" ) )
        {
            uint nID;
            archive.ReadAttribute( "id", nID );
            MIL_AgentServer::GetWorkspace().GetEntityManager().CreateFormation( nID, *this, archive ); // Auto-registration
            archive.EndList(); // formation
        }
    }
    archive.EndList(); // tactical
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::InitializeObjects
// Created: NLD 2006-10-20
// -----------------------------------------------------------------------------
void MIL_Army::InitializeObjects( MIL_InputArchive& archive )
{
    archive.BeginList( "objects" );
    while( archive.NextListElement() )
    {
        if( archive.Section( "object" ) )
        {
            uint        nID;
            std::string strType;
            archive.ReadAttribute( "id"  , nID );
            archive.ReadAttribute( "type", strType );

            const MIL_RealObjectType* pType = MIL_RealObjectType::Find( strType );
            if( !pType )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown object type", archive.GetContext() );

            MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( *pType, nID, *this, archive ); // Auto-registration
            archive.EndSection(); // object
        }
    }
    archive.EndList(); // objects
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::InitializeLogistic
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
void MIL_Army::InitializeLogistic( MIL_InputArchive& archive )
{
    archive.BeginList( "logistic" );
    while( archive.NextListElement() )
    {
        archive.BeginList( "automat" );

        uint nID;
        archive.ReadAttribute( "id", nID );
    
        MIL_Automate* pSuperior = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( nID );
        if( !pSuperior )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown automat", archive.GetContext() );
        if( pSuperior->GetArmy() != *this )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Invalid automat (not in specified side)", archive.GetContext() );
        if( !pSuperior->GetType().IsLogistic() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automat isn't a logistic automat", archive.GetContext() );

        while( archive.NextListElement() )
        {
            archive.Section( "subordinate" );
            
            uint        nSubordinateID;
            std::string strLink;
            archive.ReadAttribute( "automat", nSubordinateID );

            MIL_Automate* pSubordinate = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( nSubordinateID );
            if( !pSubordinate )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown automat", archive.GetContext() );
            if( pSubordinate->GetArmy() != *this )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Invalid automat (not in specified side)", archive.GetContext() );
    
            pSubordinate->ReadLogisticLink( static_cast< MIL_AutomateLOG& >( *pSuperior ), archive );

            archive.EndSection(); // subordinate
        }

        archive.EndList(); // automat
    }
    archive.EndList(); // logistic
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::InitializePopulations
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_Army::InitializePopulations( MIL_InputArchive& archive )
{
    archive.BeginList( "populations" );
    while( archive.NextListElement() )
    {
        archive.Section( "population" );

        uint        nID;
        std::string strType;

        archive.ReadAttribute( "id"  , nID     );
        archive.ReadAttribute( "type", strType );

        const MIL_PopulationType* pPopulationType = MIL_PopulationType::Find( strType );
        if( !pPopulationType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown population type", archive.GetContext() );

        MIL_AgentServer::GetWorkspace().GetEntityManager().CreatePopulation( *pPopulationType, MIL_IDManager::populations_.ConvertSimIDToMosID( nID ), *this, archive ); // Auto-registration
        archive.EndSection(); // population
    }
    archive.EndList(); // populations
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
    bool bOut = knowledgeGroups_.insert( std::make_pair( knowledgeGroup.GetID(), &knowledgeGroup ) ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::UnregisterKnowledgeGroup
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Army::UnregisterKnowledgeGroup( MIL_KnowledgeGroup& knowledgeGroup )
{
    int nOut = knowledgeGroups_.erase( knowledgeGroup.GetID() );
    assert( nOut == 1 );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::UpdateKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_Army::UpdateKnowledges()
{
    for( CIT_KnowledgeGroupMap itKnowledgeGroup = knowledgeGroups_.begin(); itKnowledgeGroup != knowledgeGroups_.end(); ++itKnowledgeGroup )
        itKnowledgeGroup->second->UpdateKnowledges();
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->Update();
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
E_Tristate MIL_Army::IsAnEnemy( const MIL_Army& army ) const
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
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
E_Tristate MIL_Army::IsAnEnemy( const DEC_Knowledge_Agent& knowledge ) const
{
    const MIL_Army* pArmy = knowledge.GetArmy();
    if( !pArmy )
        return eTristate_DontKnow;

    return IsAnEnemy( *pArmy );
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
E_Tristate MIL_Army::IsAFriend( const MIL_Army& army ) const
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
    const MIL_Army* pArmy = knowledge.GetArmy();
    if( !pArmy )
        return eTristate_DontKnow;

    return IsAFriend( *pArmy );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::IsNeutral
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
E_Tristate MIL_Army::IsNeutral( const MIL_Army& army ) const
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
    NET_ASN_MsgSideCreation asn;
    asn.GetAsnMsg().oid = nID_;
    asn.GetAsnMsg().nom = strName_.c_str();
    asn.Send();

    for( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        it->second->SendCreation();

    for( CIT_FormationSet it = formations_.begin(); it != formations_.end(); ++it )
        (**it).SendCreation();

    for( CIT_ObjectSet it = objects_.begin(); it != objects_.end(); ++it )
        (**it).SendCreation();

    for( CIT_PopulationSet it = populations_.begin(); it != populations_.end(); ++it )
        (**it).SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::SendFullState
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MIL_Army::SendFullState() const
{
    for( CIT_DiplomacyMap it = diplomacies_.begin(); it != diplomacies_.end(); ++it )
    {
        NET_ASN_MsgChangeDiplomatie asn;
        asn.GetAsnMsg().oid_camp1  = nID_;
        asn.GetAsnMsg().oid_camp2  = it->first->GetID();
        asn.GetAsnMsg().diplomatie = (ASN1T_EnumDiplomatie)( it->second );
        asn.Send();
    }

    for( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        it->second->SendFullState();

    for( CIT_FormationSet it = formations_.begin(); it != formations_.end(); ++it )
        (**it).SendFullState();

    for( CIT_ObjectSet it = objects_.begin(); it != objects_.end(); ++it )
        (**it).SendFullState();

    for( CIT_PopulationSet it = populations_.begin(); it != populations_.end(); ++it )
        (**it).SendFullState();
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
void MIL_Army::OnReceiveMsgChangeDiplomacy( ASN1T_MsgChangeDiplomatie& asnMsg, MIL_MOSContextID nCtx )
{
    NET_ASN_MsgChangeDiplomatieAck asnReplyMsg;
    asnReplyMsg.GetAsnMsg().oid_camp1   = asnMsg.oid_camp1;
    asnReplyMsg.GetAsnMsg().oid_camp2   = asnMsg.oid_camp2;
    asnReplyMsg.GetAsnMsg().diplomatie  = asnMsg.diplomatie;
    asnReplyMsg.GetAsnMsg().error_code  = EnumChangeDiplomatieErrorCode::no_error;

    MIL_Army* pArmy2 = MIL_AgentServer::GetWorkspace().GetEntityManager().FindArmy( asnMsg.oid_camp2 );
    if( !pArmy2 || *pArmy2 == *this )
        asnReplyMsg.GetAsnMsg().error_code  = EnumChangeDiplomatieErrorCode::error_invalid_camp;
    else
    {
        E_Diplomacy nDiplomacy = eUnknown;
        switch( asnMsg.diplomatie )
        {
            case EnumDiplomatie::inconnu: nDiplomacy = eUnknown; break;
            case EnumDiplomatie::ami    : nDiplomacy = eFriend;  break;
            case EnumDiplomatie::ennemi : nDiplomacy = eEnemy;   break;
            case EnumDiplomatie::neutre : nDiplomacy = eNeutral; break;
        }
        diplomacies_[ pArmy2 ] = nDiplomacy;
    }
    asnReplyMsg.Send( nCtx );
}
