// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamDateTime.h"
#include "moc_ParamDateTime.cpp"
#include "gaming/Simulation.h"
#include "gaming/ActionParameterDateTime.h"
#include "clients_kernel/GlTools_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamDateTime constructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ParamDateTime::ParamDateTime( QObject* parent, const QString& name, const Simulation& simulation, bool optional )
    : QObject( parent )
    , Param_ABC( name )
    , simulation_( simulation )
    , day_( 1 )
    , time_()
    , optional_( optional )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamDateTime destructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ParamDateTime::~ParamDateTime()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamDateTime::BuildInterface
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ParamDateTime::BuildInterface( QWidget* parent )
{
    new QLabel( GetName(), parent );
    QHBox* box = new QHBox( parent );
    QSpinBox* day = new QSpinBox( 1, 10000, 1, box );
    day->setPrefix( tr( "Day " ) );
    day->setValue( day_ );
    QTimeEdit* timeEdit = new QTimeEdit( box );
    timeEdit->setTime( time_ );
    connect( day     , SIGNAL( valueChanged( int ) ), SLOT( OnDayChanged( int ) ) );
    connect( timeEdit, SIGNAL( valueChanged( const QTime& ) ), SLOT( OnTimeChanged( const QTime& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamDateTime::CheckValidity
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
bool ParamDateTime::CheckValidity()
{
    return optional_; // $$$$ SBO 2007-05-14: check with current date (allow date in the past?)
}

// -----------------------------------------------------------------------------
// Name: ParamDateTime::CommitTo
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ParamDateTime::CommitTo( ActionParameterContainer_ABC& parameter ) const
{
    parameter.AddParameter( *new ActionParameterDateTime( OrderParameter( GetName(), "datetime", false ), simulation_, simulation_.ComputeTick( day_, time_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamDateTime::OnDayChanged
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ParamDateTime::OnDayChanged( int value )
{
    day_ = ( unsigned int )value;
}

// -----------------------------------------------------------------------------
// Name: ParamDateTime::OnTimeChanged
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ParamDateTime::OnTimeChanged( const QTime& value )
{
    time_ = value;
}

// -----------------------------------------------------------------------------
// Name: ParamDateTime::Draw
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
void ParamDateTime::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC&, const kernel::GlTools_ABC& tools ) const
{
    if( !time_.isNull() || day_ > 1 )
        tools.Print( tr( "Day %1 %2" ).arg( day_ ).arg( time_.toString( "hh:mm:ss" ) ).ascii(), point
                   , QFont( "Arial", 12, QFont::Bold ) ); // $$$$ SBO 2007-05-15: gather fonts somewhere
}
