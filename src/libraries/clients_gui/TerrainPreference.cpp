// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TerrainPreference.h"
#include "ColorButton.h"
#include "SizeButton.h"
#include "xeumeuleu/xml.h"
using namespace xml;

// -----------------------------------------------------------------------------
// Name: TerrainPreference constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
TerrainPreference::TerrainPreference( xml::xistream& xis )
{
    std::string color;
    xis >> attribute( "name", name_ )
        >> content( "width", lineWidth_ )
        >> content( "color", color );
    color_ = QColor( color.c_str() );
}
    
// -----------------------------------------------------------------------------
// Name: TerrainPreference destructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
TerrainPreference::~TerrainPreference()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::Display
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void TerrainPreference::Display( QWidget* parent )
{
    QHBox* pBox = new QHBox( parent );
    new QLabel( name_.c_str(), pBox );
    sizeButton_  = new SizeButton ( pBox, "Line width: ", lineWidth_ );
    sizeButton_->EnableValueLabel( " px" );
    colorButton_ = new ColorButton( pBox, "", color_ );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::SetLineWidth
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void TerrainPreference::SetLineWidth() const
{
    const float lineWidth = sizeButton_->GetSize();
    glLineWidth( lineWidth );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::SetColor
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void TerrainPreference::SetColor( float alpha ) const
{
    const QColor color = colorButton_->GetColor();
    glColor4f( color.red()/255.f, color.green()/255.f, color.blue()/255.f, alpha );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::Commit
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void TerrainPreference::Commit()
{
    colorButton_->Commit();
    sizeButton_->Commit();
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::Revert
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void TerrainPreference::Revert()
{
    colorButton_->Revert();
    sizeButton_->Revert();
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::Save
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void TerrainPreference::Save( xml::xostream& xos ) const
{
    xos << attribute( "name", name_ )
        << content( "width", sizeButton_->GetSize() )
        << content( "color", colorButton_->GetColor().name() );
}
