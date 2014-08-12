// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "score_plugin_pch.h"
#include "IndicatorBuilder.h"
#include "indicators/DataTypeFactory.h"
#include "indicators/ElementFactory.h"
#include "indicators/FormulaParser.h"
#include "indicators/Primitives.h"
#include "indicators/Serializer.h"
#include "indicators/Variable.h"
#include "indicators/Variables.h"
#include "tools/Loader_ABC.h"
#include "tools/SessionConfig.h"
#include <xeumeuleu/xml.hpp>
#include <boost/make_shared.hpp>

static std::ostream& operator<<( std::ostream& os, const QString& s )
{
    return os << s.toStdString();
}

using namespace plugins::score;

// -----------------------------------------------------------------------------
// Name: IndicatorBuilder constructor
// Created: SBO 2009-08-21
// -----------------------------------------------------------------------------
IndicatorBuilder::IndicatorBuilder( const tools::SessionConfig& config )
    : primitives_ ( new indicators::Primitives() )
    , typeFactory_( new indicators::DataTypeFactory() )
{
    primitives_->Load( *config.GetLoader().LoadFile(
        tools::GeneralConfig::BuildResourceChildFile( "IndicatorPrimitives.xml" ) ));
}

// -----------------------------------------------------------------------------
// Name: IndicatorBuilder destructor
// Created: SBO 2009-08-21
// -----------------------------------------------------------------------------
IndicatorBuilder::~IndicatorBuilder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorBuilder::Start
// Created: SBO 2009-08-21
// -----------------------------------------------------------------------------
void IndicatorBuilder::Start()
{
    variables_.reset( new indicators::Variables() );
    factory_.reset( new indicators::ElementFactory( *primitives_, *variables_ ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorBuilder::AddVariable
// Created: SBO 2009-08-21
// -----------------------------------------------------------------------------
void IndicatorBuilder::AddVariable( const std::string& name, const std::string& type, const std::string& value )
{
    if( ! variables_.get() )
        throw MASA_EXCEPTION( "indicator builder not initialized" );
    variables_->Register( name, boost::make_shared< indicators::Variable >( name, typeFactory_->Instanciate( type ), value ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorBuilder::Build
// Created: SBO 2009-08-21
// -----------------------------------------------------------------------------
void IndicatorBuilder::Build( const std::string& name, const std::string& formula, xml::xostream& xos )
{
    if( ! factory_.get() )
        throw MASA_EXCEPTION( "indicator builder not initialized" );
    indicators::Serializer handler( *factory_ );
    indicators::FormulaParser parser( handler );
    const std::string indicatorFormula = "indicator( '" + name + "', " + formula + ")";
    parser.Parse( indicatorFormula );
    handler.Serialize( xos );
}
