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
#include "dispatcher/Population.h"
#include "dispatcher/PopulationConcentration.h"
#include "dispatcher/PopulationFlow.h"
#include "PopulationManipulator.h"
#include "SimpleEntityCondition.h"
#include <directia/brain/Brain.h>
#include <boost/make_shared.hpp>

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
void PopulationConditions::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "events.populations" ] = this;
    brain.Register( "PopulationEnters", &PopulationConditions::PopulationEnters );
    brain.Register( "PopulationKnowledgeCreated", &PopulationConditions::PopulationKnowledgeCreated );
}

namespace directia
{
    void UsedByDIA    ( dispatcher::Population* ) { }
    void ReleasedByDIA( dispatcher::Population* ) { }
    void UsedByDIA( PopulationConditions* ) {}
    void ReleasedByDIA( PopulationConditions* ) {}
}

namespace
{
    struct PositionComputer : public kernel::ModelVisitor_ABC
    {
        explicit PositionComputer( const kernel::CoordinateConverter_ABC& converter )
            : converter_( &converter )
        {}

        virtual void Visit( const kernel::EntityBase_ABC& entity )
        {
            if( const dispatcher::PopulationConcentration_ABC* c = dynamic_cast< const dispatcher::PopulationConcentration_ABC* >( &entity ) )
            {
                const geometry::Point2f p = converter_->ConvertToXY( c->GetPosition() );
                points_.push_back( p );
                box_.Incorporate( p );
            }
            else if( const dispatcher::PopulationFlow_ABC* f = dynamic_cast< const dispatcher::PopulationFlow_ABC* >( &entity ) )
            {
                sword::Location location;
                f->GetLocation().Send( location );
                for( int i = 0; i < location.coordinates().elem_size(); ++i )
                {
                    const geometry::Point2f p = converter_->ConvertToXY( location.coordinates().elem(i) );
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
    return boost::make_shared< ::Enters >( zone, controller_, converter_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationConditions::PopulationKnowledgeCreated
// Created: DSO 2010-07-27
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > PopulationConditions::PopulationKnowledgeCreated()
{
    return boost::shared_ptr< Condition_ABC >( new SimpleEntityCondition< events::PopulationKnowledgeCreated >( controller_ ) );
}

