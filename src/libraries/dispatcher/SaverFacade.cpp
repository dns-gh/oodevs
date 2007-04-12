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
#include "pathfind/OutputBinaryWrapper.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SaverFacade constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
SaverFacade::SaverFacade( Model& model, const std::string& directory )
    : model_( model )
    , saver_( new Saver( directory ) )
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
void SaverFacade::Save( ASN1T_MsgsOutSim& asnMsg )
{
    if( asnMsg.msg.t == T_MsgsOutSim_msg_msg_ctrl_begin_tick )
        StartFrame( asnMsg );
    else if( asnMsg.msg.t == T_MsgsOutSim_msg_msg_ctrl_end_tick )
        EndFrame( asnMsg );
    else if( filter_.IsRelevant( asnMsg ) )
        SaveUpdate( asnMsg );
}

namespace
{
    struct Message : public Savable_ABC
    {
        Message( ASN1PEREncodeBuffer& buffer )
            : buffer_( &buffer ) {};
        virtual void Serialize( OutputBinaryWrapper& output ) const
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
void SaverFacade::SaveUpdate( ASN1T_MsgsOutSim& asnMsg )
{
    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );
    ASN1C_MsgsOutSim asnMsgCtrl( asnPEREncodeBuffer, asnMsg );
    asnMsgCtrl.Encode();
    saver_->SaveUpdateMessage( Message( asnPEREncodeBuffer ) );
}

namespace
{
    struct ModelMessage : public Savable_ABC, public Publisher_ABC
    {
        ModelMessage( Model& model, ASN1OCTET* buffer, unsigned size )
            : model_( &model )
            , output_( 0 ) 
            , buffer_( buffer ) 
            , size_( size ) {}
        virtual void Serialize( OutputBinaryWrapper& output ) const
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
        }
        virtual void Send( const ASN1T_MsgsInClient& msg )
        {
            ASN1PEREncodeBuffer asnPEREncodeBuffer( buffer_, size_, TRUE );
            ASN1C_MsgsInClient asnMsgCtrl( asnPEREncodeBuffer, const_cast< ASN1T_MsgsInClient& >( msg ) );
            asnMsgCtrl.Encode();
            Message( asnPEREncodeBuffer ).Serialize( *output_ );
        }
        Model* model_;
        OutputBinaryWrapper* output_;
        ASN1OCTET* buffer_;
        unsigned size_;
    };
}

// -----------------------------------------------------------------------------
// Name: SaverFacade::StartFrame
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void SaverFacade::StartFrame( ASN1T_MsgsOutSim& asnMsg )
{
    if( (frameCount_++) % 100 == 0 ) // $$$$ AGE 2007-04-11: 
        saver_->SaveKeyFrame( ModelMessage( model_, aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_) ) );

    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );
    ASN1C_MsgsOutSim asnMsgCtrl( asnPEREncodeBuffer, asnMsg );
    asnMsgCtrl.Encode();
    saver_->StartFrame( Message( asnPEREncodeBuffer ) );
}

// -----------------------------------------------------------------------------
// Name: SaverFacade::EndFrame
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void SaverFacade::EndFrame( ASN1T_MsgsOutSim& asnMsg )
{
    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );
    ASN1C_MsgsOutSim asnMsgCtrl( asnPEREncodeBuffer, asnMsg );
    asnMsgCtrl.Encode();
    saver_->EndFrame( Message( asnPEREncodeBuffer ) );
}
