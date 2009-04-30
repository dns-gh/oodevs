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
#include "indicators/Variables.h"
#include "indicators/ElementFactory.h"
#include "indicators/FormulaParser.h"
#include "indicators/Serializer.h"
#include <xeumeuleu/xml.h>

namespace
{
    std::string ReadFormula( xml::xistream& xis )
    {
        std::string formula;
        xis >> xml::start( "formula" ) >> formula >> xml::end();
        return formula;
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreDefinition constructor
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
ScoreDefinition::ScoreDefinition( xml::xistream& xis, const indicators::Primitives& primitives )
    : name_( xml::attribute< std::string >( xis, "name" ).c_str() )
    , formula_( ReadFormula( xis ) )
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
