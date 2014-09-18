// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MeteoModel.h"
#include "Config.h"
#include "Model.h"
#include "EntityPublisher.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "meteo/Meteo.h"
#include "meteo/MeteoLocal.h"
#include "protocol/ClientSenders.h"
#include <QtCore/qstring.h>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MeteoModel constructor
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
MeteoModel::MeteoModel( kernel::CoordinateConverter_ABC& converter, const Config& config, Model& model )
    : weather::MeteoModel_ABC( converter )
    , model_       ( model )
    , config_      ( config )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MeteoModel destructor
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
MeteoModel::~MeteoModel()
{
    // NOTHING
}

namespace
{
    class MeteoGlobal : public weather::Meteo
                      , public kernel::Entity_ABC
    {
    public:
        MeteoGlobal( unsigned int id, const sword::WeatherAttributes& attributes, unsigned int timeStep )
            : weather::Meteo( id, attributes, timeStep )
        {
            // NOTHING
        }
        ~MeteoGlobal()
        {
            // NOTHING
        }

        //! @name Entity implementation
        //@{
        virtual QString GetName() const { return name_.c_str(); }
        virtual unsigned long GetId() const { return id_; }
        virtual void Select( kernel::ActionController& ) const {}
        virtual void MultipleSelect( kernel::ActionController&, const std::vector< const kernel::GraphicalEntity_ABC* >& ) const {}
        virtual void ContextMenu( kernel::ActionController&, const QPoint&, QObject* ) const {}
        virtual void Activate( kernel::ActionController& ) const {}
        virtual void SendFullUpdate( dispatcher::ClientPublisher_ABC& publisher ) const { SendCreation( publisher ); }
        //@}
    };

    class MeteoLocal : public weather::MeteoLocal
                     , public kernel::Entity_ABC
    {
    public:
        MeteoLocal( const sword::ControlLocalWeatherCreation& msg, unsigned int timeStep, const std::string& name )
            : weather::MeteoLocal( msg, timeStep, name )
        {
            // NOTHING
        }
        ~MeteoLocal()
        {
            // NOTHING
        }

        //! @name Entity implementation
        //@{
        virtual QString GetName() const { return name_.c_str(); }
        virtual unsigned long GetId() const { return id_; }
        virtual void Select( kernel::ActionController& ) const {}
        virtual void MultipleSelect( kernel::ActionController&, const std::vector< const kernel::GraphicalEntity_ABC* >& ) const {}
        virtual void ContextMenu( kernel::ActionController&, const QPoint&, QObject* ) const {}
        virtual void Activate( kernel::ActionController& ) const {}
        //@}
    };
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::Accept
// Created: HBD 2010-03-31
// -----------------------------------------------------------------------------
void MeteoModel::Accept( kernel::ModelVisitor_ABC& visitor )
{
    if( MeteoGlobal* global = static_cast< MeteoGlobal* >( globalMeteo_.get() ) )
        visitor.Visit( *global );
    for( auto it = meteos_.begin(); it != meteos_.end(); ++it )
        visitor.Visit( static_cast< MeteoLocal& >( **it ) );
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgGlobalMeteo
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgGlobalMeteo( const sword::ControlGlobalWeather& msg )
{
    if( MeteoGlobal* global = static_cast< MeteoGlobal* >( globalMeteo_.get() ) )
    {
        globalMeteo_->Update( msg.attributes() );
        static_cast< kernel::Entity_ABC* >( global )->Update( msg );
    }
    else
    {
        MeteoGlobal* meteo = new MeteoGlobal( msg.weather().id(), msg.attributes(), config_.GetTickDuration() );
        globalMeteo_.reset( meteo );
        model_.AddExtensions( *meteo );
    }
 }

namespace
{
    geometry::Point2f ToPoint( const sword::CoordLatLong& coord )
    {
        return geometry::Point2f( static_cast< float >( coord.longitude() ), static_cast< float >( coord.latitude() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgLocalMeteo
// Created: HBD 2010-03-23
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteoCreation( const sword::ControlLocalWeatherCreation& msg )
{
    if( !msg.has_attributes() )
        return;
    weather::Meteo* meteo = Find( msg.weather().id() );
    if( meteo )
    {
        meteo->Update( msg.attributes() );
        static_cast< weather::MeteoLocal* >( meteo )->SetPosition( ToPoint( msg.top_left() ), ToPoint( msg.bottom_right() ) );
    }
    else
    {
        MeteoLocal* weather = new MeteoLocal( msg, config_.GetTickDuration(), "" );
        model_.AddExtensions( *weather );
        AddMeteo( *weather );
    }
}

// -----------------------------------------------------------------------------
// Name: MeteoModel::OnReceiveMsgGlobalMeteoDestruction
// Created: HBD 2010-04-02
// -----------------------------------------------------------------------------
void MeteoModel::OnReceiveMsgLocalMeteoDestruction( const sword::ControlLocalWeatherDestruction& message )
{
    for( IT_Meteos it = meteos_.begin(); it != meteos_.end(); ++it )
        if( (*it)->GetId() == message.weather().id() )
        {
            meteos_.erase( it );
            return;
        }
}
