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
    , date_( simulation.GetDateTime() )
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
    QDateTimeEdit* edit = new QDateTimeEdit( parent );
    edit->setDateTime( date_ );
    connect( edit, SIGNAL( valueChanged( const QDateTime& ) ), SLOT( OnChanged( const QDateTime& ) ) );
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
    parameter.AddParameter( *new ActionParameterDateTime( OrderParameter( GetName(), "datetime", false ), date_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamDateTime::OnChanged
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
void ParamDateTime::OnChanged( const QDateTime& date )
{
    date_ = date;
}

// -----------------------------------------------------------------------------
// Name: ParamDateTime::Draw
// Created: SBO 2007-05-15
// -----------------------------------------------------------------------------
void ParamDateTime::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC&, const kernel::GlTools_ABC& tools ) const
{
    if( !date_.isNull() )
        tools.Print( date_.toString( "dd-MM-yy hh:mm:ss" ).ascii(), point
                   , QFont( "Arial", 12, QFont::Bold ) ); // $$$$ SBO 2007-05-15: gather fonts somewhere
}
