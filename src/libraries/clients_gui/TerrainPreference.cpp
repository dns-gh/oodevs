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
#include "SizeButton.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Controllers.h"
#include <xeumeuleu/xml.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainPreference constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
TerrainPreference::TerrainPreference( xml::xistream& xis, kernel::Controllers& controllers )
    : controllers_      ( controllers )
    , options_          ( controllers_.options_ )
    , type_             ( xis.attribute< std::string >( "type" ) )
    , name_             ( xis.attribute( "name", type_ ) )
    , previousShown_    ( true )
    , shown_            ( true )
    , colorButton_      ( 0 )
    , sizeButton_       ( 0 )
    , groupBox_         ( 0 )
    , showCheckbox_     ( 0 )
    , lineWidth_        ( xis.content< float >( "width" ) )
    , previousLineWidth_( lineWidth_ )
    , color_            ( xis.content< std::string >( "color" ).c_str() )
    , previousColor_    ( color_ )
{
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
void TerrainPreference::Display( QVBoxLayout* parent, QCheckBox* box )
{
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins( 15, 0, 0, 0 );
    Q3HBox* pBox = new Q3HBox();
    showCheckbox_ = new QCheckBox( ENT_Tr::ConvertFromLocation( ENT_Tr::ConvertToLocation( name_ ), ENT_Tr::eToTr ).c_str(), pBox );
    showCheckbox_->setChecked( shown_ );
    box->setChecked( shown_ || box->isChecked() );
    connect( box, SIGNAL( toggled( bool ) ), showCheckbox_, SLOT( setChecked( bool ) ) );
    connect( showCheckbox_, SIGNAL( stateChanged( int ) ), this, SLOT( OnStateChanged( int ) ) );
    pBox->setStretchFactor( showCheckbox_, 2 );
    sizeButton_  = new SizeButton( "terrain-line-thickness", pBox, "", lineWidth_ );
    sizeButton_->EnableValueLabel( tools::translate( "gui::TerrainPreference", " px" ) );
    connect( sizeButton_, SIGNAL( valueChanged( int ) ), this, SLOT( OnSizeChanged( int ) ) );
    colorButton_ = new ColorButton( "terrain-color", pBox, "", color_ );
    connect( colorButton_, SIGNAL( ColorChanged( const QColor& ) ), this, SLOT( OnColorChanged( const QColor& ) ) );
    connect( box, SIGNAL( toggled( bool ) ), this, SLOT( SetEnabled( bool ) ) );
    layout->addWidget( pBox );
    parent->addLayout( layout );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::SetEnabled
// Created: LGY 2014-06-20
// -----------------------------------------------------------------------------
void TerrainPreference::SetEnabled( bool value )
{
    if( showCheckbox_ && colorButton_ && sizeButton_ )
    {
        showCheckbox_->setEnabled( value );
        colorButton_->setEnabled( value );
        sizeButton_->setEnabled( value );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::SetLineWidth
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void TerrainPreference::SetLineWidth() const
{
    glLineWidth( lineWidth_ );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::SetColor
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void TerrainPreference::SetColor( float alpha ) const
{
    glColor4f( color_.red() / 255.f, color_.green() / 255.f, color_.blue() / 255.f, shown_ ? alpha : 0.f );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::Commit
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void TerrainPreference::Commit()
{
    previousColor_ = color_;
    previousLineWidth_ = lineWidth_;
    previousShown_ = shown_;
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::Revert
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void TerrainPreference::Revert()
{
    colorButton_->SetColor( previousColor_ );
    sizeButton_->SetSize( previousLineWidth_ );
    showCheckbox_->setChecked( previousShown_ );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::Save
// Created: SBO 2008-08-18
// -----------------------------------------------------------------------------
void TerrainPreference::Save() const
{
    options_.Change( "Terrains/" + type_ + "/width", previousLineWidth_ );
    options_.Change( "Terrains/" + type_ + "/color", previousColor_.name() );
    options_.Change( "Terrains/" + type_ + "/shown", previousShown_ );
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
        previousLineWidth_ = value.To< float >();
        sizeButton_->SetSize( previousLineWidth_ );
    }
    else if( option == "/color" )
    {
        previousColor_ = QColor( value.To< QString >() );
        colorButton_->SetColor( previousColor_ );
    }
    else if( option == "/shown" )
    {
        previousShown_ = value.To< bool >();
        showCheckbox_->setChecked( previousShown_ );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::OnStateChanged
// Created: LGY 2014-06-20
// -----------------------------------------------------------------------------
void TerrainPreference::OnStateChanged( int state )
{
    shown_ = state == Qt::Checked;
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::OnSizeChanged
// Created: LGY 2014-06-20
// -----------------------------------------------------------------------------
void TerrainPreference::OnSizeChanged( int )
{
    lineWidth_ = sizeButton_->GetSize();
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::OnColorChanged
// Created: LGY 2014-06-20
// -----------------------------------------------------------------------------
void TerrainPreference::OnColorChanged( const QColor& color )
{
    color_ = color;
}
