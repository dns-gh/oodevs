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
#include "preparation/IndicatorElementFactory.h"
#include "preparation/IndicatorParser.h"
#include "preparation/IndicatorSerializer.h"
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
Score::Score( xml::xistream& xis, kernel::Controller& controller, const IndicatorPrimitives& indicators )
    : controller_( &controller )
    , name_( xml::attribute< std::string >( xis, "name" ).c_str() )
    , formula_( ReadFormula( xis ) )
    , variables_( new IndicatorVariables( xis ) )
    , elementFactory_( new IndicatorElementFactory( indicators, *variables_ ) )
{
    controller_->Create( *(Score_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Score constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
Score::Score( const QString& name, kernel::Controller& controller, const IndicatorPrimitives& indicators )
    : controller_( &controller )
    , name_( name )
    , formula_()
    , variables_( new IndicatorVariables() )
    , elementFactory_( new IndicatorElementFactory( indicators, *variables_ ) )
{
    controller_->Create( *(Score_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Score constructor
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
Score::Score( const QString& name, const QString& formula, const IndicatorVariables& variables, const IndicatorPrimitives& indicators )
    : controller_( 0 )
    , name_( name )
    , formula_( formula )
    , variables_( &variables.Clone() )
    , elementFactory_( new IndicatorElementFactory( indicators, *variables_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Score destructor
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
Score::~Score()
{
    if( controller_ )
        controller_->Delete( *(Score_ABC*)this );
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
    xos << xml::attribute( "name", name_.ascii() )
        << xml::start( "formula" ) << formula_.ascii() << xml::end();
    SerializeVariables( xos );
    SerializeIndicators( xos );
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
// Name: Score::SerializeIndicators
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
void Score::SerializeIndicators( xml::xostream& xos ) const
{
    IndicatorSerializer serializer( *elementFactory_, *variables_ );
    IndicatorParser parser( serializer );
    parser.Parse( formula_.ascii() );
    serializer.Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: Score::Accept
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
void Score::Accept( IndicatorVariablesVisitor_ABC& visitor ) const
{
    variables_->Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: Score::operator=
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
Score& Score::operator=( const Score& score )
{
    name_ = score.name_;
    formula_ = score.formula_;
    *variables_ = *score.variables_;
    return *this;
}
