// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "DatabaseEditor.h"

#pragma warning( push, 0 )
#include <gdal/ogrsf_frmts.h>
#pragma warning( pop )

using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: DatabaseEditor constructor
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
DatabaseEditor::DatabaseEditor( OGRLayer& layer )
    : layer_ ( layer )
    , useTransaction_ ( false)
    , inTransaction_ ( false )
{
    useTransaction_ = ( /*cpl_port TRUE*/ 1 == layer.TestCapability( "Transactions" ) );
}

// -----------------------------------------------------------------------------
// Name: DatabaseEditor destructor
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
DatabaseEditor::~DatabaseEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DatabaseEditor::Lock
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
void DatabaseEditor::BeginTransaction()
{
    if( useTransaction_ && !InTransaction() )
    {
        layer_.StartTransaction();
        inTransaction_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseEditor::Lock
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
void DatabaseEditor::EndTransaction()
{
    if( useTransaction_ && InTransaction() )
    {
        layer_.CommitTransaction();
        inTransaction_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseEditor::Lock
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
bool DatabaseEditor::InTransaction() const
{
    return inTransaction_;
}
