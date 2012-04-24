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
#include "InterfaceBuilder_ABC.h"
#include "actions/DateTime.h"
#include "clients_kernel/GlTools_ABC.h"
#include <QtGui/qdatetimeedit.h>

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamDateTime constructor
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
ParamDateTime::ParamDateTime( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder.GetParentObject(), builder.GetParamInterface(), parameter )
    , date_( builder.GetCurrentDateTime() )
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
    Param_ABC::BuildInterface( parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    QDateTimeEdit* edit = new QDateTimeEdit( parent );
    edit->setDateTime( date_ );
    connect( edit, SIGNAL( dateTimeChanged( const QDateTime& ) ), SLOT( OnChanged( const QDateTime& ) ) );
    layout->addWidget( edit );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamDateTime::InternalCheckValidity
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
bool ParamDateTime::InternalCheckValidity() const
{
    return !date_.isNull(); // $$$$ SBO 2007-05-14: check with current date (allow date in the past?)
}

// -----------------------------------------------------------------------------
// Name: ParamDateTime::CommitTo
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ParamDateTime::CommitTo( actions::ParameterContainer_ABC& parameter ) const
{
    if( IsChecked() )
        parameter.AddParameter( *new actions::parameters::DateTime( parameter_, date_ ) );
    else
        parameter.AddParameter( *new actions::parameters::DateTime( parameter_ ) );
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
        tools.Print( date_.toString( "dd-MM-yy hh:mm:ss" ).ascii(), point, QFont( "Arial", 12, QFont::Bold ) ); // $$$$ SBO 2007-05-15: gather fonts somewhere
}
