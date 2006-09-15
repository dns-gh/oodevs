// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "CrossingSitePrototype.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Object_ABC.h"
#include "preparation/CrossingSiteAttributes.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype constructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype::CrossingSitePrototype( QWidget* parent, Object_ABC*& creation )
    : CrossingSitePrototype_ABC( parent )
    , creation_( creation )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype destructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype::~CrossingSitePrototype()
{
    Clean();
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CrossingSitePrototype::Commit()
{
    if( !creation_ )
        return;

    CrossingSiteAttributes& attr = static_cast< CrossingSiteAttributes& >( creation_->Get< CrossingSiteAttributes_ABC >() );

    attr.SetWidth( width_->value() );
    attr.SetDepth( depth_->value() );
    attr.SetFlowSpeed( speed_->value() );
    attr.SetConstruction( needsConstruction_->isOn() );
}

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void CrossingSitePrototype::Clean()
{
    // NOTHING
}
