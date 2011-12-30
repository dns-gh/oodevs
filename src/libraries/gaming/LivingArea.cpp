// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LivingArea.h"
#include "UrbanModel.h"
#include "clients_kernel/LivingAreaVisitor_ABC.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "protocol/Protocol.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: LivingArea constructor
// Created: LGY 2011-04-29
// -----------------------------------------------------------------------------
LivingArea::LivingArea( const sword::PopulationCreation& message, unsigned long id, kernel::Controller& controller, const UrbanModel& model )
    : id_( id )
    , controller_( controller )
{
    for( int i = 0; i < message.objects_size(); ++i )
    {
        int id = message.objects( i ).id();
        gui::TerrainObjectProxy& block = model.GetObject( id );
        blocks_[ id ] = block.GetName();
    }
}

// -----------------------------------------------------------------------------
// Name: LivingArea destructor
// Created: LGY 2011-04-29
// -----------------------------------------------------------------------------
LivingArea::~LivingArea()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LivingArea::DoUpdate
// Created: LGY 2011-04-29
// -----------------------------------------------------------------------------
void LivingArea::DoUpdate( const sword::PopulationUpdate& message )
{
    for( int i = 0; i < message.occupations_size(); ++i )
    {
        const sword::PopulationUpdate_BlockOccupation& occupation = message.occupations( i );
        unsigned int id = occupation.object().id();
        CIT_Blocks it = blocks_.find( id );
        if( it != blocks_.end() )
        {
            gui::T_Human& humans = humans_[ id ];
            for( int j = 0; j < occupation.persons_size(); ++j )
                humans.persons_[ occupation.persons( j ).usage().c_str() ].first = occupation.persons( j ).number();
            humans.alerted_ = occupation.alerted();
            humans.confined_ = occupation.confined();
            humans.evacuated_ = occupation.evacuated();
            humans.angriness_ = occupation.angriness();
        }
    }
    controller_.Update( static_cast< kernel::LivingArea_ABC& >( *this ) );
}

// -----------------------------------------------------------------------------
// Name: LivingArea::Accept
// Created: LGY 2011-05-02
// -----------------------------------------------------------------------------
void LivingArea::Accept( kernel::LivingAreaVisitor_ABC& visitor ) const
{
    BOOST_FOREACH( const T_Blocks::value_type& block, blocks_ )
    {
        gui::T_HumansIdMap::const_iterator it = humans_.find( block.first );
        if( it != humans_.end() )
        {
            unsigned int resident = 0;
            BOOST_FOREACH( const gui::T_BlockOccupation::value_type& person, it->second.persons_ )
                resident += person.second.first;
            visitor.Visit( block.first, block.second, resident, it->second.alerted_, it->second.angriness_, it->second.confined_, it->second.evacuated_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LivingArea::GetId
// Created: LGY 2011-05-04
// -----------------------------------------------------------------------------
unsigned long LivingArea::GetId() const
{
    return id_;
}
