// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "DrawingProxy.h"
#include "Drawing.h"
#include "DrawingsModel.h"

using namespace plugins::messenger;

// -----------------------------------------------------------------------------
// Name: DrawingProxy constructor
// Created: AGE 2008-07-09
// -----------------------------------------------------------------------------
DrawingProxy::DrawingProxy( DrawingsModel& model, std::unique_ptr< Drawing > drawing )
    : model_  ( model )
    , drawing_( std::move( drawing ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingProxy destructor
// Created: AGE 2008-07-09
// -----------------------------------------------------------------------------
DrawingProxy::~DrawingProxy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingProxy::GetCoordinates
// Created: AGE 2008-07-09
// -----------------------------------------------------------------------------
dispatcher::Zone DrawingProxy::GetCoordinates()
{
    return dispatcher::ToZone( drawing_->GetPoints(), drawing_->GetConverter() );
}

// -----------------------------------------------------------------------------
// Name: DrawingProxy::Publish
// Created: AGE 2008-07-09
// -----------------------------------------------------------------------------
void DrawingProxy::Publish()
{
    model_.Publish( *drawing_ );
}
