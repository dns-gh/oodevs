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
{
    layout()->setAlignment( Qt::AlignTop );
    // Layouts
    QVBox* mainLayout = new QVBox( this );
    headerLayout_ = new QVBox( mainLayout );
    QVBox* contentLayout = new QVBox( mainLayout );
    QHBox* buttonsLayout = new QHBox( mainLayout );
    buttonsLayout->setMinimumHeight( 30 );
    buttonsLayout->setMaximumHeight( 30 );
    // Weather
    QGroupBox* group = new QGroupBox( 1, Qt::Horizontal, tr( "Weather" ), contentLayout );
    {
        QHBox* box = new QHBox( group );
        new QLabel( tr( "Weather type:" ), box );
        gui::ValuedComboBox< E_WeatherType >* weatherTypeCombo = new gui::ValuedComboBox< E_WeatherType >( box );
        weatherTypeCombo->AddItem( tr( "Global weather" ), eWeatherGlobal );
        weatherTypeCombo->AddItem( tr( "Local weather" ), eWeatherLocal );
        connect( weatherTypeCombo, SIGNAL( activated( int ) ), this, SLOT( OnTypeChanged( int ) ) );
    }
    // Global weather layout
    globalLayout_ = new QVBox( group );
    // Local weather layout
    localLayout_ = new QVBox( group );
    // Buttons
    QButton* okBtn     = new QPushButton( tr( "Validate" ) , buttonsLayout );
    QButton* cancelBtn = new QPushButton( tr( "Cancel" ), buttonsLayout );
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
    parametersGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Time and position parameters" ), localLayout_ );
    new QLabel( tr( "Start time:" ), parametersGroup_ );
    startTime_ = new QDateTimeEdit( parametersGroup_ );
    new QLabel( tr( "End time:" ), parametersGroup_ );
    endTime_ = new QDateTimeEdit( parametersGroup_ );

    QButton* btn = new QPushButton( tr( "Set location" ), localLayout_ );
    connect( btn, SIGNAL( clicked() ), this, SLOT( SetPatchPosition() ) );

    OnTypeChanged( 0 );
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
    layer_.Clear();
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
        selectedLocal_->SetPeriod( startTime_->dateTime(), endTime_->dateTime() );
    }
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::LocalSelectionChanged
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherPanel::LocalSelectionChanged()
{
    weather::MeteoLocal* selected = static_cast< weather::MeteoLocal* >( localWeathers_->SelectedItem() );
    if( selected )
    {
        CommitLocalWeather();
        localWidget_->Update( *selected );
        startTime_->setDateTime( selected->GetStartTime() );
        endTime_->setDateTime( selected->GetEndTime() );
        parametersGroup_->setEnabled( selected->IsCreated() );
        layer_.SetPosition( *selected );
    }
    else
        layer_.Clear();
    selectedLocal_ = selected;
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::SetPatchPosition
// Created: SBO 2006-12-21
// -----------------------------------------------------------------------------
void WeatherPanel::SetPatchPosition()
{
    layer_.StartEdition( *static_cast< weather::MeteoLocal* >( localWeathers_->SelectedItem() ) );
}
