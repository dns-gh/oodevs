// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "RoleAdapter.h"
#include "RoleAction_Moving.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Terrain/PHY_RoleInterface_TerrainAnalysis.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Population/PHY_RoleInterface_Population.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <core/ModelListener_ABC.h>
#include <core/Model.h>
#include <core/UserData.h>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>

using namespace sword;

BOOST_CLASS_EXPORT_IMPLEMENT( sword::RoleAdapter )

namespace
{
    template< typename Message, typename Serializer >
    class ModelSender : public network::NetworkMessageSender_ABC, private core::ModelListener_ABC
    {
    public:
        ModelSender( core::Model& model, Serializer serializer )
            : model_     ( model )
            , serializer_( serializer )
        {
            model.Register( *this );
        }
        virtual ~ModelSender()
        {
            model_.Unregister( *this );
        }
        virtual void SendChangedState() const
        {
            if( message_ )
                message_->Send( NET_Publisher_ABC::Publisher() );
            message_ = boost::none;
        }
        virtual void SendFullState( unsigned int context ) const
        {
            Message message;
            if( serializer_( message, model_ ) )
                message.Send( NET_Publisher_ABC::Publisher(), context );
        }
    private:
        virtual void Notify( const core::Model& model )
        {
            message_ = Message();
            serializer_( *message_, model );
        }
    private:
        core::Model& model_;
        mutable boost::optional< Message > message_;
        Serializer serializer_;
    };

    template< typename Message, typename Serializer >
    boost::shared_ptr< network::NetworkMessageSender_ABC > MakeModelSender( core::Model& model, Serializer serializer )
    {
        return boost::make_shared< ModelSender< Message, Serializer > >( boost::ref( model ), serializer );
    }

    template< typename M >
    bool Serialize( M& message, unsigned int identifier, const core::Model& model );

