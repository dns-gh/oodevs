// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FilterArgument.h"
#include "moc_FilterArgument.cpp"
#include "FilterInputArgument.h"
#include "tools/ExerciseConfig.h"
#include "clients_kernel/XmlDescription.h"
#include "clients_kernel/Tools.h"

namespace
{
    std::string ReadLang()
    {
        QSettings settings( "MASA Group", tools::translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() ).toAscii().constData();
    }
}

// -----------------------------------------------------------------------------
// Name: FilterArgument constructor
// Created: MCO 2013-06-03
// -----------------------------------------------------------------------------
FilterArgument::FilterArgument( xml::xistream& xis, const tools::ExerciseConfig& config, const std::string& value )
{
    xis >> xml::attribute( "name", name_ )
        >> xml::optional >> xml::attribute( "display-name", displayName_ )
        >> xml::optional >> xml::attribute( "value", value_ );
    if( FilterInputArgument::IsInputArgument( value_ ) )
    {
        kernel::XmlDescription description( xis, ReadLang() );
        input_.reset( new FilterInputArgument( config, value_, description, config.GetExerciseDir( config.GetExerciseName() ) ) );
        connect( input_.get(), SIGNAL( ValueChanged( const QString& ) ), SLOT( OnValueChanged( const QString& ) ) );
        value_ = "";
    }
    else
        value_ = value;
}

// -----------------------------------------------------------------------------
// Name: FilterArgument destructor
// Created: MCO 2013-06-03
// -----------------------------------------------------------------------------
FilterArgument::~FilterArgument()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterArgument::IsValid
// Created: MCO 2013-06-03
// -----------------------------------------------------------------------------
bool FilterArgument::IsValid() const
{
    if( ! input_.get() )
        return true;
    return input_->IsValid();
}

// -----------------------------------------------------------------------------
// Name: FilterArgument::Update
// Created: MCO 2013-06-03
// -----------------------------------------------------------------------------
void FilterArgument::Update()
{
    if( input_.get() )
        value_.clear();
}

// -----------------------------------------------------------------------------
// Name: FilterArgument::AddWidget
// Created: MCO 2013-06-03
// -----------------------------------------------------------------------------
void FilterArgument::AddWidget( QWidget* widget, QGridLayout* grid, int row )
{
    if( ! input_.get() )
        return;
    if( displayName_.empty() )
        grid->addWidget( new QLabel( tools::translate( "FilterCommand", "Argument '%1':" ).arg( name_.c_str() ), widget, "FilterCommand_CommandTitle" ), row, 0 );
    else
        grid->addWidget( new QLabel( QString( "%1:" ).arg( displayName_.c_str() ), widget, "FilterCommand_CommandTitle" ), row, 0 );
    grid->addWidget( input_->CreateWidget( widget ), row, 1 );
}

// -----------------------------------------------------------------------------
// Name: FilterArgument::IsInputArgument
// Created: MCO 2013-06-03
// -----------------------------------------------------------------------------
bool FilterArgument::IsInputArgument() const
{
    return input_.get() != 0;
}

// -----------------------------------------------------------------------------
// Name: FilterArgument::ToCommandLine
// Created: MCO 2013-06-03
// -----------------------------------------------------------------------------
std::string FilterArgument::ToCommandLine() const
{
    if( ! IsInputArgument() )
        return " " + name_ + (value_.empty() ? "" : "=" + value_);
    if( ! value_.empty() )
        return " " + name_ + "=\"" + value_ + "\"";
    return "";
}

// -----------------------------------------------------------------------------
// Name: FilterArgument::OnValueChanged
// Created: MCO 2013-06-03
// -----------------------------------------------------------------------------
void FilterArgument::OnValueChanged( const QString& text )
{
    value_ = text.toAscii().constData();
    emit ValueChanged( text );
}
