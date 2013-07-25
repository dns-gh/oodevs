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
#include "Sink.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "MT_Tools/MT_Logger.h"
#include <core/ModelListener_ABC.h>
#include <core/Model.h>
#include <core/UserData.h>
#include <boost/make_shared.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/optional.hpp>

using namespace sword;

BOOST_CLASS_EXPORT_IMPLEMENT( sword::RoleAdapter )

SWORD_USER_DATA_EXPORT( MIL_AgentPion* )
SWORD_USER_DATA_EXPORT( PHY_RoleInterface_Posture* )

namespace
{
    void Initialize( core::Model& entity, MIL_AgentPion& pion )
    {
        entity[ "data" ].SetUserData( &pion ); // $$$$ MCO 2012-09-13: move all initializations into Sink::CreateEntity
        entity[ "posture/data" ].SetUserData( &pion.GetRole< PHY_RoleInterface_Posture >() );
        entity[ "movement/speed" ] = 0;
        entity[ "movement/direction/x" ] = 0;
        entity[ "movement/direction/y" ] = 1;
        entity[ "movement/height" ] = 0;
        entity[ "movement/environment/area" ] = 0;
        entity[ "movement/environment/left" ] = 0;
        entity[ "movement/environment/right" ] = 0;
        entity[ "movement/environment/linear" ] = 0;
        entity[ "movement/intention" ] = false;
        entity[ "movement/speed-modifier" ] = pion.GetType().GetUnitType().GetSpeedModifier();
        entity[ "movement/foot-print-radius" ] = pion.GetType().GetUnitType().GetFootprintRadius();
        entity[ "movement/traffic-modifier" ] = 1.;
        entity[ "movement/has-been-teleported" ] = false;
        entity[ "identifier" ] = pion.GetID();
        entity[ "can-fly" ] = pion.GetType().GetUnitType().CanFly();
        entity[ "is-autonomous" ] = pion.IsAutonomous();
        entity[ "is-pc" ] = pion.IsPC();
        entity[ "is-civilian" ] = pion.IsCivilian();
        entity[ "can-emit" ] = true;
        entity[ "perceptions/cones" ];
        entity[ "perceptions/peripherical-vision/activated" ] = false;
        entity[ "perceptions/scan/activated" ] = false;
        entity[ "perceptions/record-mode/activated" ] = false;
        entity[ "perceptions/sensor/activated" ] = true;
        entity[ "perceptions/radars/radar/activated" ] = false;
        entity[ "perceptions/radars/tapping/activated" ] = false;
        entity[ "perceptions/radars/tapping-radar/activated" ] = false;
        entity[ "perceptions/radars/acquisitions" ];
        entity[ "perceptions/localized-radars/radar" ];
        entity[ "perceptions/localized-radars/tapping" ];
        entity[ "perceptions/localized-radars/tapping-radar" ];
        entity[ "perceptions/object-detection" ];
        entity[ "perceptions/recognition-point" ];
        entity[ "perceptions/flying-shell/zones" ];
        entity[ "perceptions/flying-shell/previous" ];
        entity[ "perceptions/reco" ];
        entity[ "perceptions/urban" ];
        entity[ "perceptions/alat/reco" ];
        entity[ "perceptions/alat/monitoring" ];
        entity[ "perceptions/fire-observer/activated" ] = false;
        entity[ "perceptions/vision/mode" ] = "normal";
        entity[ "perceptions/vision/location/x" ] = 0;
        entity[ "perceptions/vision/location/y" ] = 1;
        entity[ "perceptions/main-perception-direction/x" ] = 0;
        entity[ "perceptions/main-perception-direction/y" ] = 1;
        entity[ "perceptions/drill-blow/width" ] = pion.GetType().GetUnitType().GetCoupDeSondeWidth();
        entity[ "perceptions/drill-blow/length" ] = pion.GetType().GetUnitType().GetCoupDeSondeLength();
        entity[ "perceptions/max-agent-perception-distance" ] = 0;
        entity[ "perceptions/max-theoretical-agent-perception-distance" ] = 0;
        entity[ "perceptions/notifications/agents" ];
        entity[ "perceptions/notifications/agents-in-zone" ];
        entity[ "perceptions/notifications/objects" ];
        entity[ "perceptions/notifications/population-concentrations" ];
        entity[ "perceptions/notifications/population-flows" ];
        entity[ "perceptions/notifications/urban-blocks" ];
        entity[ "fire/force-ratio/feedback-time" ] = pion.GetType().GetFeedbackTime();
    }
}

// -----------------------------------------------------------------------------
// Name: RoleAdapter constructor
// Created: LDC 2013-01-10
// -----------------------------------------------------------------------------
RoleAdapter::RoleAdapter()
    : sink_  ( 0 )
    , pion_  ( 0 )
    , entity_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RoleAdapter constructor
