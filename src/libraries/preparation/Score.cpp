// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Score.h"
#include "IndicatorVariables.h"
#include "clients_kernel/Controller.h"
#include <xeumeuleu/xml.h>

namespace
{
    QString ReadFormula( xml::xistream& xis )
    {
        std::string formula;
        xis >> xml::start( "formula" ) >> formula >> xml::end();
        return formula.c_str();            
    }
}

// -----------------------------------------------------------------------------
// Name: Score constructor
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
Score::Score( xml::xistream& xis, kernel::Controller& controller )
    : controller_( controller )
    , name_( xml::attribute< std::string >( xis, "name" ).c_str() )
    , formula_( ReadFormula( xis ) )
    , variables_( new IndicatorVariables( xis ) )
{
    controller_.Create( *(Score_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Score constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
Score::Score( const QString& name, kernel::Controller& controller )
    : controller_( controller )
    , name_( name )
    , variables_( new IndicatorVariables() )
{
    controller_.Create( *(Score_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Score destructor
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
Score::~Score()
{
    controller_.Delete( *(Score_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Score::GetName
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
QString Score::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Score::GetFormula
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
QString Score::GetFormula() const
{
    return formula_;
}

// -----------------------------------------------------------------------------
// Name: Score::Serialize
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
void Score::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "name", name_.ascii() );
    SerializeVariables( xos );
}

// -----------------------------------------------------------------------------
// Name: Score::SerializeVariables
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
void Score::SerializeVariables( xml::xostream& xos ) const
{
    xos << xml::start( "variables" );
    variables_->Serialize( xos );
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Score::Accept
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
void Score::Accept( IndicatorVariablesVisitor_ABC& visitor ) const
{
    variables_->Accept( visitor );
}
