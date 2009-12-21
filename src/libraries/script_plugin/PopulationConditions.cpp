// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "PopulationConditions.h"

#include "AgentEvents.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "directia/Brain.h"
#include "dispatcher/Population.h"
#include "dispatcher/PopulationConcentration.h"
#include "dispatcher/PopulationFlow.h"
#include "PopulationManipulator.h"
#include "SimpleEntityCondition.h"

using namespace plugins::script;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationConditions constructor
// Created: SBO 2008-11-18
// -----------------------------------------------------------------------------
PopulationConditions::PopulationConditions( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter )
    : controller_( controller )
    , converter_ ( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationConditions destructor
// Created: SBO 2008-11-18
// -----------------------------------------------------------------------------
PopulationConditions::~PopulationConditions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationConditions::RegisterIn
// Created: SBO 2008-11-18
// -----------------------------------------------------------------------------
void PopulationConditions::RegisterIn( directia::Brain& brain )
{
    brain.RegisterObject( "events.populations", this );
    brain.RegisterFunction( "PopulationEnters", &PopulationConditions::PopulationEnters );
}

namespace directia
{
    template< > inline void UsedByDIA    ( dispatcher::Population* ) { }
    template< > inline void ReleasedByDIA( dispatcher::Population* ) { }
}

namespace
{
    struct PositionComputer : public kernel::ModelVisitor_ABC
    {
        explicit PositionComputer( const kernel::CoordinateConverter_ABC& converter )
            : converter_( &converter )
        {}

        virtual void Visit( const kernel::Entity_ABC& entity )
        {
            if( const dispatcher::PopulationConcentration* c = dynamic_cast< const dispatcher::PopulationConcentration* >( &entity ) )
            {
                const geometry::Point2f p = converter_->ConvertToXY( c->position_ );
                points_.push_back( p );
                box_.Incorporate( p );
            }
            else if( const dispatcher::PopulationFlow* f = dynamic_cast< const dispatcher::PopulationFlow* >( &entity ) )
            {
                ASN1T_Location location;
                f->flow_.Send( location );
                for( unsigned int i = 0; i < location.coordinates.n; ++i )
                {
                    const geometry::Point2f p = converter_->ConvertToXY( location.coordinates.elem[i] );
                    points_.push_back( p );
                    box_.Incorporate( p );
                }
            }
        }

        const kernel::CoordinateConverter_ABC* converter_;
        std::vector< geometry::Point2f > points_;
        geometry::Rectangle2f box_;
    };

    class Enters : public SimpleEntityCondition< events::PopulationMoved >
    {
    public:
        Enters( const Zone& zone, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter )
            : SimpleEntityCondition< events::PopulationMoved >( controller )
            , zone_      ( ToPolygon( zone ) )
            , box_       ( zone_.BoundingBox() )
            , converter_ ( converter )
        {
            // NOTHING
        }

        virtual void NotifyUpdated( const events::PopulationMoved& move )
        {
            PositionComputer position( converter_ );
            move.entity.Accept( position );
            if( ! box_.Intersect( position.box_ ).IsEmpty() )
                for( unsigned int i = 0; i < position.points_.size(); ++i )
                    if( zone_.IsInside( position.points_[i] ) )
                    {
                        Trigger( move.entity );
                        return;
                    }
        }

    private:
        Enters& operator=( const Enters& );

    private:
        geometry::Polygon2f zone_;
        geometry::Rectangle2f box_;
        const kernel::CoordinateConverter_ABC& converter_;
    };
}

// -----------------------------------------------------------------------------
// Name: PopulationConditions::PopulationEnters
// Created: SBO 2008-11-18
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > PopulationConditions::PopulationEnters( const dispatcher::Zone& zone )
{
    return boost::shared_ptr< Condition_ABC >( new ::Enters( zone, controller_, converter_ ) );
}
