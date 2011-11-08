// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "SaverFacade.h"
#include "Saver.h"
#include "Savable_ABC.h"
#include "dispatcher/Config.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Visitors.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/Protocol.h"
#include "tools/OutputBinaryWrapper.h"

using namespace plugins::saver;

// -----------------------------------------------------------------------------
// Name: SaverFacade constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
SaverFacade::SaverFacade( dispatcher::ClientPublisher_ABC& client, dispatcher::Model_ABC& model, const dispatcher::Config& config )
    : config_         ( config )
    , model_          ( model )
    , saver_          ( new Saver( client, config ) )
    , keyFrameIndex_  ( 0 )
    , fragmentIndex_  ( 0 )
    , firstTick_      ( true )
    , infoInitialized_( false )
{
    // NOTHING
}

namespace
{
    class Message : public Savable_ABC
    {
    public:
        Message( const std::string& buffer )
            : buffer_( buffer )
        {
            // NOTHING
        }

        virtual void Serialize( tools::OutputBinaryWrapper& output ) const
        {
            output << static_cast< unsigned int >( buffer_.size() );
            output.Write( buffer_.c_str(), buffer_.size() );
        }

    private:
        const std::string& buffer_;
    };

    template< typename M >
    std::string& SerializeToString( const M& message, std::string& buffer )
    {
        if( !message.SerializeToString( &buffer ) )
        {
            std::stringstream ss;
            ss << "Error serializing message of type \"" << message.GetTypeName() << "\"" << std::endl;
            throw std::runtime_error( ss.str() );
        }
        return buffer;
    }

    class ModelMessage : public Savable_ABC, public dispatcher::ClientPublisher_ABC
    {
    public:
        ModelMessage( const dispatcher::Model_ABC& model, std::string& buffer, bool firstFrame )
            : model_     ( model )
            , output_    ( 0 )
            , buffer_    ( buffer )
            , firstFrame_( firstFrame )
        {
            // NOTHING
        }

        virtual ~ModelMessage()
        {
            // NOTHING
        }

        virtual void Serialize( tools::OutputBinaryWrapper& output ) const
        {
            ModelMessage* that = const_cast< ModelMessage* >( this );
            that->output_ = &output;
            {
                dispatcher::CreationVisitor visitor( *that );
                model_.Accept( visitor );
            }
            {
                dispatcher::FullUpdateVisitor visitor( *that );
                model_.Accept( visitor );
            }
            // $$$$ AGE 2007-10-15: revoir tous ces trucs !
            if( firstFrame_ )
                model_.SendFirstTick( *that );
        }

        virtual void Send( const sword::SimToClient& message )
        {
            Message( SerializeToString( message, buffer_ ) ).Serialize( *output_ );
        }
        virtual void Send( const sword::AuthenticationToClient& ) {}
        virtual void Send( const sword::ReplayToClient& ) {}
        virtual void Send( const sword::AarToClient& ) {}
        virtual void Send( const sword::MessengerToClient& ) {}
        virtual void Send( const sword::DispatcherToClient& ) {}

    private:
        const dispatcher::Model_ABC& model_;
        tools::OutputBinaryWrapper* output_;
        std::string& buffer_;
        bool firstFrame_;
    };
}

// -----------------------------------------------------------------------------
// Name: SaverFacade destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
SaverFacade::~SaverFacade()
{
    if( firstTick_ == true )
        saver_->SaveKeyFrame( ModelMessage( model_, encodingBuffer_, firstTick_ ) );
}

// -----------------------------------------------------------------------------
// Name: SaverFacade::Receive
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void SaverFacade::Receive( const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_control_begin_tick())
        StartFrame( wrapper, wrapper.message().control_begin_tick().date_time().data() );
    else if( wrapper.message().has_control_end_tick() )
        EndFrame( wrapper );
    else if( wrapper.message().has_control_checkpoint_save_end() )
        fragmentIndex_ = config_.GetReplayFragmentsFrequency();
    else if( !infoInitialized_ && wrapper.message().has_control_information() )
    {
        infoInitialized_ = true;
        saver_->ControlInformation( wrapper.message().control_information() );
    }
    else if( filter_.IsRelevant( wrapper ) )
    {
        if( firstTick_ && config_.HasCheckpoint() &&
          ( wrapper.message().has_unit_creation() || wrapper.message().has_automat_creation() || wrapper.message().has_formation_creation() ||
            wrapper.message().has_knowledge_group_creation() || wrapper.message().has_report() || wrapper.message().has_party_creation() ||
            wrapper.message().has_object_creation() || wrapper.message().has_crowd_creation() || wrapper.message().has_crowd_concentration_creation() ||
            wrapper.message().has_crowd_flow_creation() || wrapper.message().has_unit_knowledge_creation() || wrapper.message().has_object_knowledge_creation() ||
            wrapper.message().has_crowd_knowledge_creation() || wrapper.message().has_crowd_concentration_knowledge_creation() || wrapper.message().has_crowd_flow_knowledge_creation() ||
            wrapper.message().has_log_maintenance_handling_creation() || wrapper.message().has_log_medical_handling_creation() || wrapper.message().has_log_supply_handling_creation() ||
            wrapper.message().has_start_unit_fire() || wrapper.message().has_start_fire_effect() || wrapper.message().has_start_crowd_fire() ||
            wrapper.message().has_urban_creation() || wrapper.message().has_urban_knowledge_creation() ) )
            return;
        saver_->SaveUpdateMessage( Message( SerializeToString( wrapper, encodingBuffer_ ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SaverFacade::StartFrame
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void SaverFacade::StartFrame( const sword::SimToClient& message, const std::string& dateTime )
{
    if( firstTick_ && config_.HasCheckpoint() )
        firstTick_ = false;
    unsigned int freq = config_.GetReplayFragmentsFrequency();
    if( freq != 0 && fragmentIndex_ == freq )
    {
        saver_->CreateNewFragment();
        fragmentIndex_ = 0;
        keyFrameIndex_ = 0;
    }
    if( keyFrameIndex_== config_.GetKeyFramesFrequency() )
        keyFrameIndex_ = 0;
    if( keyFrameIndex_ == 0 )
        saver_->SaveKeyFrame( ModelMessage( model_, encodingBuffer_, firstTick_ ) );
    saver_->StartFrame( Message( SerializeToString( message, encodingBuffer_ ) ), dateTime );
    ++fragmentIndex_;
    ++keyFrameIndex_;
    firstTick_ = false;
}

// -----------------------------------------------------------------------------
// Name: SaverFacade::EndFrame
// Created: AGE 2007-04-11
// -----------------------------------------------------------------------------
void SaverFacade::EndFrame( const sword::SimToClient& message )
{
    saver_->EndFrame( Message( SerializeToString( message, encodingBuffer_ ) ) );
}
