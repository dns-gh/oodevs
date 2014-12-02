// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SelectionLayer.h"

#include "ColorStrategy_ABC.h"
#include "GLView_ABC.h"
#include "PickingSelector.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"

using namespace gui;

SelectionLayer::SelectionLayer( kernel::Controllers& controllers,
                                gui::GLView_ABC& tools,
                                ColorStrategy_ABC& strategy )
    : Layer( controllers, tools, eLayerTypes_Selection )
    , selected_( controllers )
    , strategy_( strategy )
{
    controllers_.Update( *this );
}

SelectionLayer::~SelectionLayer()
{
    controllers_.Unregister( *this );
}

void SelectionLayer::Paint( Viewport_ABC& viewport )
{
    if( !selected_ || !layer_ )
        return;
    strategy_.SetAlpha( layer_->GetAlpha() );
    layer_->Draw( *selected_, viewport, view_.GetPickingSelector().IsPickingMode() );
}

void SelectionLayer::Reset()
{
    selected_ = 0;
}

void SelectionLayer::NotifySelected( const kernel::Entity_ABC* element )
{
    selected_ = element;
    layer_ = element
        ? view_.GetLayer( [ element ]( const T_Layer& layer ) { return layer->IsIn( *element ); } )
        : T_Layer();
}
