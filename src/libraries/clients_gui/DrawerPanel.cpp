// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerPanel.h"
#include "moc_DrawerPanel.cpp"
#include "xeumeuleu/xml.h"
#include <qtoolbox.h>
#include "DrawerCategory.h"
#include "DrawerLayer.h"
#include "ColorButton.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerPanel constructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerPanel::DrawerPanel( QWidget* parent, DrawerLayer& layer, kernel::GlTools_ABC& tools )
    : QVBox( parent, "Drawer" )
    , layer_( layer )
{
    color_ = new ColorButton( this, "" );
    toolBox_ = new QToolBox( this );
    toolBox_->setMargin( 0 );
    toolBox_->setBackgroundColor( Qt::white );
    ReadTemplates( tools );
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel destructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerPanel::~DrawerPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::ReadTemplates
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
void DrawerPanel::ReadTemplates( kernel::GlTools_ABC& tools )
{
    xml::xifstream input( "DrawingTemplates.xml" ); // $$$$ AGE 2006-09-01: 
    input >> xml::start( "templates" )
            >> xml::list( "category", *this, &DrawerPanel::ReadCategory, tools )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::ReadCategory
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
void DrawerPanel::ReadCategory( xml::xistream& input, kernel::GlTools_ABC& tools )
{
    DrawerCategory* category = new DrawerCategory( this, tools, input );
    connect( category, SIGNAL( Selected( DrawerStyle& ) ), this, SLOT( OnSelect( DrawerStyle& ) ) );
    int id = toolBox_->addItem( category, category->GetName() );
    toolBox_->setItemToolTip ( id, category->GetDescription() );
}

// -----------------------------------------------------------------------------
// Name: DrawerPanel::OnSelect
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
void DrawerPanel::OnSelect( DrawerStyle& style )
{
    color_->Commit();
    layer_.StartShape( style, color_->GetColor() );
}
