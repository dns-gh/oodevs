// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "AdvancedConfigPanel.h"
#include "moc_AdvancedConfigPanel.cpp"

#include "clients_kernel/Tools.h"

#include "frontend/CreateSession.h"

#include <QtGui/qcheckbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qspinbox.h>
#include <QtCore/qsettings.h>

using namespace frontend;

namespace
{
    void AddControl( QLayout* layout, QWidget* label, QWidget* field )
    {
        QWidget* container = new QWidget();
        QHBoxLayout* inner = new QHBoxLayout( container );
        inner->addWidget( label );
        inner->addWidget( field );
        layout->addWidget( container );
    }

    void AddSpinBox( QVBoxLayout* boxLayout, QLabel*& label, QSpinBox*& spin, int rangeMin, int rangeMax, int step, int spinValue )
    {
        label = new QLabel();
        spin = new QSpinBox();
        spin->setRange(rangeMin, rangeMax );
        spin->setSingleStep( step );
        spin->setValue( spinValue );
        AddControl( boxLayout, label, spin );
    }

    void AddCheckBox( QVBoxLayout* layout, QLabel*& label, QCheckBox*& checkbox, bool checked )
    {
        label = new QLabel();
        checkbox = new QCheckBox();
        checkbox->setCheckState( checked ? Qt::Checked : Qt::Unchecked );
        AddControl( layout, label, checkbox );
    }
}

// -----------------------------------------------------------------------------
// Name: AdvancedConfigPanel constructor
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
AdvancedConfigPanel::AdvancedConfigPanel( QWidget* parent, const tools::GeneralConfig& config,
                                          bool hasClient, bool autostartEvents )
    : PluginConfig_ABC( parent )
    , config_( config )
{
    //time box

    //---
    stepLabel_ = new QLabel();
    stepSpin_ = new QSpinBox();
    stepSpin_->setRange( 1, 100 );
    stepSpin_->setSingleStep( 1 );
    stepSpin_->setValue( 10 );

    //---
    factorLabel_ = new QLabel();
    factorSpin_ = new QSpinBox();
    factorSpin_->setRange( 1, 100 );
    factorSpin_->setSingleStep( 1 );
    factorSpin_->setValue( 10 );

    //---
    endtickLabel_ = new QLabel();
    endtickSpin_ = new QSpinBox();
    endtickSpin_->setRange( 0, std::numeric_limits< int >::max() );
    endtickSpin_->setSingleStep( 1 );
    endtickSpin_->setValue( 0 );

    //---
    pausedLabel_ = new QLabel();
    pausedCheckBox_ = new QCheckBox();
    pausedCheckBox_->setChecked( false );

    //---
    timeBox_ = new QGroupBox();
    QGridLayout* timeBoxLayout = new QGridLayout( timeBox_ );
    timeBoxLayout->addWidget( stepLabel_, 0, 0, 1, 1 );
    timeBoxLayout->addWidget( stepSpin_, 0, 1, 1, 1 );
    timeBoxLayout->addWidget( factorLabel_, 0, 2, 1, 1 );
    timeBoxLayout->addWidget( factorSpin_, 0, 3, 1, 1 );
    timeBoxLayout->addWidget( endtickLabel_, 1, 0, 1, 1 );
    timeBoxLayout->addWidget( endtickSpin_, 1, 1, 1, 1 );
    timeBoxLayout->addWidget( pausedLabel_, 1, 2, 1, 1 );
    timeBoxLayout->addWidget( pausedCheckBox_, 1, 3, 1, 1 );

    QGroupBox* miscellaneousBox = new QGroupBox();
    QVBoxLayout* miscellaneousBoxLayout = new QVBoxLayout( miscellaneousBox );
    miscellaneousBoxLayout->setMargin( 5 );
    AddSpinBox( miscellaneousBoxLayout, pathThreadsLabel_, pathThreadsSpin_, 0, 4, 1, 1 );
    AddSpinBox( miscellaneousBoxLayout, fragmentsFrequencyLabel_, fragmentsFrequencySpin_, 0, std::numeric_limits< int >::max(), 1, 200 );

    //client box
    AddCheckBox( miscellaneousBoxLayout, noClientLabel_, noClientCheckBox_, !hasClient );
    connect( noClientCheckBox_, SIGNAL( stateChanged ( int ) ), SLOT( OnClientChecked( int ) ) );

    AddSpinBox( miscellaneousBoxLayout, reportsFrequencyLabel_, reportsFrequencySpin_, 1, std::numeric_limits< int >::max(), 1, 100 );

    AddCheckBox( miscellaneousBoxLayout, autostartEventsLabel_, autostartEvents_, autostartEvents );
    connect( autostartEvents_, SIGNAL( clicked( bool ) ), this, SIGNAL( OnAutostartEvents( bool ) ) );

    //general panel
    QVBoxLayout* boxLayout = new QVBoxLayout( this );
    boxLayout->setMargin( 5 );
    boxLayout->addWidget( timeBox_ );
    boxLayout->addWidget( miscellaneousBox );
}

// -----------------------------------------------------------------------------
// Name: AdvancedConfigPanel destructor
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
AdvancedConfigPanel::~AdvancedConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AdvancedConfigPanel::OnLanguageChanged
// Created: ABR 2011-11-10
// -----------------------------------------------------------------------------
void AdvancedConfigPanel::OnLanguageChanged()
{
    timeBox_->setTitle( tr( "Time" ) );
    stepLabel_->setText( tr( "Time step" ) );
    factorLabel_->setText( tr( "Time factor" ) );
    endtickLabel_->setText( tr( "End tick" ) );
    pausedLabel_->setText( tr( "Paused at startup" ) );

    pathThreadsLabel_->setText( tr( "Number of pathfind threads" ) );
    fragmentsFrequencyLabel_->setText( tr( "Recorder fragmentation frequency" ) );
    noClientLabel_->setText( tr( "Do not start gaming client" ) );
    reportsFrequencyLabel_->setText( tr( "Report clean frequency" ) );
    autostartEventsLabel_->setText( tr( "Automatically start timeline events" ) );
}

// -----------------------------------------------------------------------------
// Name: AdvancedConfigPanel::GetName
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
QString AdvancedConfigPanel::GetName() const
{
    return tr( "Advanced" );
}

// -----------------------------------------------------------------------------
// Name: AdvancedConfigPanel::Commit
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
void AdvancedConfigPanel::Commit( const tools::Path& exercise, const tools::Path& session )
{
    frontend::CreateSession action( config_, exercise, session );
    action.SetOption( "session/config/simulation/time/@step", stepSpin_->value() );
    action.SetOption( "session/config/simulation/time/@factor", factorSpin_->value() );
    action.SetOption( "session/config/simulation/time/@end-tick", endtickSpin_->value() );
    action.SetOption( "session/config/simulation/time/@paused", pausedCheckBox_->isChecked() );
    action.SetOption( "session/config/simulation/pathfinder/@threads", pathThreadsSpin_->value() );
    action.SetOption( "session/config/dispatcher/plugins/recorder/@fragmentfreq", fragmentsFrequencySpin_->value() );
    action.SetOption( "session/config/dispatcher/reports/@frequency", reportsFrequencySpin_->value() );
    action.Commit();
}

// -----------------------------------------------------------------------------
// Name: AdvancedConfigPanel::OnClientChecked
// Created: RBA 2012-01-30
// -----------------------------------------------------------------------------
void AdvancedConfigPanel::OnClientChecked( int state )
{
    emit OnClientEnabled( state == Qt::Unchecked );
}
