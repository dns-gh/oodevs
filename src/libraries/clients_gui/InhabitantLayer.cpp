// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "InhabitantLayer.h"
#include "View_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Positions.h"
#include "preparation/InhabitantPositions.h"
#include <boost/noncopyable.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EntityLayerBase::InhabitantLayer
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
InhabitantLayer::InhabitantLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile )
    : tools_( tools )
    , strategy_( strategy )
    , view_( view )
    , selectedInhab_( 0 )
{
    controllers.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantLayer::~InhabitantLayer
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
InhabitantLayer::~InhabitantLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantLayer::Paint
// Created: SLG 2006-03-23
// -----------------------------------------------------------------------------
void InhabitantLayer::Paint( kernel::Viewport_ABC& viewport )
{
    if( selectedInhab_ )
    {
        strategy_.SelectColor( *selectedInhab_ );
        selectedInhab_->Draw( tools_ );
    }
}

// -----------------------------------------------------------------------------
// Name: EditorLayer::Reset
// Created: RPD 2009-10-14
// -----------------------------------------------------------------------------
void InhabitantLayer::Reset2d()
{
    selectedInhab_ = 0;
    Layer_ABC::Reset2d();
}

// -----------------------------------------------------------------------------
// Name: EntityLayer::NotifySelected
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
void InhabitantLayer::NotifySelected( const kernel::Inhabitant_ABC* inhab )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantLayer::BeforeSelection
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void InhabitantLayer::BeforeSelection()
{
    selectedInhab_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantLayer::AfterSelection
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void InhabitantLayer::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantLayer::Select
// Created: SLG 2010-11-26
// -----------------------------------------------------------------------------
void InhabitantLayer::Select( const kernel::Inhabitant_ABC& element )
{
    selectedInhab_ = &element;
}

// -----------------------------------------------------------------------------
// Name: InhabitantLayer::NotifyActivated
// Created: SLG 2010-12-02
// -----------------------------------------------------------------------------
void InhabitantLayer::NotifyActivated( const kernel::Inhabitant_ABC& element )
{
    view_.CenterOn( element.Get< kernel::Positions >().GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: InhabitantLayer::NotifyDeleted
// Created: SLG 2010-12-02
// -----------------------------------------------------------------------------
void InhabitantLayer::NotifyDeleted( const kernel::Inhabitant_ABC& element )
{
    selectedInhab_ = 0;
}
