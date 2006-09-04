// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerCategory.h"
#include "moc_DrawerCategory.cpp"
#include "DrawerStyle.h"
#include "xeumeuleu/xml.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerCategory constructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerCategory::DrawerCategory( QWidget* parent, kernel::GlTools_ABC& tools, xml::xistream& input )
    : QVButtonGroup( parent )
{
    setExclusive( true );
    setMargin( 0 );
    setInsideMargin( 0 );
    setInsideSpacing( 0 );
    setBackgroundColor( Qt::white );
    std::string name, description;
    input >> xml::attribute( "name", name )
          >> xml::optional() >> xml::content( "description", description );
    ReadTemplates( input, tools );
    name_ = name.c_str();
    description_ = description.c_str();

    connect( this, SIGNAL( clicked( int ) ), this, SLOT( OnClicked( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: DrawerCategory destructor
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
DrawerCategory::~DrawerCategory()
{
    for( CIT_Styles it = styles_.begin(); it != styles_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: DrawerCategory::GetName
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
QString DrawerCategory::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: DrawerCategory::GetDescription
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
QString DrawerCategory::GetDescription() const
{
    return description_;
}

// -----------------------------------------------------------------------------
// Name: DrawerCategory::ReadTemplates
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
void DrawerCategory::ReadTemplates( xml::xistream& input, kernel::GlTools_ABC& tools )
{
    input >> xml::list( "template", *this, &DrawerCategory::ReadTemplate, tools );
}

// -----------------------------------------------------------------------------
// Name: DrawerCategory::ReadTemplate
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
void DrawerCategory::ReadTemplate( xml::xistream& input, kernel::GlTools_ABC& tools )
{
    DrawerStyle* style = new DrawerStyle( input, tools );
    QPushButton* button = new QPushButton( style->GetName(), this, style->GetName() );
    button->setToggleButton( true );
    button->setBackgroundColor( Qt::white );
    button->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Preferred );
    button->setFlat( true );
    QToolTip::add( button, style->GetDescription() );

    int id = insert( button );
    styles_[ id ] = style;
}

// -----------------------------------------------------------------------------
// Name: DrawerCategory::OnClicked
// Created: AGE 2006-09-01
// -----------------------------------------------------------------------------
void DrawerCategory::OnClicked( int id )
{
    DrawerStyle* style = styles_[ id ];
    if( style )
        emit Selected( *style );
}
