// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Folk.h"
#include "Network/NET_Publisher_ABC.h"
#include "Tools/MIL_Config.h"

#include "folk/Config.h"
#include "folk/FlowManagerFactory.h"
#include "folk/FlowManager_ABC.h"
#include "folk/ObjectManager_ABC.h"
#include "folk/Serializer_ABC.h"
#include "folk/Time.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ClientSenders.h"

using namespace population;

// -----------------------------------------------------------------------------
// Name: MIL_Folk constructor
// Created: JCR 2007-08-24
// -----------------------------------------------------------------------------
MIL_Folk::MIL_Folk( const MIL_Config& config )
    : pFlow_ ( ( config.IsPopulationEnabled() ) ? population::FlowManagerFactory::Create() : 0 )
    , first_update_ ( true )
    , step_         ( 0 )
    , start_        ( 0 )
{
    if( pFlow_ )
    {
        MT_LOG_INFO_MSG( "Loading population module" )
        Config  popConfig( config.GetPopulationDir(), "population.xml" );
        pFlow_->Load( popConfig );
        start_ = 14 * 3600 * 1000; // 14h00m
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Folk destructor
// Created: JCR 2007-08-24
// -----------------------------------------------------------------------------
MIL_Folk::~MIL_Folk()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Folk::GetObjectManager
// Created: JCR 2007-09-12
// -----------------------------------------------------------------------------
ObjectManager_ABC* MIL_Folk::GetObjectManager() const
{
    if( pFlow_ )
        return &pFlow_->GetObjectManager();
    return (ObjectManager_ABC*)0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Folk::Update
// Created: JCR 2007-08-24
// -----------------------------------------------------------------------------
void MIL_Folk::Update( unsigned utime, unsigned timeStep )
{
    if( pFlow_ )
    {
        pFlow_->Update( population::Time( "", start_ + utime ), timeStep );
        UpdateNetwork();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Folk::SendStateToNewClient
// Created: JCR 2007-08-30
// -----------------------------------------------------------------------------
void MIL_Folk::SendStateToNewClient() const
{
    if( pFlow_ )
    {
        SendCreation();
        SendUpdate();
        first_update_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Folk::UpdateNetwork
// Created: JCR 2007-08-27
// -----------------------------------------------------------------------------
void MIL_Folk::UpdateNetwork() const
{
    if( !first_update_ && ( ++step_ % 60 ) == 0 )
        SendUpdate();
}

namespace
{
    template< typename T >
    void CopyList( T& list, const std::vector<const std::string*>& value )
    {
        for( int i = 0; i < list.elem_size(); ++i )
            *list.mutable_elem( i ) = value[i]->c_str();
    }

    struct FolkCreationSerializer : public population::message::Serializer_ABC
    {
        explicit FolkCreationSerializer( client::FolkCreation& asn ) : asn_ ( &asn )
        {
//            (*asn_)().mutable_profiles()->set_n( 0 );
//            (*asn_)().mutable_activities()->set_n( 0 );
        }

        virtual population::message::Serializer_ABC& operator <<( const std::vector<const std::string*>& value )
        {
            if( (*asn_)().profiles().elem_size() == 0 )
                CopyList( *(*asn_)().mutable_profiles(), value );
            else
                CopyList( *(*asn_)().mutable_activities(), value );
            return *this;
        }
        client::FolkCreation* asn_;
    };

    struct FolkUpdateSerializer : public population::message::Serializer_ABC
    {
        FolkUpdateSerializer()
            : bufferOffset_( 0 )
            , count_       ( 0 )
        {
            messages_.reserve( 50 );
            const unsigned maxBufferLength = 0x4000 - 1; // $$$$ AGE 2008-02-13: ASN1.PER limit before fragmentation
            buffer_.resize( maxBufferLength );
        }

        virtual population::message::Serializer_ABC& operator <<( int value )
        {
            messages_.push_back( MsgsSimToClient::MsgFolkGraphEdgeUpdate() );
            messages_.back().set_oid( value );
            messages_.back().set_shp_oid( count_++ );
            return *this;
        }

        virtual population::message::Serializer_ABC& operator <<( const std::vector<int>& value )
        {
            const unsigned size = value.size();
            if( size + bufferOffset_ > buffer_.size() )
                Flush();
            memcpy( &buffer_[bufferOffset_], &*value.begin(), size * sizeof( int ) );
            for (std::vector < int >::const_iterator iter(buffer_.begin()); iter != buffer_.end(); ++iter)
                messages_.back().add_population_occupation( *iter );
            bufferOffset_+=size;
            return *this;
        }
        void Commit()
        {
            if( ! messages_.empty() )
            {
                client::FolkGraphUpdate message;
                for (std::vector< MsgsSimToClient::MsgFolkGraphEdgeUpdate >::const_iterator iter(messages_.begin()); iter != messages_.end(); ++iter)
                    *message().add_elem() = *iter ;
                message.Send( NET_Publisher_ABC::Publisher() );
            }
            bufferOffset_ = 0;
            messages_.resize( 0 );
        }
    private:
        void Flush()
        {
            if( messages_.empty() ) return;
            int lastId = messages_.back().oid();
            messages_.pop_back();
            --count_;

            Commit();

            *this << lastId;
        }
        std::vector< int > buffer_;
        std::vector< MsgsSimToClient::MsgFolkGraphEdgeUpdate > messages_;
        unsigned bufferOffset_;
        unsigned count_;
    };

    struct EdgeCounter : public population::message::Serializer_ABC
    {
        EdgeCounter() : count_( 0 ) {}
        virtual population::message::Serializer_ABC& operator <<( int )
        {
            ++count_;
            return *this;
        }
        virtual population::message::Serializer_ABC& operator <<( const std::vector<int>& )
        {
            return *this;
        }
        unsigned count_;
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_Folk::SendCreation
// Created: JCR 2007-08-24
// -----------------------------------------------------------------------------
void MIL_Folk::SendCreation() const
{
    client::FolkCreation asn;
    FolkCreationSerializer  serializer( asn );
    asn().set_container_size( 5 ); // $$$$ AGE 2007-09-04: hc

    EdgeCounter counter;
    pFlow_->SerializePopulationUpdate( counter );
    asn().set_edge_number( counter.count_ );

    MT_LOG_INFO_MSG( "MIL_Folk::SendCreation()" )
    pFlow_->SerializePopulationCreation( serializer );
    asn.Send( NET_Publisher_ABC::Publisher() );
    asn().mutable_profiles()->Clear();
    asn().mutable_activities()->Clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_Folk::SendUpdate
// Created: JCR 2007-08-24
// -----------------------------------------------------------------------------
void MIL_Folk::SendUpdate() const
{
    MT_LOG_INFO_MSG( "MIL_Folk::SendUpdate()" )
    FolkUpdateSerializer    serializer;
    pFlow_->SerializePopulationUpdate( serializer );
    serializer.Commit();
}
