// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "MinePrototype.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Object_ABC.h"
#include "preparation/MineAttributes.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MinePrototype constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MinePrototype::MinePrototype( QWidget* parent, kernel::Object_ABC*& creation )
    : MinePrototype_ABC( parent )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MinePrototype destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MinePrototype::~MinePrototype()
{
    Clean();
}

// -----------------------------------------------------------------------------
// Name: MinePrototype::Commit
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MinePrototype::Commit()
{
    if( !creation_ )
        return;

    MineAttributes& attr = static_cast< MineAttributes& >( creation_->Get< MineAttributes_ABC >() );
    attr.SetDensity( density_->text().toFloat() );
    attr.SetActivityTime( activityTime_->value() );
}

// -----------------------------------------------------------------------------
// Name: MinePrototype::Clean
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MinePrototype::Clean()
{
    // NOTHING
}
