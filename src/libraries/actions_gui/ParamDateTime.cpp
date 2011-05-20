// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamDateTime.h"
#include "moc_ParamDateTime.cpp"
#include "actions/DateTime.h"
#include "clients_kernel/GlTools_ABC.h"
#include <qdatetimeedit.h>

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamDateTime constructor
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
ParamDateTime::ParamDateTime( QObject* parent, const kernel::OrderParameter& parameter, const QDateTime& current )
    : QObject( parent )
    , Param_ABC( parameter.GetName().c_str() )
    , parameter_( parameter )
    , date_( current )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamDateTime constructor
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
ParamDateTime::ParamDateTime( QObject* parent, const QString& name, const QDateTime& current, bool optional )
    : QObject( parent )
    , Param_ABC( name )
    , parameter_( name.ascii(), "datetime", optional )
    , date_( current )
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
QWidget* ParamDateTime::BuildInterface( QWidget* parent )
{
    QHBox* box = new QHBox( parent );
    box->setSpacing( 5 );
    new QLabel( GetName(), box );
    QDateTimeEdit* edit = new QDateTimeEdit( box );
    edit->setDateTime( date_ );
    box->setStretchFactor( edit, 1 );
    connect( edit, SIGNAL( valueChanged( const QDateTime& ) ), SLOT( OnChanged( const QDateTime& ) ) );
    return box;
}

// -----------------------------------------------------------------------------
// Name: ParamDateTime::CheckValidity
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
bool ParamDateTime::CheckValidity()
{
    return IsOptional() || !date_.isNull(); // $$$$ SBO 2007-05-14: check with current date (allow date in the past?)
}

// -----------------------------------------------------------------------------
// Name: ParamDateTime::CommitTo
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ParamDateTime::CommitTo( actions::ParameterContainer_ABC& parameter ) const
{
    parameter.AddParameter( *new actions::parameters::DateTime( parameter_, date_ ) );
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
    if( date_.isValid() )
        tools.Print( date_.toString( "dd-MM-yy hh:mm:ss" ).ascii(), point
                   , QFont( "Arial", 12, QFont::Bold ) ); // $$$$ SBO 2007-05-15: gather fonts somewhere
}

// -----------------------------------------------------------------------------
// Name: ParamDateTime::IsOptional
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
bool ParamDateTime::IsOptional() const
{
    return parameter_.IsOptional();
}
