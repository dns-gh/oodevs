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
#include "Tools.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Controllers.h"
#include <xeumeuleu/xml.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainPreference constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
TerrainPreference::TerrainPreference( xml::xistream& xis, kernel::Controllers& controllers )
    : controllers_( controllers )
    , options_( controllers_.options_ )
    , type_( xml::attribute< std::string >( xis, "type" ) )
    , name_( xml::attribute< std::string >( xis, "name" ) )
{
    std::string color;
    xis >> xml::content( "color", color )
        >> xml::content( "width", lineWidth_ );
    color_ = QColor( color.c_str() );
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: TerrainPreference destructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
TerrainPreference::~TerrainPreference()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::Display
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void TerrainPreference::Display( QWidget* parent )
{
    QHBox* pBox = new QHBox( parent );
    new QLabel( name_.c_str(), pBox );
    sizeButton_  = new SizeButton ( pBox, tools::translate( "TerrainPreference", "Line thickness: " ), lineWidth_ );
    sizeButton_->EnableValueLabel( tools::translate( "TerrainPreference", " px" ) );
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
    glColor4f( color.red() / 255.f, color.green() / 255.f, color.blue() / 255.f, alpha );
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
// Created: SBO 2008-08-18
// -----------------------------------------------------------------------------
void TerrainPreference::Save() const
{
    options_.Change( "Terrains/" + type_ + "/width", sizeButton_->GetSize() );
    options_.Change( "Terrains/" + type_ + "/color", colorButton_->GetColor().name() );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::OptionChanged
// Created: SBO 2008-08-18
// -----------------------------------------------------------------------------
void TerrainPreference::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    const QString root( QString( "Terrains/" ) + type_.c_str() );
    QString option( name.c_str() );
    if( !option.startsWith( root ) )
        return;
    option.remove( root );
    if( option == "/width" )
    {
        sizeButton_->SetSize( value.To< float >() );
        sizeButton_->Commit();
    }
    else if( option == "/color" )
    {
        colorButton_->SetColor( QColor( value.To< QString >() ) );
        colorButton_->Commit();
    }
}
