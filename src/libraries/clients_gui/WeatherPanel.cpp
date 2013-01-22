// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::WeatherPanel */

#include "clients_gui_pch.h"
#include "WeatherPanel.h"
#include "moc_WeatherPanel.cpp"

#include "ValuedComboBox.h"
#include "WeatherLayer.h"
#include "WeatherListView.h"
#include "WeatherWidget.h"
#include "clients_kernel/Controllers.h"
#include "meteo/MeteoLocal.h"
#include "meteo/PHY_Lighting.h"
#include "ENT/ENT_Tr.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: WeatherPanel constructor
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
WeatherPanel::WeatherPanel( QWidget* parent, PanelStack_ABC& panel, WeatherLayer& layer )
    : InfoPanel_ABC( parent, panel, tr( "Weathers" ), "WeatherPanel" )
    , layer_        ( layer )
    , selectedLocal_( 0 )
    , currentType_  ( eWeatherGlobal )
    , localWeathers_( 0 )
    , lighting_     ( 0 )
    , startTime_    ( 0 )
    , endTime_      ( 0 )
{
    // Layouts
    QWidget* box = new QWidget( this );
    QBoxLayout* layout = new QBoxLayout( box, QBoxLayout::TopToBottom, 0, 5 );
    layout->setMargin( 5 );
    layout->setAlignment( Qt::AlignTop );

    Q3VBox* mainLayout = new Q3VBox( this );
    mainLayout->layout()->setAlignment( Qt::AlignTop );

    headerLayout_ = new Q3VBox( box );
    layout->addWidget( headerLayout_ );
    Q3VBox* contentLayout = new Q3VBox( box );
    layout->addWidget( contentLayout );
    Q3HBox* buttonsLayout = new Q3HBox( box );
    buttonsLayout->layout()->setSpacing( 5 );

    layout->addWidget( buttonsLayout );
    buttonsLayout->setMinimumHeight( 30 );
    buttonsLayout->setMaximumHeight( 30 );
    // Weather
    Q3GroupBox* group = new Q3GroupBox( 1, Qt::Horizontal, tr( "Weather" ), contentLayout );
    {
        Q3HBox* hbox = new Q3HBox( group );
        new QLabel( tr( "Weather type:" ), hbox );
        gui::ValuedComboBox< E_WeatherType >* weatherTypeCombo = new gui::ValuedComboBox< E_WeatherType >( hbox );
        weatherTypeCombo->AddItem( tr( "Global weather" ), eWeatherGlobal );
        weatherTypeCombo->AddItem( tr( "Local weather" ), eWeatherLocal );
        connect( weatherTypeCombo, SIGNAL( activated( int ) ), this, SLOT( OnTypeChanged( int ) ) );
    }
    // Global weather layout
    globalLayout_ = new Q3VBox( group );
    globalLayout_->layout()->setSpacing( 5 );
    // Local weather layout
    localLayout_ = new Q3VBox( group );
    localLayout_->layout()->setSpacing( 5 );
    // Buttons
    setWidget( box );
    QPushButton* okBtn     = new QPushButton( tr( "Validate" ) , buttonsLayout );
    QPushButton* cancelBtn = new QPushButton( tr( "Cancel" ), buttonsLayout );
    connect( okBtn,     SIGNAL( clicked() ), this, SLOT( Commit() ) );
    connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( Reset() ) );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel destructor
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
WeatherPanel::~WeatherPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::CreateLocalParameters
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
void WeatherPanel::CreateLocalParameters()
{
    // Lighting added to common parameters
    new QLabel( tr( "Lighting:" ), localWidget_ );
    lighting_ = new gui::ValuedComboBox< E_LightingType >( localWidget_ );
    for( int i = 0; i < static_cast< int >( eLightingType_Eclairant ); ++i )
        lighting_->AddItem( ENT_Tr::ConvertFromLightingType( static_cast< E_LightingType >( i ), ENT_Tr::eToTr ), static_cast< E_LightingType >( i ) );
    lighting_->AddItem( ENT_Tr::ConvertFromLightingType( eNbrLightingType, ENT_Tr::eToTr ), eNbrLightingType );

    // Time
    parametersGroup_ = new Q3GroupBox( 2, Qt::Horizontal, tr( "Time and position parameters" ), localLayout_ );
    localLayout_->setMinimumHeight( 350 );
    new QLabel( tr( "Start time:" ), parametersGroup_ );
    startTime_ = new QDateTimeEdit( parametersGroup_ );
    new QLabel( tr( "End time:" ), parametersGroup_ );
    endTime_ = new QDateTimeEdit( parametersGroup_ );

    locationButton_ = new QPushButton( tr( "Set location" ), localLayout_ );
    connect( locationButton_, SIGNAL( clicked() ), this, SLOT( SetPatchPosition() ) );

    OnTypeChanged( 0 );
    EnableLocalParameters( false, false );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::EnableLocalParameters
// Created: ABR 2012-03-09
// -----------------------------------------------------------------------------
void WeatherPanel::EnableLocalParameters( bool enabled, bool isCreated )
{
    localWidget_->setEnabled( enabled );
    parametersGroup_->setEnabled( enabled && isCreated );
    locationButton_->setEnabled( enabled && isCreated );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::OnTypeChanged
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
void WeatherPanel::OnTypeChanged( int selected )
{
    currentType_ = static_cast< E_WeatherType >( selected );
    assert( currentType_ == eWeatherLocal || currentType_ == eWeatherGlobal );
    Reset();
    if( currentType_ == eWeatherLocal )
    {
        localLayout_->show();
        localWeathers_->clearSelection();
        localWidget_->Clear();
        globalLayout_->hide();
    }
    else
    {
        localLayout_->hide();
        globalLayout_->show();
    }
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::CommitLocalWeather
// Created: SBO 2010-05-17
// -----------------------------------------------------------------------------
void WeatherPanel::CommitLocalWeather()
{
    if( selectedLocal_ )
    {
        localWidget_->CommitTo( *selectedLocal_ );
        if( selectedLocal_->IsCreated() )
            selectedLocal_->SetPeriod( startTime_->dateTime(), endTime_->dateTime() );
        const weather::PHY_Lighting* lighting = weather::PHY_Lighting::FindLighting( ENT_Tr::ConvertFromLightingType( lighting_->GetValue() ) );
        selectedLocal_->SetLighting( lighting );
    }
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::LocalSelectionChanged
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherPanel::LocalSelectionChanged()
{
    weather::MeteoLocal* selected = static_cast< weather::MeteoLocal* >( localWeathers_->SelectedItem() );
    CommitLocalWeather();
    if( selected != 0 )
    {
        localWidget_->Update( *selected );
        startTime_->setDateTime( selected->GetStartTime() );
        endTime_->setDateTime( selected->GetEndTime() );
        const weather::PHY_Lighting* lighting = selected->GetLocalLighting();
        const E_LightingType& lightingType = lighting ? ENT_Tr::ConvertToLightingType( lighting->GetName() ) : eNbrLightingType;
        lighting_->SetCurrentItem( lightingType );
        layer_.SetPosition( *selected );
    }
    else
        layer_.Clear();
    EnableLocalParameters( selected != 0, selected != 0 && selected->IsCreated() );
    selectedLocal_ = selected;
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::SetPatchPosition
// Created: SBO 2006-12-21
// -----------------------------------------------------------------------------
void WeatherPanel::SetPatchPosition()
{
    if( localWeathers_ && localWeathers_->SelectedItem() )
        layer_.StartEdition( *static_cast< weather::MeteoLocal* >( localWeathers_->SelectedItem() ) );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::hide
// Created: ABR 2011-06-10
// -----------------------------------------------------------------------------
void WeatherPanel::hide()
{
    QWidget::hide();
    layer_.Clear();
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::Purge
// Created: ABR 2012-05-31
// -----------------------------------------------------------------------------
void WeatherPanel::Purge()
{
    layer_.Clear();
    EnableLocalParameters( false, false );
}
