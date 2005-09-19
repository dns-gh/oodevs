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

#include "MIL_pch.h"

#include "MIL_ObjectManager.h"
#include "MIL_RealObjectType.h"
#include "MIL_RealObject_ABC.h"
#include "MIL_VirtualObject_ABC.h"
#include "Network/NET_ASN_Messages.h"
#include "Hla/HLA_Federate.h"

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

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::ReadODB
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_ObjectManager::ReadODB( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing objects" );

    if ( !archive.BeginList( "Objets", MIL_InputArchive::eNothing ) )
        return;

    while( archive.NextListElement() )
    {
        archive.Section( "Objet" );
        CreateObject( archive );
        archive.EndSection(); // Objet
    }
    archive.EndList(); // Objets
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
            object.SendMsgDestruction();
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
    object.SendMsgConstruction();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::CreateObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
MIL_RealObject_ABC* MIL_ObjectManager::CreateObject( MIL_Army& army, DIA_Parameters& diaParameters, uint nCurrentParamIdx )
{
    uint nObjectTypeID = diaParameters[ nCurrentParamIdx++ ].ToId();
    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::FindObjectType( nObjectTypeID );
    if( !pObjectType )
        return 0;

    MIL_RealObject_ABC& object = pObjectType->InstanciateObject();
    if( !object.Initialize( army, diaParameters, nCurrentParamIdx ) )
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
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_ObjectManager::CreateObject( uint nID, const ASN1T_MagicActionCreateObject& asn )
{
    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::FindObjectType( asn.type );
    if( !pObjectType )
        return EnumObjectErrorCode::error_invalid_object;

    MIL_RealObject_ABC& object = pObjectType->InstanciateObject();
    ASN1T_EnumObjectErrorCode nErrorCode = object.Initialize( nID, asn );
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
void MIL_ObjectManager::CreateObject( MIL_InputArchive& archive )
{
    std::string strType;
    uint        nID;
    
    archive.ReadAttribute( "id", nID );
    archive.ReadAttribute( "type", strType );

    const MIL_RealObjectType* pObjectType = MIL_RealObjectType::FindObjectType( strType );
    if( !pObjectType )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown object type", archive.GetContext() );

    MIL_RealObject_ABC& object = pObjectType->InstanciateObject();
    object.Initialize( nID, archive );

    // Default state : full constructed, valorized if it can be, not prepared
    object.Construct();
    object.Mine     ();

    RegisterObject( object );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::SendStateToNewClient
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_ObjectManager::SendStateToNewClient()
{
    for( CIT_RealObjectMap it = realObjects_.begin(); it != realObjects_.end(); ++it )
        it->second->SendStateToNewClient();
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::OnReceiveMsgObjectMagicAction
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_ObjectManager::OnReceiveMsgObjectMagicAction( ASN1T_MsgObjectMagicAction& asnMsg, MIL_MOSContextID nCtx )
{
    ASN1T_EnumObjectErrorCode nErrorCode = EnumObjectErrorCode::no_error;

    NET_ASN_MsgObjectMagicActionAck asnReplyMsg;
    asnReplyMsg.GetAsnMsg().oid = asnMsg.oid_objet;

    if( asnMsg.action.t == T_MsgObjectMagicAction_action_create_object )
        nErrorCode = CreateObject( asnMsg.oid_objet, *asnMsg.action.u.create_object );
    else
    {
        MIL_RealObject_ABC* pObject = FindRealObject( asnMsg.oid_objet );
        if( !pObject )
            nErrorCode = EnumObjectErrorCode::error_invalid_object;
        else
        {
            switch( asnMsg.action.t )
            {
                case T_MsgObjectMagicAction_action_destroy_object :              pObject->Destroy(); break;
                case T_MsgObjectMagicAction_action_update_object  : nErrorCode = pObject->OnReceiveMagicActionUpdate( *asnMsg.action.u.update_object ); break;
                default:
                    assert( false );
            }
        }
    }
    asnReplyMsg.GetAsnMsg().error_code = nErrorCode;
    asnReplyMsg.Send( nCtx );
}

