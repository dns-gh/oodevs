// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "WeatherPanel.h"
#include "moc_WeatherPanel.cpp"
#include "RichWidget.h"
#include "RichGroupBox.h"
#include "RichPushButton.h"
#include "SubObjectName.h"
#include "ValuedComboBox.h"
#include "WeatherLayer.h"
#include "WeatherListView.h"
#include "WeatherWidget.h"
#include "SubObjectName.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "meteo/MeteoLocal.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: WeatherPanel constructor
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
WeatherPanel::WeatherPanel( QWidget* parent,
                            PanelStack_ABC& panel,
                            const std::shared_ptr< WeatherLayer >& layer )
    : InfoPanel_ABC( parent, panel, tr( "Weathers" ), "WeatherPanel" )
    , layer_        ( layer )
    , selectedLocal_( 0 )
    , localWeathers_( 0 )
    , currentType_  ( eWeatherGlobal )
    , startTime_    ( 0 )
    , endTime_      ( 0 )
{
    SubObjectName subObject1( "WeatherPanel" );

    //----------------headerlayout------------------
    headerLayout_ = new QVBoxLayout();

    //----------------content Layout----------------
    SubObjectName subObject2( "WeatherGroup" );
    //Weather type
    QLabel* weatherTypeLabel = new QLabel( tr( "Weather type:" ) );
    gui::ValuedComboBox< E_WeatherType >* weatherTypeCombo = new gui::ValuedComboBox< E_WeatherType >( "weatherTypeCombo" );
    weatherTypeCombo->AddItem( tr( "Global weather" ), eWeatherGlobal );
    weatherTypeCombo->AddItem( tr( "Local weather" ), eWeatherLocal );
    connect( weatherTypeCombo, SIGNAL( activated( int ) ), this, SLOT( OnTypeChanged( int ) ) );

    QHBoxLayout* weatherTypeLayout = new QHBoxLayout();
    weatherTypeLayout->addWidget( weatherTypeLabel );
    weatherTypeLayout->addWidget( weatherTypeCombo );

    // Global weather layout
    globalWidget_ = new QWidget();
    QVBoxLayout* globalLayout = new QVBoxLayout( globalWidget_ );
    globalLayout->setSpacing( 5 );
    globalLayout->addStretch( 1 );

    // Local weather layout
    localWidget_ = new QWidget();
    QVBoxLayout* localLayout = new QVBoxLayout( localWidget_ );
    localLayout->setSpacing( 5 );
    localLayout->addStretch( 1 );
    localLayout->setAlignment( Qt::AlignTop );

    RichGroupBox* weatherGroup = new RichGroupBox( "weatherGroup", tr( "Weather" ) );
    QVBoxLayout* groupLayout = new QVBoxLayout( weatherGroup );
    groupLayout->addLayout( weatherTypeLayout );
    groupLayout->addWidget( globalWidget_ );
    groupLayout->addWidget( localWidget_ );
    groupLayout->addStretch( 1 );

    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->addWidget( weatherGroup );

    //----------------Buttons layout----------------
    RichPushButton* okBtn     = new RichPushButton( "ok", tr( "Validate" )  );
    RichPushButton* cancelBtn = new RichPushButton( "cancel", tr( "Cancel" ) );
    connect( okBtn,     SIGNAL( clicked() ), this, SLOT( Commit() ) );
    connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( Reset() ) );

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget( okBtn );
    buttonsLayout->addWidget( cancelBtn );
    buttonsLayout->setSpacing( 5 );

    //----------------main panel----------------
    QWidget* mainWidget = new QWidget( this );
    QVBoxLayout* mainLayout = new QVBoxLayout( mainWidget );
    mainLayout->setMargin( 5 );
    mainLayout->setAlignment( Qt::AlignTop );
    mainLayout->addLayout( headerLayout_ );
    mainLayout->addLayout( contentLayout );
    mainLayout->addLayout( buttonsLayout );
    mainLayout->setSizeConstraint( QLayout::Minimum );
    mainLayout->addStretch( 1 );
    setWidget( mainWidget );
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
    localWidget_->setMinimumHeight( 400 );
    QLabel* startTimeLabel = new QLabel( tr( "Start time:" ) );
    startTime_ = new RichWidget< QDateTimeEdit >( "startTime" );
    QLabel* endTimeLabel = new QLabel( tr( "End time:" ) );
    endTime_ = new RichWidget< QDateTimeEdit >( "endTime" );

    parametersGroup_ = new gui::RichGroupBox( "parametersGroup", tr( "Time and position parameters" ), localWidget_ );
    QGridLayout* parametersGrouplayout = new QGridLayout( parametersGroup_ );
    parametersGrouplayout->addWidget( startTimeLabel, 0, 0 );
    parametersGrouplayout->addWidget( startTime_, 0, 1 );
    parametersGrouplayout->addWidget( endTimeLabel, 1, 0 );
    parametersGrouplayout->addWidget( endTime_, 1, 1 );
    localWidget_->layout()->addWidget( parametersGroup_ );

    locationButton_ = new RichPushButton( "location", tr( "Set location" ), localWidget_ );
    connect( locationButton_, SIGNAL( clicked() ), this, SLOT( SetPatchPosition() ) );
    localWidget_->layout()->addWidget( locationButton_ );

    OnTypeChanged( 0 );
    EnableLocalParameters( false, false );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::EnableLocalParameters
// Created: ABR 2012-03-09
// -----------------------------------------------------------------------------
void WeatherPanel::EnableLocalParameters( bool enabled, bool isCreated )
{
    localWeatherWidget_->setEnabled( enabled );
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
        localWidget_->show();
        localWeathers_->clearSelection();
        localWeatherWidget_->Clear();
        globalWidget_->hide();
    }
    else
    {
        localWidget_->hide();
        globalWidget_->show();
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
        localWeatherWidget_->CommitTo( *selectedLocal_ );
        if( selectedLocal_->IsCreated() )
            selectedLocal_->SetPeriod( tools::QTimeToBoostTime( startTime_->dateTime() ), tools::QTimeToBoostTime( endTime_->dateTime() ) );
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
        localWeatherWidget_->Update( *selected );
        startTime_->setDateTime( tools::BoostTimeToQTime( selected->GetStartTime() ) );
        endTime_->setDateTime( tools::BoostTimeToQTime( selected->GetEndTime() ) );
        layer_->SetPosition( *selected );
    }
    else
        layer_->Clear();
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
        layer_->StartEdition( *static_cast< weather::MeteoLocal* >( localWeathers_->SelectedItem() ) );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::hide
// Created: ABR 2011-06-10
// -----------------------------------------------------------------------------
void WeatherPanel::hide()
{
    QWidget::hide();
    layer_->Clear();
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::Purge
// Created: ABR 2012-05-31
// -----------------------------------------------------------------------------
void WeatherPanel::Purge()
{
    layer_->Clear();
    EnableLocalParameters( false, false );
}
