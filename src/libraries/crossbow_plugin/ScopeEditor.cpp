// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "esri_plugin_pch.h"
#include "ScopeEditor.h"
#include "Database_ABC.h"

using namespace plugins;

// -----------------------------------------------------------------------------
// Name: ScopeEditor constructor
// Created: JCR 2007-12-06
// -----------------------------------------------------------------------------
esri::ScopeEditor::ScopeEditor( esri::Database_ABC& database )
    : database_ ( database )
{
    // database_.Lock();
    database_.StartEdit();
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor destructor
// Created: JCR 2007-12-06
// -----------------------------------------------------------------------------
esri::ScopeEditor::~ScopeEditor()
{
    database_.StopEdit();
}
