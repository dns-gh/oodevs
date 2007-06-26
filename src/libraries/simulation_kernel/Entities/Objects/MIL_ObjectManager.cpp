// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ObjectManager.cpp $
// $Author: Nld $
// $Modtime: 10/06/05 10:18 $
// $Revision: 9 $
// $Workfile: MIL_ObjectManager.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ObjectManager.h"
#include "MIL_RealObjectType.h"
#include "MIL_RealObject_ABC.h"
#include "MIL_VirtualObject_ABC.h"
#include "MIL_NuageNBC.h"
#include "MIL_ZoneMineeParDispersion.h"
#include "MIL_ControlZone.h"
#include "Network/NET_ASN_Messages.h"
#include "Hla/HLA_Federate.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Tools/MIL_IDManager.h"

BOOST_CLASS_EXPORT_GUID( MIL_ObjectManager, "MIL_ObjectManager" )

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_ObjectManager::MIL_ObjectManager()
    : realObjects_   ()
    , virtualObjects_()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_ObjectManager::~MIL_ObjectManager()
{
    for( CIT_RealObjectMap itReal = realObjects_.begin(); itReal != realObjects_.end(); ++itReal )
        delete itReal->second;

    for( CIT_VirtualObjectVector itVirtual = virtualObjects_.begin(); itVirtual != virtualObjects_.end(); ++itVirtual )
        delete *itVirtual;
}


// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_ObjectManager::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> realObjects_;
         // >> virtualObjects_; // les objets virtuels sont créés par le décisionnel   
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_ObjectManager::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << realObjects_;
         // << virtualObjects_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::ProcessEvents
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_ObjectManager::ProcessEvents()
{
    for( CIT_RealObjectMap itRealObject = realObjects_.begin(); itRealObject != realObjects_.end(); ++itRealObject )
        itRealObject->second->ProcessEvents();

    for( CIT_VirtualObjectVector itVirtualObject = virtualObjects_.begin(); itVirtualObject != virtualObjects_.end(); ++itVirtualObject )
        (**itVirtualObject).ProcessEvents();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::UpdateStates
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_ObjectManager::UpdateStates()
{
    // Real objects
    for( IT_RealObjectMap itRealObject = realObjects_.begin(); itRealObject != realObjects_.end();  )
    {
        MIL_RealObject_ABC& object = *itRealObject->second;
        if( object.IsReadyForDeletion() )
        {
            object.SendDestruction();
            delete &object;
            itRealObject = realObjects_.erase( itRealObject );
        }
        else
        {
            object.UpdateState();   
            ++itRealObject;
        }
    }

    // Virtual objects
    for( IT_VirtualObjectVector itVirtualObject = virtualObjects_.begin(); itVirtualObject != virtualObjects_.end();  )
    {
        MIL_VirtualObject_ABC& object = **itVirtualObject;
        if( object.IsReadyForDeletion() )
        {
            delete &object;
            itVirtualObject = virtualObjects_.erase( itVirtualObject );
        }
        else
        {
            object.UpdateState();   
            ++itVirtualObject;
        }
    }
}

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::RegisterObject
// Created: NLD 2004-10-27
// -----------------------------------------------------------------------------
void MIL_ObjectManager::RegisterObject( MIL_VirtualObject_ABC& object )
{
    assert( std::find( virtualObjects_.begin(), virtualObjects_.end(), &object ) == virtualObjects_.end() );
    virtualObjects_.push_back( &object );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::RegisterObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void MIL_ObjectManager::RegisterObject( MIL_RealObject_ABC& object )
{
    if( MIL_AgentServer::GetWorkspace().GetHLAFederate() )
        MIL_AgentServer::GetWorkspace().GetHLAFederate()->Register( object );
    bool bOut = realObjects_.insert( std::make_pair( object.GetID(), &object ) ).second;
    assert( bOut );
    object.SendCreation(); //$$$ a déplacer ...
    object.GetArmy().GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddEphemeralObjectKnowledge( object ); //$$$ A CHANGER DE PLACE QUAND REFACTOR OBJETS -- NB : ne doit pas être fait dans RealObject::InitializeCommon <= crash dans connaissance, si initialisation objet failed
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_RealObject_ABC& MIL_ObjectManager::CreateObject( const MIL_RealObjectType& type, uint nID, MIL_Army& army, MIL_InputArchive& archive )
{
    nID = type.GetIDManager().ConvertSimIDToMosID( nID );
    if( realObjects_.find( nID ) != realObjects_.end() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "ID already exists", archive.GetContext() );

    if( type.GetIDManager().IsMosIDValid( nID ) )
    {
        if( !type.GetIDManager().LockMosID( nID ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "ID already used", archive.GetContext() );
    }

    MIL_RealObject_ABC& object = type.InstanciateObject( nID, army );
    object.Initialize( archive );

    // Default state : full constructed, valorized if it can be, not prepared
    object.Construct();
    object.Mine     ();

    RegisterObject( object );
    return object;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_ObjectManager::CreateObject( const ASN1T_MagicActionCreateObject& asn )
{
    const MIL_RealObjectType* pType = MIL_RealObjectType::Find( asn.type );
    if( !pType )
        return EnumObjectErrorCode::error_invalid_object;

    MIL_Army* pArmy = MIL_AgentServer::GetWorkspace().GetEntityManager().FindArmy( asn.team );
    if( !pArmy )
        return EnumObjectErrorCode::error_invalid_camp;

    MIL_RealObject_ABC& object = pType->InstanciateObject( pType->GetIDManager().GetFreeSimID(), *pArmy );
    ASN1T_EnumObjectErrorCode nErrorCode = object.Initialize( asn );
    if( nErrorCode != EnumObjectErrorCode::no_error )
    {
        object.MarkForDestruction(); //$$$ naze, mais nécessaire
        delete &object;
        return nErrorCode;
    }

    // Default state : full constructed, valorized if it can be, not prepared
    object.Construct();
    object.Mine     ();

    RegisterObject( object );
    return EnumObjectErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
MIL_RealObject_ABC* MIL_ObjectManager::CreateObject( MIL_Army& army, const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint nCurrentParamIdx )
{
    uint nObjectTypeID = diaParameters[ nCurrentParamIdx++ ].ToId();
    const MIL_RealObjectType* pType = MIL_RealObjectType::Find( nObjectTypeID );
    if( !pType )
        return 0;

    MIL_RealObject_ABC& object = pType->InstanciateObject( pType->GetIDManager().GetFreeSimID(), army );
    if( !object.Initialize( obstacleType, diaParameters, nCurrentParamIdx ) )
    {
        object.MarkForDestruction(); //$$$ naze, mais nécessaire
        delete &object;
        return 0;
    }
    RegisterObject( object );
    return &object;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_RealObject_ABC* MIL_ObjectManager::CreateObject( const MIL_RealObjectType& type, MIL_Army& army, const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass )
{
    MIL_RealObject_ABC& object = type.InstanciateObject( type.GetIDManager().GetFreeSimID(), army );
    if( !object.Initialize( localisation, strOption, strExtra, rCompletion, rMining, rBypass ) )
    {
        object.MarkForDestruction(); //$$$ naze, mais nécessaire
        delete &object;
        return 0;
    }
    RegisterObject( object );
    return &object;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObjectNuageNBC
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_NuageNBC& MIL_ObjectManager::CreateObjectNuageNBC( MIL_Army& army, const TER_Localisation& localisation, const MIL_NbcAgentType& nbcAgentType )
{
    MIL_NuageNBC& object = *new MIL_NuageNBC( MIL_RealObjectType::nuageNBC_, MIL_RealObjectType::nuageNBC_.GetIDManager().GetFreeSimID(), army );
    object.Initialize( localisation, nbcAgentType );
    object.Construct();
    RegisterObject( object );
    return object;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObjectZoneeMineeParDispersion
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_ZoneMineeParDispersion& MIL_ObjectManager::CreateObjectZoneeMineeParDispersion( MIL_Army& army, const TER_Localisation& localisation, uint nNbrMines )
{
    MIL_ZoneMineeParDispersion& object = *new MIL_ZoneMineeParDispersion( MIL_RealObjectType::zoneMineeParDispersion_, MIL_RealObjectType::nuageNBC_.GetIDManager().GetFreeSimID(), army );
    object.Initialize( localisation, nNbrMines );
    object.Construct();
    RegisterObject( object );
    return object;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObjectControlZone
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_ControlZone& MIL_ObjectManager::CreateObjectControlZone( MIL_Army& army, const TER_Localisation& localisation, MT_Float rRadius )
{
    MIL_ControlZone& object = *new MIL_ControlZone( army, localisation, rRadius );
    RegisterObject( object );
    return object;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::OnReceiveMsgObjectMagicAction
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_ObjectManager::OnReceiveMsgObjectMagicAction( const ASN1T_MsgObjectMagicAction& asnMsg, uint nCtx )
{
    ASN1T_EnumObjectErrorCode nErrorCode = EnumObjectErrorCode::no_error;

    if( asnMsg.action.t == T_MsgObjectMagicAction_action_create_object )
        nErrorCode = CreateObject( *asnMsg.action.u.create_object );
    else if( asnMsg.action.t == T_MsgObjectMagicAction_action_destroy_object )
    {
        MIL_RealObject_ABC* pObject = FindRealObject( asnMsg.action.u.destroy_object );
        if( !pObject )
            nErrorCode = EnumObjectErrorCode::error_invalid_object;
        else
            pObject->Destroy();
    }
    else if( asnMsg.action.t == T_MsgObjectMagicAction_action_update_object )
    {
        MIL_RealObject_ABC* pObject = FindRealObject( asnMsg.action.u.update_object->oid );
        if( !pObject )
            nErrorCode = EnumObjectErrorCode::error_invalid_object;
        else
            nErrorCode = pObject->OnReceiveMagicActionUpdate( *asnMsg.action.u.update_object );
    }
    
    NET_ASN_MsgObjectMagicActionAck asnReplyMsg;
    asnReplyMsg().error_code = nErrorCode;
    asnReplyMsg.Send( nCtx );
}
