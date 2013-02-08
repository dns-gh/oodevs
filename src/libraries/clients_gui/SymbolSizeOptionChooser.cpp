// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SymbolSizeOptionChooser.h"
#include "moc_SymbolSizeOptionChooser.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: SymbolSizeOptionChooser constructor
// Created: MMC 2013-02-01
// -----------------------------------------------------------------------------
SymbolSizeOptionChooser::SymbolSizeOptionChooser( QWidget* parent, QToolBar* toolBar
                                                , QPixmap increaseIcon, QPixmap decreaseIcon
                                                , kernel::Controllers& controllers )
    : QObject( parent )
    , increaseButton_( nullptr )
    , decreaseButton_( nullptr )
    , controllers_( controllers )
    , min_( 1.0f )
    , max_( 6.0f )
    , default_( 3.0f )
    , increment_( 1.0f )
    , name_( "SymbolSize" )
{
    if( toolBar )
    {
        increaseButton_ = new QToolButton( toolBar );
        toolBar->addWidget( increaseButton_ );
        increaseButton_->setTextLabel( tools::translate( "Menu", "Increase symbol size" ) );
        increaseButton_->setIconSet( increaseIcon );
        connect( increaseButton_, SIGNAL( clicked() ), this, SLOT( OnIncreaseSymbolUnit() ) );
        
        decreaseButton_ = new QToolButton( toolBar );
        toolBar->addWidget( decreaseButton_ );
        decreaseButton_->setTextLabel( tools::translate( "Menu", "Decrease symbol size" ) );
        decreaseButton_->setIconSet( decreaseIcon );
        connect( decreaseButton_, SIGNAL( clicked() ), this, SLOT( OnDecreaseSymbolUnit() ) );
    }

    float curValue = controllers.options_.GetOption( name_, default_ ).To< float >();
    if( curValue <= min_ )
        controllers_.options_.Change( name_, min_ );
    else if( curValue >= max_ )
        controllers_.options_.Change( name_, max_ );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SymbolSizeOptionChooser destructor
// Created: MMC 2013-02-01
// -----------------------------------------------------------------------------
SymbolSizeOptionChooser::~SymbolSizeOptionChooser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SymbolSizeOptionChooser::OnIncreaseSymbolUnit
// Created: MMC 2013-02-01
// -----------------------------------------------------------------------------
void SymbolSizeOptionChooser::OnIncreaseSymbolUnit()
{
    float nextValue = increment_ + controllers_.options_.GetOption( name_, default_ ).To< float >();
    if( nextValue <= max_ )
        controllers_.options_.Change( name_, nextValue );
}

// -----------------------------------------------------------------------------
// Name: SymbolSizeOptionChooser::OnDecreaseSymbolUnit
// Created: MMC 2013-02-01
// -----------------------------------------------------------------------------
void SymbolSizeOptionChooser::OnDecreaseSymbolUnit()
{
    float nextValue = -increment_ + controllers_.options_.GetOption( name_, default_ ).To< float >();
    if( nextValue >= min_ )
        controllers_.options_.Change( name_, nextValue );
}

// -----------------------------------------------------------------------------
// Name: SymbolSizeOptionChooser::OptionChanged
// Created: MMC 2013-02-01
// -----------------------------------------------------------------------------
void SymbolSizeOptionChooser::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "SymbolSize" )
        CheckButtonsEnable( value.To< float >() );
}

// -----------------------------------------------------------------------------
// Name: SymbolSizeOptionChooser::CheckButtonsEnable
// Created: MMC 2013-02-01
// -----------------------------------------------------------------------------
void SymbolSizeOptionChooser::CheckButtonsEnable( float value )
{
    const float epsilon = increment_ / 100;
    increaseButton_->setEnabled( value < ( max_ - epsilon ) );
    decreaseButton_->setEnabled( value > ( min_ + epsilon ) );
}
