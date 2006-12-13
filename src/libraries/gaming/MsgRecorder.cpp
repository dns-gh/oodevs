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
// $Archive: /MVW_v10/Build/SDK/Light2/src/MsgRecorder.cpp $
// $Author: Ape $
// $Modtime: 6/07/05 11:35 $
// $Revision: 3 $
// $Workfile: MsgRecorder.cpp $
//
// *****************************************************************************

#include "gaming_pch.h"
#include "MsgRecorder.h"
#include "AgentServerMsgMgr.h"
#include "Network.h"
#include <fstream>

namespace internal
{

    class Message
    {
    public:
        explicit Message( std::ifstream& input );
        explicit Message( ASN1PEREncodeBuffer& asnPEREncodeBuffer );
        virtual ~Message();

        void Write( std::ofstream& output ) const;
        void Send( AgentServerMsgMgr& msgManager );

    private:
        ASN1OCTET* pMsg_;
        int nMsgLength_;
    };

}

using namespace internal;

// -----------------------------------------------------------------------------
// Name: Message::Message
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
Message::Message( std::ifstream& input )
    : pMsg_( 0 )
{
    input.read( (char*)( &nMsgLength_ ), sizeof( nMsgLength_ ) );
    pMsg_ = new ASN1OCTET[ nMsgLength_];
    input.read( (char*)pMsg_, nMsgLength_ );
}


// -----------------------------------------------------------------------------
// Name: Message::Message
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
Message::Message( ASN1PEREncodeBuffer& asnPEREncodeBuffer )
    : pMsg_     ( asnPEREncodeBuffer.GetMsgCopy() )
    , nMsgLength_( asnPEREncodeBuffer.GetMsgLen() )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: Message::~Message
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
Message::~Message()
{
    delete[] pMsg_;
}

// -----------------------------------------------------------------------------
// Name: Message::Send
/** @param  msgManager 
*/
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void Message::Send( AgentServerMsgMgr& msgManager )
{
    msgManager.SendMsgOutClient( pMsg_, nMsgLength_ );
}

// -----------------------------------------------------------------------------
// Name: Message::Write
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void Message::Write( std::ofstream& output ) const 
{
    output.write( (const char*)( &nMsgLength_ ), sizeof( nMsgLength_ ) );
    output.write( (const char*)pMsg_, nMsgLength_ );
}

// -----------------------------------------------------------------------------
// Name: MsgRecorder constructor
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
MsgRecorder::MsgRecorder( Network& network )
    : msgManager_( network.GetMessageMgr() )
    , recording_ ( false )
{
    msgManager_.RegisterMessageRecorder( *this );   
}


// -----------------------------------------------------------------------------
// Name: MsgRecorder destructor
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
MsgRecorder::~MsgRecorder()
{
    msgManager_.UnregisterMessageRecorder( *this );
    Clear();
}

// -----------------------------------------------------------------------------
// Name: MsgRecorder::OnNewMsg
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MsgRecorder::OnNewMsg( int nType, ASN1PEREncodeBuffer& asnPEREncodeBuffer )
{
    if( ! recording_ )
        return;

    if(    nType != T_MsgsOutClient_msg_msg_pion_order
        && nType != T_MsgsOutClient_msg_msg_frag_order
        && nType != T_MsgsOutClient_msg_msg_automate_order )
        return;

    messages_.push_back( new Message( asnPEREncodeBuffer ) );
}

// -----------------------------------------------------------------------------
// Name: MsgRecorder::Play
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MsgRecorder::Play()
{
    for( IT_Messages it = messages_.begin(); it != messages_.end(); ++it )
        (*it)->Send( msgManager_ );
}

// -----------------------------------------------------------------------------
// Name: MsgRecorder::Record
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MsgRecorder::Record()
{
    Clear();
    recording_ = true;
}

// -----------------------------------------------------------------------------
// Name: MsgRecorder::Stop
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
bool MsgRecorder::Stop()
{
    const bool wasRecording = recording_;
    recording_ = false;
    return wasRecording;
}

// -----------------------------------------------------------------------------
// Name: MsgRecorder::Clear
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MsgRecorder::Clear()
{
    for( IT_Messages it = messages_.begin(); it != messages_.end(); ++it )
        delete *it;
    messages_.clear();
}

// -----------------------------------------------------------------------------
// Name: MsgRecorder::Read
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void MsgRecorder::Read( const std::string& filename )
{
    Clear();
    std::ifstream input( filename.c_str(), std::ios_base::binary & std::ios_base::in );
    int size;
    input.read( (char*)(&size), sizeof( int ) );
    for( int n = 0; n < size; ++n  )
        messages_.push_back( new Message( input ) );
}

// -----------------------------------------------------------------------------
// Name: MsgRecorder::Write
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void MsgRecorder::Write( const std::string& filename ) const
{
    std::ofstream output( filename.c_str(), std::ios_base::binary & std::ios_base::out );
    int size = (int)( messages_.size() );
    output.write( (const char*)( &size ), sizeof( size ) );
    for( CIT_Messages it = messages_.begin(); it != messages_.end(); ++it )
        (*it)->Write( output );
}