    template<>
    bool Serialize( client::UnitPathFind& message, unsigned int identifier, const core::Model& points )
    {
        if( ! points.GetSize() )
            return false;
        message().mutable_unit()->set_id( identifier );
        sword::Path& root = *message().mutable_path();
        root.mutable_location()->set_type( sword::Location::line );
        for( std::size_t i = 0; i < points.GetSize(); ++i )
        {
            const core::Model& point = points.GetElement( i );
            NET_ASN_Tools::WritePoint( MT_Vector2D( point["x"], point["y"] ),
                                       *root.mutable_location()->mutable_coordinates()->add_elem() );
        }
        return true;
    }
    template<>
    bool Serialize( client::UnitEnvironmentType& message, unsigned int identifier, const core::Model& environment )
    {
        message().mutable_unit()->set_id( identifier );
        message().set_area( environment[ "area" ] );
        message().set_left( environment[ "left" ] );
        message().set_right( environment[ "right" ] );
        message().set_linear( environment[ "linear" ] );
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: RoleAdapter constructor
// Created: SLI 2012-01-16
// -----------------------------------------------------------------------------
RoleAdapter::RoleAdapter( MIL_AgentPion& pion, core::Model& entity )
    : pion_  ( pion )
    , entity_( entity )
{
    senders_.push_back( MakeModelSender< client::UnitPathFind >( entity_[ "movement/path/points" ], boost::bind( &Serialize< client::UnitPathFind >, _1, pion.GetID(), _2 ) ) );
    senders_.push_back( MakeModelSender< client::UnitEnvironmentType >( entity_[ "movement/environment" ], boost::bind( &Serialize< client::UnitEnvironmentType >, _1, pion.GetID(), _2 ) ) );
    entity_[ "roles/RoleAction_Moving" ].SetUserData( &pion.GetRole< RoleAction_Moving >() );
    entity_[ "roles/PHY_RoleInterface_TerrainAnalysis" ].SetUserData( &pion.GetRole< PHY_RoleInterface_TerrainAnalysis >() );
    entity_[ "roles/PHY_RoleInterface_Composantes" ].SetUserData( &pion.GetRole< PHY_RoleInterface_Composantes >() );
    entity_[ "roles/DEC_Decision_ABC" ].SetUserData( &pion.GetRole< DEC_Decision_ABC >() );
    entity_[ "roles/PHY_RoleInterface_Perceiver" ].SetUserData( &pion.GetRole< PHY_RoleInterface_Perceiver >() );
    entity_[ "roles/RoleInterface_Posture" ].SetUserData( &pion.GetRole< PHY_RoleInterface_Posture >() );
    entity_[ "roles/PHY_RoleInterface_UrbanLocation" ].SetUserData( &pion.GetRole< PHY_RoleInterface_UrbanLocation >() );
    entity_[ "roles/PHY_RoleInterface_Dotations" ].SetUserData( &pion.GetRole< dotation::PHY_RoleInterface_Dotations >() );
    entity_[ "roles/PHY_RoleInterface_Communications" ].SetUserData( &pion.GetRole< PHY_RoleInterface_Communications >() );
    entity_[ "roles/PHY_RoleInterface_Posture" ].SetUserData( &pion.GetRole< PHY_RoleInterface_Posture >() );
    entity_[ "roles/PHY_RoleInterface_Population" ].SetUserData( &pion.GetRole< PHY_RoleInterface_Population >() );
    entity_[ "roles/PHY_RoleAction_Loading" ].SetUserData( &pion.GetRole< transport::PHY_RoleAction_Loading >() );
    entity_[ "roles/PHY_RoleInterface_Transported" ].SetUserData( &pion.GetRole< transport::PHY_RoleInterface_Transported >() );
    entity_[ "movement/speed" ] = 0;
    entity_[ "movement/direction/x" ] = 0;
    entity_[ "movement/direction/y" ] = 1;
    entity_[ "movement/height" ] = 0;
    entity_[ "movement/environment/area" ] = 0;
    entity_[ "movement/environment/left" ] = 0;
    entity_[ "movement/environment/right" ] = 0;
    entity_[ "movement/environment/linear" ] = 0;
    entity_[ "identifier" ] = pion.GetID();
    entity_[ "pion" ].SetUserData( &pion );
    entity_[ "can-fly" ] = pion.GetType().GetUnitType().CanFly();
    entity_[ "is-autonomous" ] = pion.IsAutonomous();
    entity_[ "is-pc" ] = pion.IsPC();
    entity_[ "perceptions/peripherical-vision/activated" ] = false;
    entity_[ "perceptions/scan/activated" ] = false;
    entity_[ "perceptions/record-mode/activated" ] = false;
    entity_[ "perceptions/sensor/activated" ] = true;
    entity_[ "perceptions/radars/radar/activated" ] = false;
    entity_[ "perceptions/radars/tapping/activated" ] = false;
    entity_[ "perceptions/radars/tapping-radar/activated" ] = false;
    entity_[ "perceptions/localized-radars/radar/activated" ] = false;
    entity_[ "perceptions/localized-radars/radar/perception-id" ] = 0;
    entity_[ "perceptions/localized-radars/tapping/activated" ] = false;
    entity_[ "perceptions/localized-radars/tapping/perception-id" ] = 0;
    entity_[ "perceptions/localized-radars/tapping-radar/activated" ] = false;
    entity_[ "perceptions/localized-radars/tapping-radar/perception-id" ] = 0;
    entity_[ "perceptions/fire-observer/activated" ] = false;
    entity_[ "perceptions/flying-shell/activated" ] = false;
    entity_[ "perceptions/flying-shell/perception-id" ] = 0;
    entity_[ "perceptions/vision/mode" ] = "normal";
    entity_[ "perceptions/vision/location/x" ] = 0;
    entity_[ "perceptions/vision/location/y" ] = 1;
    entity_[ "perceptions/main-perception-direction/x" ] = 0;
    entity_[ "perceptions/main-perception-direction/y" ] = 1;
    entity_[ "fire/force-ratio/feedback-time" ] = pion.GetType().GetFeedbackTime();
}

// -----------------------------------------------------------------------------
// Name: RoleAdapter destructor
// Created: SLI 2012-01-16
// -----------------------------------------------------------------------------
RoleAdapter::~RoleAdapter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RoleAdapter destructor
// Created: SLI 2012-01-16
// -----------------------------------------------------------------------------
template< typename Archive >
void RoleAdapter::serialize( Archive& a, const unsigned int )
{
    a & boost::serialization::base_object< RoleAdapterInterface >( *this );
}

// -----------------------------------------------------------------------------
// Name: RoleAdapter::SendChangedState
// Created: SLI 2012-01-19
// -----------------------------------------------------------------------------
void RoleAdapter::SendChangedState() const
{
    std::for_each( senders_.begin(), senders_.end(), boost::bind( &network::NetworkMessageSender_ABC::SendChangedState, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: RoleAdapter::SendFullState
// Created: SLI 2012-01-19
// -----------------------------------------------------------------------------
void RoleAdapter::SendFullState( unsigned int context ) const
{
    std::for_each( senders_.begin(), senders_.end(), boost::bind( &network::NetworkMessageSender_ABC::SendFullState, _1, context ) );
}
