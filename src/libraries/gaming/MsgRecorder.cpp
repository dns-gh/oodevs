// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
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
    : pMsg_     ( 0 )
    , nMsgLength_( asnPEREncodeBuffer.GetMsgLen() )
{
    pMsg_ = new ASN1OCTET[ nMsgLength_ ];
    memcpy( pMsg_, asnPEREncodeBuffer.GetMsgPtr(), nMsgLength_ );
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
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void Message::Send( AgentServerMsgMgr& msgManager )
{
    msgManager.SendMsgClientToSim( pMsg_, nMsgLength_ );
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

    boost::mutex::scoped_lock locker( mutex_ );
    for( IT_Messages it = messages_.begin(); it != messages_.end(); ++it )
        delete *it;
    messages_.clear();
}

// -----------------------------------------------------------------------------
// Name: MsgRecorder::OnNewMsg
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MsgRecorder::OnNewMsg( int nType, ASN1PEREncodeBuffer& asnPEREncodeBuffer )
{
    if( ! recording_ )
        return;

    if( nType != T_MsgsClientToSim_msg_msg_pion_order
        && nType != T_MsgsClientToSim_msg_msg_frag_order
        && nType != T_MsgsClientToSim_msg_msg_automate_order
        && nType != T_MsgsClientToSim_msg_msg_population_order )
        return;
    Message* message = new Message( asnPEREncodeBuffer );

    boost::mutex::scoped_lock locker( mutex_ );
    messages_.push_back( message );
}

// -----------------------------------------------------------------------------
// Name: MsgRecorder::Play
// Created: AGE 2007-03-05
// -----------------------------------------------------------------------------
void MsgRecorder::Play( const std::string& filename )
{
    std::ifstream input( filename.c_str(), std::ios_base::binary & std::ios_base::in );
    int size;
    input.read( (char*)(&size), sizeof( int ) );
    for( int n = 0; n < size; ++n  )
    {
        Message message( input );
        message.Send( msgManager_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MsgRecorder::Record
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MsgRecorder::Record()
{
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
// Name: MsgRecorder::Write
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void MsgRecorder::Write( const std::string& filename )
{
    T_Messages toSave;
    {
        boost::mutex::scoped_lock locker( mutex_ );
        std::swap( toSave, messages_ );
    }

    std::ofstream output( filename.c_str(), std::ios_base::binary & std::ios_base::out );
    int size = int( toSave.size() );
    output.write( (const char*)( &size ), sizeof( size ) );
    for( CIT_Messages it = toSave.begin(); it != toSave.end(); ++it )
    {
        (*it)->Write( output );
        delete *it;
    }
}
