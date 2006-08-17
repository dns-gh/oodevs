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
#include "MIL_EntityManager.h"
#include "Network/NET_ASN_Messages.h"

MT_Converter< std::string, MIL_Army::E_Diplomacy > MIL_Army::relationConverter_( eUnknown );

BOOST_CLASS_EXPORT_GUID( MIL_Army, "MIL_Army" )

// -----------------------------------------------------------------------------
// Name: MIL_Army::Initialize
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_Army::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing armies relations" );
    relationConverter_.Register( "Ennemi", eEnemy   );
    relationConverter_.Register( "Ami"   , eFriend  );
    relationConverter_.Register( "Neutre", eNeutral );
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::Terminate
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_Army::Terminate()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_Army constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Army::MIL_Army( const std::string& strName, uint nID, MIL_InputArchive& archive )
    : strName_             ( strName )
    , nID_                 ( nID )
    , pKnowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_Army( *this ) )
{
    archive.BeginList( "GroupesConnaissance" );

    while( archive.NextListElement() )
    {
        archive.Section( "GroupeConnaissance" );

        uint        nID;
        std::string strType;
        archive.ReadAttribute( "id"  , nID     );
        archive.ReadAttribute( "type", strType );

        const MIL_KnowledgeGroupType* pType = MIL_KnowledgeGroupType::FindType( strType );
        if( !pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Knowledge group type doesn't exist", archive.GetContext() );

        MIL_KnowledgeGroup*& pKnowledgeGroup = knowledgeGroups_[ nID ];
        if( pKnowledgeGroup )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Knowledge group id already defined", archive.GetContext() );

        pKnowledgeGroup = &pType->InstanciateKnowledgeGroup( nID, *this, archive );

        archive.EndSection(); // GroupeConnaissance
    }

    archive.EndList(); // GroupesConnaissance
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
        void serialize( Archive& file, MIL_Army::T_ArmyRelationMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const MIL_Army::T_ArmyRelationMap& map, const uint )
        {
            file << map.size();
            for ( MIL_Army::CIT_ArmyRelationMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first
                     << it->second;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, MIL_Army::T_ArmyRelationMap& map, const uint )
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
void MIL_Army::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> const_cast< std::string& >( strName_ )
         >> const_cast< uint& >( nID_ )
         >> knowledgeGroups_
         >> relations_
         >> pKnowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_Army::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << strName_
         << nID_
         << knowledgeGroups_
         << relations_
         << pKnowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_Army::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "Armee" );
    archive.WriteAttribute( "id" , nID_ );
    archive.WriteAttribute( "nom", strName_ );
    
    archive.Section( "GroupesConnaissance" );
    for( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        it->second->WriteODB( archive );
    archive.EndSection(); // GroupesConnaissance

    archive.Section( "Diplomatie" );
    for( CIT_ArmyRelationMap it = relations_.begin(); it != relations_.end(); ++it )
    {
        archive.Section( "Armee" );
        archive.WriteAttribute( "nom", it->first->GetName() );
        archive.WriteAttribute( "relation", relationConverter_.RevertConvert( it->second ) );
        archive.EndSection(); // Armee
    }
    archive.EndSection(); // Diplomatie 
    
    archive.EndSection(); // Armee
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Army::InitializeDiplomacy
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_Army::InitializeDiplomacy( const MIL_EntityManager& entityManager, MIL_InputArchive& archive )
{
    archive.BeginList( "Diplomatie" );

    while( archive.NextListElement() )
    {
        archive.Section( "Armee" );

        std::string strName;
        std::string strRelation;

        archive.ReadAttribute( "nom", strName );
        archive.ReadAttribute( "relation", strRelation );

        E_Diplomacy nRelation = relationConverter_.Convert( strRelation );
        if( nRelation == eUnknown )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown diplomacy relation between armies", archive.GetContext() );

        MIL_Army* pArmy = entityManager.FindArmy( strName );
        if( !pArmy )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown army name", archive.GetContext() );

        if( relations_.find( pArmy ) != relations_.end() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Relation between armies already exist", archive.GetContext() );
        if( pArmy == this )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Self relation not allowed", archive.GetContext() );

        relations_[ pArmy ] = nRelation;

        archive.EndSection(); // Armee
    }
    archive.EndList(); // Diplomatie
}

// =============================================================================
// OPERATIONS
// =============================================================================

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
    E_Diplomacy nRelation = GetRelationWith( army );
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
    E_Diplomacy nRelation = GetRelationWith( army );
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
    E_Diplomacy nRelation = GetRelationWith( army );
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
void MIL_Army::SendCreation()
{
    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();

    DIN::DIN_BufferedMessage msg = msgMgr.BuildMessage();
    msg << (uint32)nID_;
    msg << strName_;
    msgMgr.SendMsgArmy( msg );

    for( CIT_KnowledgeGroupMap itKnowledgeGroup = knowledgeGroups_.begin(); itKnowledgeGroup != knowledgeGroups_.end(); ++itKnowledgeGroup )
        itKnowledgeGroup->second->SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::SendFullState
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MIL_Army::SendFullState()
{
    for( CIT_ArmyRelationMap it = relations_.begin(); it != relations_.end(); ++it )
    {
        NET_ASN_MsgChangeDiplomatie asn;

        asn.GetAsnMsg().oid_camp1  = nID_;
        asn.GetAsnMsg().oid_camp2  = it->first->GetID();
        asn.GetAsnMsg().diplomatie = (ASN1T_EnumDiplomatie)( it->second );

        asn.Send();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::SendKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_Army::SendKnowledge()
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
        relations_[ pArmy2 ] = nDiplomacy;
    }
    asnReplyMsg.Send( nCtx );
}
