// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "SaverFacade.h"
#include "Saver.h"
#include "Savable_ABC.h"
#include "Model.h"
#include "Side.h"
#include "Publisher_ABC.h"
#include "Visitors.h"
#include "network_def.h"
#include "tools/OutputBinaryWrapper.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SaverFacade constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
SaverFacade::SaverFacade( Model& model, const Config& config )
    : model_     ( model )
    , saver_     ( new Saver( config ) )
    , frameCount_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SaverFacade destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
SaverFacade::~SaverFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SaverFacade::Save
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void SaverFacade::Save( const ASN1T_MsgsSimToClient& asnMsg )
{
    if( asnMsg.msg.t == T_MsgsSimToClient_msg_msg_ctrl_begin_tick )
        StartFrame( const_cast< ASN1T_MsgsSimToClient& >( asnMsg ) );
    else if( asnMsg.msg.t == T_MsgsSimToClient_msg_msg_ctrl_end_tick )
        EndFrame( const_cast< ASN1T_MsgsSimToClient& >( asnMsg ) );
    else if( filter_.IsRelevant( asnMsg ) )
        SaveUpdate( const_cast< ASN1T_MsgsSimToClient& >( asnMsg ) );
}

namespace
{
    struct Message : public Savable_ABC
    {
        Message( ASN1PEREncodeBuffer& buffer )
            : buffer_( &buffer ) {};
        virtual void Serialize( tools::OutputBinaryWrapper& output ) const
        {
            output << buffer_->GetMsgLen();
            output.Write( (const char*)buffer_->GetMsgPtr(), buffer_->GetMsgLen() );
        };
    private:
        ASN1PEREncodeBuffer* buffer_;
    };
}

// -----------------------------------------------------------------------------
// Name: SaverFacade::SaveUpdate
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void SaverFacade::SaveUpdate( ASN1T_MsgsSimToClient& asnMsg )
{
    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );
    ASN1C_MsgsSimToClient asnMsgCtrl( asnPEREncodeBuffer, asnMsg );
    asnMsgCtrl.Encode();
    saver_->SaveUpdateMessage( Message( asnPEREncodeBuffer ) );
}

namespace
{
    struct ModelMessage : public Savable_ABC, public Publisher_ABC
    {
        ModelMessage( Model& model, ASN1OCTET* buffer, unsigned size, bool firstFrame )
            : model_( &model )
            , output_( 0 ) 
            , buffer_( buffer ) 
            , size_( size )
            , firstFrame_( firstFrame ) {}
        virtual void Serialize( tools::OutputBinaryWrapper& output ) const
        {
            ModelMessage* that = const_cast< ModelMessage* >( this );
            that->output_ = &output;
            {
                CreationVisitor visitor( *that );
                model_->Accept( visitor );
            }
            {
                FullUpdateVisitor visitor( *that );
                model_->Accept( visitor );
            }
            if( firstFrame_ )
                that->SendStartTick();
        }
        virtual void Send( const ASN1T_MsgsSimToClient& msg )
        {
            ASN1PEREncodeBuffer asnPEREncodeBuffer( buffer_, size_, TRUE );
            ASN1C_MsgsSimToClient asnMsgCtrl( asnPEREncodeBuffer, const_cast< ASN1T_MsgsSimToClient& >( msg ) );
            asnMsgCtrl.Encode();
            Message( asnPEREncodeBuffer ).Serialize( *output_ );
        }
        void SendStartTick()
        {
            AsnMsgSimToClientControlBeginTick tick;
            tick() = 0;
            tick.Send( *this );
        }
        Model* model_;
        tools::OutputBinaryWrapper* output_;
        ASN1OCTET* buffer_;
        unsigned size_;
        bool firstFrame_;
    };
}

// -----------------------------------------------------------------------------
// Name: SaverFacade::StartFrame
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void SaverFacade::StartFrame( ASN1T_MsgsSimToClient& asnMsg )
{
    if( frameCount_ % 100 == 0 ) // $$$$ AGE 2007-04-11: 
        saver_->SaveKeyFrame( ModelMessage( model_, aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), frameCount_ == 0 ) );
    ++frameCount_;

    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );
    ASN1C_MsgsSimToClient asnMsgCtrl( asnPEREncodeBuffer, asnMsg );
    asnMsgCtrl.Encode();
    saver_->StartFrame( Message( asnPEREncodeBuffer ) );
}

// -----------------------------------------------------------------------------
// Name: SaverFacade::EndFrame
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void SaverFacade::EndFrame( ASN1T_MsgsSimToClient& asnMsg )
{
    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );
    ASN1C_MsgsSimToClient asnMsgCtrl( asnPEREncodeBuffer, asnMsg );
    asnMsgCtrl.Encode();
    saver_->EndFrame( Message( asnPEREncodeBuffer ) );
}
