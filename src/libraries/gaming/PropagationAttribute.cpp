// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PropagationAttribute.h"
#include "Simulation.h"
#include "Propagation.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PropagationAttribute constructor
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
PropagationAttribute::PropagationAttribute( Controller& controller, const CoordinateConverter_ABC& converter )
    : controller_( controller )
    , converter_ ( converter )
    , pManager_  ( new PropagationManager() )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute destructor
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
PropagationAttribute::~PropagationAttribute()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute::DoUpdate
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
void PropagationAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    if( message.attributes().has_propagation() )
        pManager_->Initialize( message.attributes().propagation().model() );
}

namespace
{
    bool Compare( const std::vector< std::string >& lhs, const std::vector< std::string >& rhs )
    {
        if( lhs.size() != rhs.size() )
            return false;
        for( std::size_t i = 0; i < rhs.size(); ++i )
            if( lhs[ i ] != rhs[ i ] )
                return false;
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute::NotifyUpdated
// Created: LGY 2012-10-16
// -----------------------------------------------------------------------------
void PropagationAttribute::NotifyUpdated( const Simulation& simulation )
{
    PropagationManager::T_Files files = pManager_->GetCurrentFiles( simulation.GetDateTime()
                                                            .toString(  "yyyyMMdd'T'HHmmss" ).toStdString() );

    if( ! Compare( files, currentFiles_ ) )
    {
        propagations_.clear();
        for( std::size_t i = 0; i < files.size(); ++i )
            propagations_.push_back( boost::shared_ptr< Propagation >( new Propagation( files[ i ], *pManager_, converter_ ) ) );
        currentFiles_ = files;
    }
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute::Draw
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
void PropagationAttribute::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& /*viewport*/, const kernel::GlTools_ABC& /*tools*/ ) const
{
    for( CIT_Propagations it = propagations_.begin(); it != propagations_.end(); ++it )
        (*it)->Draw();
}
