//*****************************************************************************
//
// $Created: NLD 2003-01-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/MIL_Fuseau.cpp $
// $Author: Jvt $
// $Modtime: 16/05/05 14:55 $
// $Revision: 17 $
// $Workfile: MIL_Fuseau.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_TacticalLineManager.h"

#include "MIL_Limit.h"
#include "MIL_Lima.h"
#include "TER_LimitData.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager constructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
MIL_TacticalLineManager::MIL_TacticalLineManager()
    : limits_    ()
    , limas_     ()
    , limitsData_()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager destructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
MIL_TacticalLineManager::~MIL_TacticalLineManager()
{
    for( CIT_LimitMap it = limits_.begin(); it != limits_.end(); ++it )
        delete it->second;
    for( CIT_LimaMap it = limas_.begin(); it != limas_.end(); ++it )
        delete it->second;
    for( CIT_LimitDataMap it = limitsData_.begin(); it != limitsData_.end(); ++it )
        delete it->second;
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::load
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
void MIL_TacticalLineManager::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> limits_
         >> limas_;
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::save
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
void MIL_TacticalLineManager::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << limits_
         << limas_;
}

// =============================================================================
// LIMIT DATA
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::CreateLimit
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void MIL_TacticalLineManager::CreateLimit( const MIL_Formation& formation, MIL_InputArchive& archive )
{
    while( archive.NextListElement() )
    {
        MIL_Limit* pLimit = new MIL_Limit( formation, archive );
        bool bOut = limits_.insert( std::make_pair( pLimit->GetID(), pLimit ) ).second;
        assert( bOut );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::CreateLimit
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void MIL_TacticalLineManager::CreateLimit( const MIL_Automate& automate, MIL_InputArchive& archive )
{
    while( archive.NextListElement() )
    {
        MIL_Limit* pLimit = new MIL_Limit( automate, archive );
        bool bOut = limits_.insert( std::make_pair( pLimit->GetID(), pLimit ) ).second;
        assert( bOut );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::CreateLima
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void MIL_TacticalLineManager::CreateLima( const MIL_Formation& formation, MIL_InputArchive& archive )
{
    while( archive.NextListElement() )
    {
        MIL_Lima* pLima = new MIL_Lima( formation, archive );
        bool bOut = limas_.insert( std::make_pair( pLima->GetID(), pLima ) ).second;
        assert( bOut );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::CreateLima
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void MIL_TacticalLineManager::CreateLima( const MIL_Automate& automate, MIL_InputArchive& archive )
{
    while( archive.NextListElement() )
    {
        MIL_Lima* pLima = new MIL_Lima( automate, archive );
        bool bOut = limas_.insert( std::make_pair( pLima->GetID(), pLima ) ).second;
        assert( bOut );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::CreateLimitData
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
const TER_LimitData& MIL_TacticalLineManager::CreateLimitData( const T_PointVector& points )
{
    TER_LimitData*& pData = limitsData_[ points ];
    if( !pData )
        pData = new TER_LimitData( points );
    return *pData;
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::DestroyLimitData
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
void MIL_TacticalLineManager::DestroyLimitData( const TER_LimitData& data )
{
    int nOut = limitsData_.erase( data.GetPoints() );
    assert( nOut == 1 );
    delete &data;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::OnReceiveMsgLimitCreationRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void MIL_TacticalLineManager::OnReceiveMsgLimitCreationRequest( const ASN1T_MsgLimitCreationRequest& asn, uint nCtx )
{
    NET_ASN_MsgLimitCreationRequestAck ack;
    ack() = EnumInfoContextErrorCode::no_error;
    try
    {
        MIL_Limit* pLimit = new MIL_Limit( asn );
        bool bOut = limits_.insert( std::make_pair( pLimit->GetID(), pLimit ) ).second;
        assert( bOut );
    }
    catch( NET_AsnException< ASN1T_EnumInfoContextErrorCode >& e )
    {
        ack() = e.GetErrorID();
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::OnReceiveMsgLimitUpdateRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void MIL_TacticalLineManager::OnReceiveMsgLimitUpdateRequest( const ASN1T_MsgLimitUpdateRequest& asn, uint nCtx )
{
    NET_ASN_MsgLimitUpdateRequestAck ack;
    ack() = EnumInfoContextErrorCode::no_error;

    CIT_LimitMap it = limits_.find( asn.oid );
    if( it == limits_.end() )
        ack() = EnumInfoContextErrorCode::error_invalid_id;
    else
    {
        try
        {
            it->second->Update( asn );
        }
        catch( NET_AsnException< ASN1T_EnumInfoContextErrorCode >& e )
        {
            ack() = e.GetErrorID();
        }
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::OnReceiveMsgLimitDestructionRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void MIL_TacticalLineManager::OnReceiveMsgLimitDestructionRequest( const ASN1T_MsgLimitDestructionRequest& asn, uint nCtx )
{
    NET_ASN_MsgLimitDestructionRequestAck ack;
    ack() = EnumInfoContextErrorCode::no_error;

    IT_LimitMap it = limits_.find( asn );
    if( it == limits_.end() )
        ack() = EnumInfoContextErrorCode::error_invalid_id;
    else
    {
        delete it->second;
        limits_.erase( it );
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::OnReceiveMsgLimaCreationRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void MIL_TacticalLineManager::OnReceiveMsgLimaCreationRequest( const ASN1T_MsgLimaCreationRequest& asn, uint nCtx )
{
    NET_ASN_MsgLimaCreationRequestAck ack;
    ack() = EnumInfoContextErrorCode::no_error;
    try
    {
        MIL_Lima* pLima = new MIL_Lima( asn );
        bool bOut = limas_.insert( std::make_pair( pLima->GetID(), pLima ) ).second;
        assert( bOut );
    }
    catch( NET_AsnException< ASN1T_EnumInfoContextErrorCode >& e )
    {
        ack() = e.GetErrorID();
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::OnReceiveMsgLimaUpdateRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void MIL_TacticalLineManager::OnReceiveMsgLimaUpdateRequest( const ASN1T_MsgLimaUpdateRequest& asn, uint nCtx )
{
    NET_ASN_MsgLimaUpdateRequestAck ack;
    ack() = EnumInfoContextErrorCode::no_error;

    CIT_LimaMap it = limas_.find( asn.oid );
    if( it == limas_.end() )
        ack() = EnumInfoContextErrorCode::error_invalid_id;
    else
    {
        try
        {
            it->second->Update( asn );
        }
        catch( NET_AsnException< ASN1T_EnumInfoContextErrorCode >& e )
        {
            ack() = e.GetErrorID();
        }
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::OnReceiveMsgLimaDestructionRequest
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void MIL_TacticalLineManager::OnReceiveMsgLimaDestructionRequest( const ASN1T_MsgLimaDestructionRequest& asn, uint nCtx )
{
    NET_ASN_MsgLimaDestructionRequestAck ack;
    ack() = EnumInfoContextErrorCode::no_error;

    IT_LimaMap it = limas_.find( asn );
    if( it == limas_.end() )
        ack() = EnumInfoContextErrorCode::error_invalid_id;
    else
    {
        delete it->second;
        limas_.erase( it );
    }
    ack.Send( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_TacticalLineManager::SendStateToNewClient
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void MIL_TacticalLineManager::SendStateToNewClient() const
{
    for( CIT_LimitMap it = limits_.begin(); it != limits_.end(); ++it )
        it->second->SendFullState();
    for( CIT_LimaMap it = limas_.begin(); it != limas_.end(); ++it )
        it->second->SendFullState();
}
        
