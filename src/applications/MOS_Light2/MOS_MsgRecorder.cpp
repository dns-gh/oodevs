// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-10-20 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MsgRecorder.cpp $
// $Author: Ape $
// $Modtime: 6/07/05 11:35 $
// $Revision: 3 $
// $Workfile: MOS_MsgRecorder.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_MsgRecorder.h"

#include "MOS_AgentServerMsgMgr.h"
#include "MOS_App.h"


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::Msg::Msg
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
MOS_MsgRecorder::Msg::Msg()
    : pMsg_( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::Msg::Msg
/** @param  asnPEREncodeBuffer 
    @param  nTime 
    @param  nOffset 
*/
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
MOS_MsgRecorder::Msg::Msg( ASN1PEREncodeBuffer& asnPEREncodeBuffer, int nTime, int nOffset )
    : bContext_ ( false )
    , nContext_ ( 0 )
    , pMsg_     ( asnPEREncodeBuffer.GetMsgCopy() )
    , nMsgLength_( asnPEREncodeBuffer.GetMsgLen() )
    , nTime_    ( nTime )
    , nOffset_  ( nOffset )
{
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::Msg::Msg
/** @param  nContext 
    @param  asnPEREncodeBuffer 
    @param  nTime 
    @param  nOffset 
*/
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
MOS_MsgRecorder::Msg::Msg( MIL_MOSContextID nContext, ASN1PEREncodeBuffer& asnPEREncodeBuffer, int nTime, int nOffset )
    : bContext_ ( true )
    , nContext_ ( nContext )
    , pMsg_     ( asnPEREncodeBuffer.GetMsgCopy() )
    , nMsgLength_( asnPEREncodeBuffer.GetMsgLen() )
    , nTime_    ( nTime )
    , nOffset_  ( nOffset )
{
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::Msg::~Msg
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
MOS_MsgRecorder::Msg::~Msg()
{
    delete[] pMsg_;
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::Msg::GetOffset
/** @return 
*/
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
int MOS_MsgRecorder::Msg::GetOffset() const
{
    return nOffset_;
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::Msg::GetTime
/** @return 
*/
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
int MOS_MsgRecorder::Msg::GetTime() const
{
    return nTime_;
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::Msg::SendSelf
/** @param  msgManager 
*/
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MOS_MsgRecorder::Msg::SendSelf( MOS_AgentServerMsgMgr& msgManager )
{
    if( bContext_ )
        msgManager.SendMsgMosSimWithContext( pMsg_, nMsgLength_, nContext_ );
    else
        msgManager.SendMsgMosSim( pMsg_, nMsgLength_ );
    MT_LOG_INFO( "Envoi ordre enregistré", eSent, 0 );
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::Msg::ReadArchive
/** @param  archive 
*/
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MOS_MsgRecorder::Msg::ReadArchive( MT_InputArchive_ABC& archive )
{
    archive >> bContext_;
    archive >> nContext_;
    archive >> nTime_;
    archive >> nOffset_;
    archive >> nMsgLength_;
    pMsg_ = new ASN1OCTET[ nMsgLength_];
    for( int n = 0; n < nMsgLength_; ++n )
        archive >> pMsg_[n];
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::Msg::WriteArchive
/** @param  archive 
*/
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MOS_MsgRecorder::Msg::WriteArchive( MT_OutputArchive_ABC& archive ) const
{
    archive << bContext_;
    archive << nContext_;
    archive << nTime_;
    archive << nOffset_;
    archive << nMsgLength_;
    for( int n = 0; n < nMsgLength_; ++n )
        archive << pMsg_[n];
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder constructor
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
MOS_MsgRecorder::MOS_MsgRecorder( MOS_AgentServerMsgMgr& msgManager )
    : msgManager_   ( msgManager )
    , nState_       ( eStopped )
    , nTimeStart_   ( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder destructor
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
MOS_MsgRecorder::~MOS_MsgRecorder()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::OnNewMsg
/** @param  nType 
    @param  pMsg 
    */
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MOS_MsgRecorder::OnNewMsg( int /*nType*/, ASN1PEREncodeBuffer& /*asnPEREncodeBuffer*/ )
{
    if( nState_ != eRecording )
        return;
    return;
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::OnNewMsgWithContext
/** @param  nType 
    @param  nContext 
        @param  pMsg 
        */
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MOS_MsgRecorder::OnNewMsgWithContext( int nType, MIL_MOSContextID nContext, ASN1PEREncodeBuffer& asnPEREncodeBuffer )
{
    if( nState_ != eRecording )
        return;

    if(    nType != T_MsgsMosSimWithContext_msg_pion_order
        && nType != T_MsgsMosSimWithContext_msg_order_conduite
        && nType != T_MsgsMosSimWithContext_msg_automate_order )
        return;

    int nTime = MOS_App::GetApp().GetTime();
    Msg* pNewMsg = new Msg( nContext, asnPEREncodeBuffer, nTime, nTime - nTimeStart_);
    messages_.push_back( pNewMsg );
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::Play
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MOS_MsgRecorder::Play()
{
//    nState_ = ePlaying;
//    nTimeStart_ = MOS_App::GetApp().GetTime();

    for( IT_MsgVector it = messages_.begin(); it != messages_.end(); ++it )
    {
        (*it)->SendSelf( msgManager_ );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::Record
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MOS_MsgRecorder::Record()
{
    this->Clear();
    nState_ = eRecording;
    nTimeStart_ = MOS_App::GetApp().GetTime();
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::Stop
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MOS_MsgRecorder::Stop()
{
    nState_ = eStopped;
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::Clear
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MOS_MsgRecorder::Clear()
{
    messages_.clear();
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::OnTimeTick
/** @param  nTime 
*/
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MOS_MsgRecorder::OnTimeTick( int )
{
    /*
    This is disabled since the users find it more useful to have all the recorded
    messages sent at once once read back from a file.

    int nOffset = nTime - nTimeStart_;
    for( IT_MsgVector it = messages_.begin(); it != messages_.end(); ++it )
    {
        if( (*it)->GetOffset() == nOffset )
        {
            (*it)->SendSelf( msgManager_ );
        }
    }
    */
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::ReadArchive
/** @param  archive 
*/
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MOS_MsgRecorder::ReadArchive( MT_InputArchive_ABC& archive )
{
    Clear();
    int nSize;
    archive >> nTimeStart_;
    archive >> nSize;
    for( int n = 0; n < nSize; ++n  )
    {
        Msg* pNewMsg = new Msg();
        pNewMsg->ReadArchive( archive );
        messages_.push_back( pNewMsg );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_MsgRecorder::WriteArchive
/** @param  archive 
*/
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MOS_MsgRecorder::WriteArchive( MT_OutputArchive_ABC& archive ) const
{
    archive << nTimeStart_;
    archive << (int)messages_.size();
    for( CIT_MsgVector it = messages_.begin(); it != messages_.end(); ++it )
    {
        (*it)->WriteArchive( archive );
    }
}

