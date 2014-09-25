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
#include "moc_TerrainPreference.cpp"
#include "ColorButton.h"
#include "RichCheckBox.h"
#include "SizeButton.h"

#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"

#include "ENT/Ent_Tr.h"

#include <xeumeuleu/xml.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainPreference constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
TerrainPreference::TerrainPreference( xml::xistream& xis, kernel::Controllers& controllers )
    : controllers_( controllers )
    , options_    ( controllers_.options_ )
    , type_       ( xis.attribute< std::string >( "type" ) )
    , name_       ( xis.attribute( "name", type_ ) )
    , currentShown_ ( true )
    , previousShown_ ( true )
    , colorButton_( 0 )
    , showCheckbox_( 0 )
    , sizeButton_( 0 )
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
    SubObjectName subObject( name_.c_str() );
    Q3HBox* pBox = new Q3HBox( parent );
    pBox->setSpacing( 5 );
    if( parent->layout() )
        parent->layout()->addWidget( pBox );
    showCheckbox_ = new RichCheckBox( "showCheckbox", ENT_Tr::ConvertFromLocation( ENT_Tr::ConvertToLocation( name_ ), ENT_Tr::eToTr ).c_str(), pBox );
    showCheckbox_->setChecked( previousShown_ );
    connect( showCheckbox_, SIGNAL( stateChanged( int ) ), this, SLOT( OnCategoryChecked() ) );
    pBox->setStretchFactor( showCheckbox_, 2 );
    sizeButton_  = new SizeButton( "sizeButton", pBox, "", lineWidth_ );
    sizeButton_->EnableValueLabel( tools::translate( "gui::TerrainPreference", " px" ) );
    colorButton_ = new ColorButton( "colorButton", pBox, "", color_ );
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
    glColor4f( color.red() / 255.f, color.green() / 255.f, color.blue() / 255.f, currentShown_ ? alpha : 0.f );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::Commit
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void TerrainPreference::Commit()
{
    colorButton_->Commit();
    sizeButton_->Commit();
    currentShown_ = showCheckbox_->isChecked();
    previousShown_ = currentShown_;
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::Revert
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void TerrainPreference::Revert()
{
    colorButton_->Revert();
    sizeButton_->Revert();
    showCheckbox_->setChecked( previousShown_ );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::Save
// Created: SBO 2008-08-18
// -----------------------------------------------------------------------------
void TerrainPreference::Save() const
{
    options_.Change( "Terrains/" + type_ + "/Width", sizeButton_->GetSize() );
    options_.Change( "Terrains/" + type_ + "/Color", colorButton_->GetColor().name() );
    options_.Change( "Terrains/" + type_ + "/Shown", showCheckbox_->isChecked() );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::OptionChanged
// Created: SBO 2008-08-18
// -----------------------------------------------------------------------------
void TerrainPreference::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( !colorButton_ || !showCheckbox_ || !sizeButton_ )
        return;

    const QString root( QString( "Terrains/" ) + type_.c_str() );
    QString option( name.c_str() );
    if( !option.startsWith( root ) )
        return;
    option.remove( root );
    if( option == "/Width" )
    {
        sizeButton_->SetSize( value.To< float >() );
        sizeButton_->Commit();
    }
    else if( option == "/Color" )
    {
        colorButton_->SetColor( QColor( value.To< QString >() ) );
        colorButton_->Commit();
    }
    else if( option == "/Shown" )
    {
        currentShown_ = value.To< bool >();
        showCheckbox_->setChecked( currentShown_ );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::OnCategoryChecked
// Created: NPT 2013-03-25
// -----------------------------------------------------------------------------
void TerrainPreference::OnCategoryChecked()
{
    currentShown_ = showCheckbox_->isChecked();
}