// Created: SLI 2012-01-16
// -----------------------------------------------------------------------------
RoleAdapter::RoleAdapter( Sink& sink, MIL_AgentPion& pion, core::Model& entity )
    : sink_  ( &sink )
    , pion_  ( &pion )
    , entity_( &entity )
{
    Initialize( entity, pion );
}

// -----------------------------------------------------------------------------
// Name: RoleAdapter destructor
// Created: SLI 2012-01-16
// -----------------------------------------------------------------------------
RoleAdapter::~RoleAdapter()
{
    try
    {
        if( entity_ )
            entity_->Remove();
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Entity removal failed : " << e.what() );
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Entity removal failed for an unknown reason" );
    }
}

namespace
{
    bool Serialize( client::UnitPathFind& message, unsigned int identifier, const core::Model& points )
    {
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
    bool Serialize( client::UnitEnvironmentType& message, unsigned int identifier, const core::Model& environment )
    {
        message().mutable_unit()->set_id( identifier );
        message().set_area( environment[ "area" ] );
        message().set_left( environment[ "left" ] );
        message().set_right( environment[ "right" ] );
        message().set_linear( environment[ "linear" ] );
        return true;
    }

    template< typename Message >
    class ModelSender : public network::NetworkMessageSender_ABC, private core::ModelListener_ABC // $$$$ MCO 2012-09-13: factorize with ListenerHelper
    {
    public:
        ModelSender( Sink& sink, core::Model& model, unsigned int entity )
            : sink_  ( sink )
            , model_ ( &model )
            , entity_( entity )
        {
            sink_.Register( *model_, *this );
        }
        virtual ~ModelSender()
        {
            if( model_ )
                sink_.Unregister( *model_, *this );
        }
        virtual void SendChangedState() const
        {
            if( message_ )
                message_->Send( NET_Publisher_ABC::Publisher() );
            message_ = boost::none;
        }
        virtual void SendFullState( unsigned int context ) const
        {
            if( !model_ )
                return;
            Message message;
            if( Serialize( message, entity_, *model_ ) )
                message.Send( NET_Publisher_ABC::Publisher(), context );
        }
    private:
        virtual void NotifyChanged( const core::Model& model )
        {
            model_ = &model;
            Message message;
            if( Serialize( message, entity_, model ) )
                message_ = message;
        }
        virtual void NotifyRemoved( const core::Model& /*model*/ )
        {
            sink_.Unregister( *model_, *this );
            model_ = 0;
        }
    private:
        Sink& sink_;
        const core::Model* model_;
        unsigned int entity_;
        mutable boost::optional< Message > message_;
    };

    template< typename Message >
    boost::shared_ptr< network::NetworkMessageSender_ABC > MakeModelSender( Sink& sink, core::Model& model, unsigned int entity )
    {
        return boost::make_shared< ModelSender< Message > >( boost::ref( sink ), boost::ref( model ), entity );
    }
}

// -----------------------------------------------------------------------------
// Name: RoleAdapter::Finalize
// Created: BAX 2012-10-18
// -----------------------------------------------------------------------------
void RoleAdapter::Finalize()
{
    senders_.push_back( MakeModelSender< client::UnitPathFind >( boost::ref( *sink_ ), (*entity_)[ "movement/path/points" ], pion_->GetID() ) );
    senders_.push_back( MakeModelSender< client::UnitEnvironmentType >( boost::ref( *sink_ ), (*entity_)[ "movement/environment" ], pion_->GetID() ) ); // $$$$ MCO 2012-09-13: move the two listeners to RoleAction_Moving
}

// -----------------------------------------------------------------------------
// Name: RoleAdapter::load
// Created: SLI 2012-01-16
// -----------------------------------------------------------------------------
template< typename Archive >
void RoleAdapter::load( Archive& a, const unsigned int )
{
    a >> boost::serialization::base_object< RoleAdapterInterface >( *this );
    a >> entity_;
    a >> sink_;
    a >> pion_;
    Initialize( *entity_, *pion_ );
}

// -----------------------------------------------------------------------------
// Name: RoleAdapter::save
// Created: SLI 2012-01-16
// -----------------------------------------------------------------------------
template< typename Archive >
void RoleAdapter::save( Archive& a, const unsigned int ) const
{
    a << boost::serialization::base_object< RoleAdapterInterface >( *this );
    a << entity_;
    a << sink_;
    a << pion_;
}

// -----------------------------------------------------------------------------
// Name: RoleAdapter::SendChangedState
// Created: SLI 2012-01-19
// -----------------------------------------------------------------------------
void RoleAdapter::SendChangedState() const
{
    boost::for_each( senders_, boost::mem_fn( &network::NetworkMessageSender_ABC::SendChangedState ) );
}

// -----------------------------------------------------------------------------
// Name: RoleAdapter::SendFullState
// Created: SLI 2012-01-19
// -----------------------------------------------------------------------------
void RoleAdapter::SendFullState( unsigned int context ) const
{
    boost::for_each( senders_, boost::bind( &network::NetworkMessageSender_ABC::SendFullState, _1, context ) );
}
