// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ScoreDefinition.h"
#include "indicators/ElementFactory.h"
#include "indicators/FormulaParser.h"
#include "indicators/Gauge.h"
#include "indicators/GaugeFactory.h"
#include "indicators/Serializer.h"
#include "indicators/Variables.h"
#include <xeumeuleu/xml.h>

namespace
{
    std::string ReadFormula( xml::xistream& xis )
    {
        std::string formula;
        xis >> xml::start( "formula" ) >> formula >> xml::end();
        return formula;
    }

    indicators::Gauge* ReadGauge( xml::xistream& xis, const indicators::GaugeFactory& factory )
    {
        xis >> xml::start( "gauge" );
        indicators::Gauge* gauge = factory.Create( xis );
        xis >> xml::end();
        return gauge;
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreDefinition constructor
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
ScoreDefinition::ScoreDefinition( xml::xistream& xis, const indicators::Primitives& primitives, const indicators::GaugeFactory& factory )
    : name_( xml::attribute< std::string >( xis, "name" ).c_str() )
    , formula_( ReadFormula( xis ) )
    , gauge_( ReadGauge( xis, factory ) )
    , variables_( new indicators::Variables( xis ) )
    , elementFactory_( new indicators::ElementFactory( primitives, *variables_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoreDefinition destructor
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
ScoreDefinition::~ScoreDefinition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoreDefinition::GetName
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
QString ScoreDefinition::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: ScoreDefinition::Commit
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
std::string ScoreDefinition::Commit( const T_Parameters& /*parameters*/ ) const
{
    indicators::Serializer serializer( *elementFactory_, *variables_ );
    indicators::FormulaParser parser( serializer );
    parser.Parse( QString( "plot( %1 )" ).arg( formula_.c_str() ).ascii() );
    xml::xostringstream xos;
    serializer.Serialize( xos );
    return xos.str();
}

// -----------------------------------------------------------------------------
// Name: ScoreDefinition::CreateGauge
// Created: SBO 2009-05-06
// -----------------------------------------------------------------------------
indicators::Gauge* ScoreDefinition::CreateGauge() const
{
    return new indicators::Gauge( *gauge_ );
}
