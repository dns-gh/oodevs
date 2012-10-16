// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DirectFirePerceptionEventListener.h"
#include "AlgorithmsFactories.h"
#include "OnComponentFunctor_ABC.h"
#include "OnComponentComputer_ABC.h"
#include "OnComponentFunctorComputerFactory_ABC.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Sensors/PHY_Sensor.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "MT_Tools/MT_Logger.h"
#include <core/Facade.h>
#include <core/Model.h>

using namespace sword;

namespace
{
    const std::string event = "direct fire pion";
}

//-----------------------------------------------------------------------------
// Name: DirectFirePerceptionEventListener constructor
// Created: MCO 2012-04-26
//-----------------------------------------------------------------------------
DirectFirePerceptionEventListener::DirectFirePerceptionEventListener( core::Facade& facade )
    : facade_( facade )
{
    facade.Register( event, *this );
}

//-----------------------------------------------------------------------------
// Name: DirectFirePerceptionEventListener destructor
// Created: MCO 2012-04-26
//-----------------------------------------------------------------------------
DirectFirePerceptionEventListener::~DirectFirePerceptionEventListener()
{
    facade_.Unregister( event, *this );
}

namespace
{
    class SensorFunctor : private boost::noncopyable
    {
    public:
        SensorFunctor(const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target )
            : perceiver_( perceiver ), target_( target ), isInside_( false )
        {}
        void operator() ( const PHY_Sensor& sensor )
        {
            const PHY_SensorTypeAgent* sensorTypeAgent = sensor.GetType().GetTypeAgent();
            if( sensorTypeAgent )
            {
                MT_Vector2D targetPosition = target_.GetRole< PHY_RoleInterface_Location >().GetPosition();
                MT_Vector2D perceiverPosition = perceiver_.GetRole< PHY_RoleInterface_Location >().GetPosition();
                isInside_ = sensorTypeAgent->CanDetectFirer( targetPosition.Distance( perceiverPosition ) );
            }
        }
        bool IsInside(){ return isInside_; }
    private:
        const MIL_Agent_ABC& perceiver_;
        const MIL_Agent_ABC& target_;
        bool                 isInside_;
    };
    class Functor : public OnComponentFunctor_ABC
    {
    public:
        Functor( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target )
            : perceiver_( perceiver )
            , transport_( perceiver.RetrieveRole< transport::PHY_RoleAction_Loading >() )
            , target_( target )
            , isInside_( false )
        {}
        void operator() ( PHY_ComposantePion& composante )
        {
            if( !composante.CanPerceive( transport_ ) )
                return;
            SensorFunctor dataFunctor( perceiver_, target_ );
            composante.ApplyOnSensors( dataFunctor );
            if( !isInside_)
                isInside_ = dataFunctor.IsInside();
        }
        bool IsInside(){ return isInside_; }

    private:
        const MIL_Agent_ABC& perceiver_;
        const transport::PHY_RoleAction_Loading* transport_;
        const MIL_Agent_ABC& target_;
        bool isInside_;
    };
    void NotifyFirerPerception( MIL_AgentPion& pion, MIL_Agent_ABC& target ) // $$$$ MCO 2012-05-02: see how to hook this with perception module
    {
        Functor dataFunctor( target, pion );
        std::auto_ptr< OnComponentComputer_ABC > dataComputer( target.GetAlgorithms().onComponentFunctorComputerFactory_->Create( dataFunctor ) );
        target.Execute( *dataComputer );
        if( dataFunctor.IsInside() )
        {
            PHY_RoleInterface_Perceiver& role = target.GetRole< PHY_RoleInterface_Perceiver >();
            role.NotifyExternalPerception( pion, PHY_PerceptionLevel::recognized_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DirectFirePerceptionEventListener::Notify
// Created: MCO 2012-04-26
// -----------------------------------------------------------------------------
void DirectFirePerceptionEventListener::Notify( const core::Model& event )
{
    try
    {
        if( event[ "running" ] )
        {
            MIL_AgentPion& pion = event[ "entity/data" ].GetUserData< MIL_AgentPion >();
            MIL_Agent_ABC& target = event[ "enemy/data" ].GetUserData< boost::shared_ptr< DEC_Knowledge_Agent > >()->GetAgentKnown();
            NotifyFirerPerception( pion, target ); // $$$$ MCO 2012-09-10: move to a separate listener
        }
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Exception in " << __FUNCTION__ << " : " << e.what() );
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Unknown exception in " << __FUNCTION__ );
    }
}