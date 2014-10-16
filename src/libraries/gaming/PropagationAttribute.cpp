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
#include "Propagation.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/DisasterTypes.h"
#include "propagation/PropagationManager.h"
#include "protocol/Protocol.h"
#include <boost/smart_ptr/make_shared.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PropagationAttribute constructor
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
PropagationAttribute::PropagationAttribute( Controller& controller, const CoordinateConverter_ABC& converter,
                                            const kernel::ObjectType& type, const kernel::DisasterTypes& disasterTypes,
                                            const kernel::Time_ABC& simulation )
    : controller_  ( controller )
    , converter_   ( converter )
    , pManager_    ( new PropagationManager() )
    , simulation_  ( simulation )
    , disasterType_( disasterTypes.Get( type.GetDisasterType() ) )
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
    {
        sword::ObjectAttributePropagation attribute = message.attributes().propagation();
        pManager_->Initialize( tools::Path::FromUTF8( attribute.model() ), attribute.has_date() ? attribute.date() : "" );
    }
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute::NotifyUpdated
// Created: LGY 2012-10-16
// -----------------------------------------------------------------------------
void PropagationAttribute::NotifyUpdated( const Simulation::sEndTick& /*tick*/ )
{
    tools::Path::T_Paths files = pManager_->GetFiles( simulation_.GetDateTime().toString(  "yyyyMMdd'T'HHmmss" ).toStdString() );
    if( files_ != files )
    {
        propagations_.clear();
        for( std::size_t i = 0; i < files.size(); ++i )
            propagations_.push_back( boost::make_shared< Propagation >( files[ i ], *pManager_, converter_, disasterType_ ) );
        files_ = files;
    }
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute::Draw
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
void PropagationAttribute::Draw( const geometry::Point2f& /*where*/, const gui::Viewport_ABC& /*viewport*/, gui::GLView_ABC& /*tools*/ ) const
{
    for( auto it = propagations_.begin(); it != propagations_.end(); ++it )
        (*it)->Draw();
}
