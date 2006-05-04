// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "GraphicsPanel.h"
#include "moc_GraphicsPanel.cpp"
#include "GraphicPreferences.h"

// -----------------------------------------------------------------------------
// Name: GraphicsPanel constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicsPanel::GraphicsPanel( QWidget* parent )
    : QVBox     ( parent )
    , preferences_( *new GraphicPreferences() )
{
    preferences_.Display( this );

    QHBox* buttons = new QHBox( this );
    QPushButton* ok     = new QPushButton( tr( "Okay" ), buttons );
    QPushButton* apply  = new QPushButton( tr( "Appliquer" ), buttons );
    QPushButton* cancel = new QPushButton( tr( "Nope" ), buttons );
    ok->setDefault( true );

    connect( ok, SIGNAL( clicked() ), this, SLOT( OnOk() ) );
    connect( apply, SIGNAL( clicked() ), this, SLOT( OnApply() ) );
    connect( cancel, SIGNAL( clicked() ), this, SLOT( OnCancel() ) );
}
    
// -----------------------------------------------------------------------------
// Name: GraphicsPanel destructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicsPanel::~GraphicsPanel()
{
    delete &preferences_;
}

// -----------------------------------------------------------------------------
// Name: GraphicsPanel::GetPreferences
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
GraphicPreferences& GraphicsPanel::GetPreferences()
{
    return preferences_;
}

// -----------------------------------------------------------------------------
// Name: GraphicsPanel::OnOk
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicsPanel::OnOk()
{
    preferences_.Commit();
    parentWidget()->hide();
}

// -----------------------------------------------------------------------------
// Name: GraphicsPanel::OnApply
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicsPanel::OnApply()
{
    preferences_.Commit();
}

// -----------------------------------------------------------------------------
// Name: GraphicsPanel::OnCancel
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicsPanel::OnCancel()
{
    preferences_.Revert();
    parentWidget()->hide();
}
