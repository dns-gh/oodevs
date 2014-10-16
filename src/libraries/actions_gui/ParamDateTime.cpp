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
#include "clients_gui/GLView_ABC.h"
#include <QtGui/qdatetimeedit.h>

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamDateTime constructor
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
ParamDateTime::ParamDateTime( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder, parameter )
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
QWidget* ParamDateTime::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    dateTimeEdit_ = new QDateTimeEdit( parent );
    dateTimeEdit_->setDateTime( date_ );
    connect( dateTimeEdit_, SIGNAL( dateTimeChanged( const QDateTime& ) ), SLOT( OnChanged( const QDateTime& ) ) );
    connect( dateTimeEdit_, SIGNAL( dateTimeChanged( const QDateTime& ) ), SLOT( Update() ) );
    layout->addWidget( dateTimeEdit_ );
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
void ParamDateTime::Draw( const geometry::Point2f& point, const ::gui::Viewport_ABC&, gui::GLView_ABC& tools ) const
{
    if( date_.isValid() )
        tools.Print( date_.toString( "dd-MM-yy hh:mm:ss" ).toStdString(), point, QFont( "Arial", 12, QFont::Bold ) ); // $$$$ SBO 2007-05-15: gather fonts somewhere
}

// -----------------------------------------------------------------------------
// Name: ParamDateTime::Visit
// Created: ABR 2013-06-12
// -----------------------------------------------------------------------------
void ParamDateTime::Visit( const actions::parameters::DateTime& param )
{
    ActivateOptionalIfNeeded( param );
    if( param.IsSet() )
        dateTimeEdit_->setDateTime( QDateTime::fromString( param.GetTime().c_str(), "yyyyMMddTHHmmss" ) );
}
