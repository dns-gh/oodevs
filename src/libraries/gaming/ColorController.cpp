// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ColorController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Color_ABC.h"

// -----------------------------------------------------------------------------
// Name: ColorController constructor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
ColorController::ColorController( kernel::Controllers& controllers )
    : controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorController destructor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
ColorController::~ColorController()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorController::Apply
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
QColor ColorController::Apply( const kernel::Entity_ABC& entity, const QColor& base )
{
    CIT_Colors it = colors_.find( entity.GetId() );
    if( it != colors_.end() )
        return it->second;
    return base;
}

// -----------------------------------------------------------------------------
// Name: ColorController::NotifyCreated
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
void ColorController::NotifyCreated( const kernel::Agent_ABC& agent )
{
    ChangeColor( agent );
}

// -----------------------------------------------------------------------------
// Name: ColorController::NotifyDeleted
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
void ColorController::NotifyDeleted( const kernel::Agent_ABC& agent )
{
    colors_.erase( agent.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ColorController::NotifyCreated
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
void ColorController::NotifyCreated( const kernel::Automat_ABC& automat )
{
    ChangeColor( automat );
}

// -----------------------------------------------------------------------------
// Name: ColorController::NotifyDeleted
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
void ColorController::NotifyDeleted( const kernel::Automat_ABC& automat )
{
    colors_.erase( automat.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ColorController::NotifyCreated
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
void ColorController::NotifyCreated( const kernel::Formation_ABC& formation )
{
    ChangeColor( formation );
}

// -----------------------------------------------------------------------------
// Name: ColorController::NotifyDeleted
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
void ColorController::NotifyDeleted( const kernel::Formation_ABC& formation )
{
    colors_.erase( formation.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ColorController::NotifyCreated
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
void ColorController::NotifyCreated( const kernel::Team_ABC& team )
{
    ChangeColor( team );
}

// -----------------------------------------------------------------------------
// Name: ColorController::NotifyDeleted
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
void ColorController::NotifyDeleted( const kernel::Team_ABC& team )
{
    colors_.erase( team.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ColorController::ChangeColor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
void ColorController::ChangeColor( const kernel::Entity_ABC& entity )
{
    if( const kernel::Color_ABC* pColor = entity.Retrieve< kernel::Color_ABC >() )
    {
        const kernel::Color_ABC::T_Color& color = pColor->GetColor();
        colors_[ entity.GetId() ] = QColor( color.get< 0 >(), color.get< 1 >(), color.get< 2 >() );
    }
}
