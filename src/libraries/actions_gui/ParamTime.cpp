// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamTime.h"
#include "moc_ParamTime.cpp"
#include "InterfaceBuilder_ABC.h"
#include "actions/Quantity.h"
#include "clients_kernel/GlTools_ABC.h"
#include <QtGui/qdatetimeedit.h>

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamTime constructor
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
ParamTime::ParamTime( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder.GetParentObject(), builder.GetParamInterface(), parameter )
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
QWidget* ParamTime::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    QTimeEdit* edit = new QTimeEdit( parent );
    edit->setTime( time_ );
    edit->setDisplayFormat( "hh:mm:ss" );
    connect( edit, SIGNAL( timeChanged( const QTime& ) ), SLOT( OnChanged( const QTime& ) ) );
    layout->addWidget( edit );
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
