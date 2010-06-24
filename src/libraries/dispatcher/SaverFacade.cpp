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
#include "protocol/ClientPublisher_ABC.h"
#include "Model.h"
#include "Side.h"
#include "Visitors.h"
#include "tools/OutputBinaryWrapper.h"
#include <google/protobuf/Message.h>
#include <google/protobuf/Descriptor.h>
#include "protocol/protocol.h"

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
// Name: SaverFacade::Receive
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void SaverFacade::Receive( const MsgsSimToClient::MsgSimToClient& wrapper )
{
    if( wrapper.message().has_control_begin_tick())
        StartFrame( wrapper );
    else if( wrapper.message().has_control_end_tick() )
        EndFrame( wrapper );
    else if( filter_.IsRelevant( wrapper ) )
        SaveUpdate( wrapper );
}

namespace
{
    struct Message : public Savable_ABC
    {
        Message( std::string& buffer )
            : buffer_( &buffer ) {};
        virtual void Serialize( tools::OutputBinaryWrapper& output ) const
        {
            output << buffer_->size();
            output.Write( buffer_->c_str(), buffer_->size() );
        };
    private:
        std::string* buffer_;
    };

    template< typename M >
    std::string& SerializeToString( const M& message, std::string& buffer )
    {
        if( !message.SerializeToString( &buffer ) )
        {
            std::stringstream ss;
            ss << "Error serializing message of type \"" << message.GetDescriptor()->full_name() << "\"" << std::endl;
            throw std::runtime_error( ss.str() );
        }
        return buffer;
    }
}

// -----------------------------------------------------------------------------
// Name: SaverFacade::SaveUpdate
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void SaverFacade::SaveUpdate( const MsgsSimToClient::MsgSimToClient& wrapper )
{
    if( wrapper.has_message() )
        saver_->SaveUpdateMessage( Message( SerializeToString( wrapper, encodingBuffer_ ) ) );
}

namespace
{
    struct ModelMessage : public Savable_ABC, public ClientPublisher_ABC
    {
        ModelMessage( Model& model, std::string& buffer, bool firstFrame )
            : model_( &model )
            , output_( 0 ) 
            , buffer_( buffer ) 
            , firstFrame_( firstFrame ) {}
        virtual ~ModelMessage() {}
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
            // $$$$ AGE 2007-10-15: revoir tous ces trucs !
            if( firstFrame_ )
                model_->SendFirstTick( *that );
        }
        virtual void Send( const MsgsSimToClient::MsgSimToClient& message )
        {
            Message( SerializeToString( message, buffer_ ) ).Serialize( *output_ );
        }
        virtual void Send( const MsgsAuthenticationToClient::MsgAuthenticationToClient& ) {}
        virtual void Send( const MsgsReplayToClient::MsgReplayToClient& ) {}
        virtual void Send( const MsgsAarToClient::MsgAarToClient& ) {}
        virtual void Send( const MsgsMessengerToClient::MsgMessengerToClient& ) {}
        virtual void Send( const MsgsDispatcherToClient::MsgDispatcherToClient& ) {}
        virtual std::string GetEndpoint() const { return ""; }
        Model* model_;
        tools::OutputBinaryWrapper* output_;
        std::string& buffer_;
        bool firstFrame_;
    };
}

// -----------------------------------------------------------------------------
// Name: SaverFacade::StartFrame
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void SaverFacade::StartFrame( const MsgsSimToClient::MsgSimToClient& message )
{
    if( ( frameCount_ % 100 ) == 0 )
        saver_->SaveKeyFrame( ModelMessage( model_, encodingBuffer_, frameCount_ == 0 ) );
    ++frameCount_;
    saver_->StartFrame( Message( SerializeToString( message, encodingBuffer_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: SaverFacade::EndFrame
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void SaverFacade::EndFrame( const MsgsSimToClient::MsgSimToClient& message )
{
    saver_->EndFrame( Message( SerializeToString( message, encodingBuffer_ ) ) );
}
