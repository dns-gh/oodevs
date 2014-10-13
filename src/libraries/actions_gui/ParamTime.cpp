// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamTime.h"
#include "moc_ParamTime.cpp"
#include "InterfaceBuilder_ABC.h"
#include "actions/Quantity.h"
#include "clients_gui/GLView_ABC.h"
#include <QtGui/qdatetimeedit.h>

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamTime constructor
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
ParamTime::ParamTime( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder, parameter )
    , time_( 0, 0, 0, 0 ) // $$$ LDC Must be a valid time otherwise user can't understand why he can't click ok with a 0 value time.
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamTime destructor
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
ParamTime::~ParamTime()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamTime::BuildInterface
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
QWidget* ParamTime::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    timeEdit_ = new QTimeEdit( parent );
    timeEdit_->setTime( time_ );
    timeEdit_->setDisplayFormat( "hh:mm:ss" );
    connect( timeEdit_, SIGNAL( timeChanged( const QTime& ) ), SLOT( OnChanged( const QTime& ) ) );
    connect( timeEdit_, SIGNAL( timeChanged( const QTime& ) ), SLOT( Update() ) );
    layout->addWidget( timeEdit_ );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamTime::InternalCheckValidity
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
bool ParamTime::InternalCheckValidity() const
{
    return !time_.isNull();
}

// -----------------------------------------------------------------------------
// Name: ParamTime::CommitTo
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
void ParamTime::CommitTo( actions::ParameterContainer_ABC& parameter ) const
{
    int seconds = 3600 * time_.hour() + 60 * time_.minute() + time_.second();
    if( IsChecked() )
        parameter.AddParameter( *new actions::parameters::Quantity( parameter_, seconds ) );
    else
        parameter.AddParameter( *new actions::parameters::Quantity( parameter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamTime::OnChanged
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
void ParamTime::OnChanged( const QTime& time )
{
    time_ = time;
}

// -----------------------------------------------------------------------------
// Name: ParamTime::Visit
// Created: ABR 2013-06-12
// -----------------------------------------------------------------------------
void ParamTime::Visit( const actions::parameters::Quantity& param )
{
    ActivateOptionalIfNeeded( param );
    if( param.IsSet() )
    {
        int seconds = param.GetValue();
        time_ = QTime( 0, 0, 0, 0 );
        time_.addSecs( seconds );
        timeEdit_->setTime( time_ );
    }
}
