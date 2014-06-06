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
#include "ProfileSelection.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "indicators/ElementFactory.h"
#include "indicators/FormulaParser.h"
#include "indicators/Gauge.h"
#include "indicators/GaugeFactory_ABC.h"
#include "indicators/Serializer.h"
#include "indicators/Variables.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    QString ReadFormula( xml::xistream& xis )
    {
        std::string formula;
        xis >> xml::start( "formula" ) >> formula >> xml::end;
        return formula.c_str();
    }

    indicators::Gauge* ReadGauge( xml::xistream& xis, const indicators::GaugeFactory_ABC& gaugeFactory )
    {
        xis >> xml::start( "gauge" );
        indicators::Gauge* result = gaugeFactory.Create( xis );
        xis >> xml::end;
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: Score constructor
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
Score::Score( xml::xistream& xis, kernel::Controllers& controllers, const indicators::Primitives& indicators, const indicators::GaugeFactory_ABC& gaugeFactory, const ProfilesModel& profilesModel )
    : controller_( &controllers.controller_ )
    , indicators_( indicators )
    , name_      ( xis.attribute< std::string >( "name" ).c_str() )
    , formula_   ( ReadFormula( xis ) )
    , gauge_     ( ReadGauge( xis, gaugeFactory ) )
    , variables_ ( new indicators::Variables( xis ) )
    , profiles_  ( new ProfileSelection( controllers, xis, profilesModel ) )
{
    controller_->Create( *static_cast< Score_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: Score constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
Score::Score( const QString& name, kernel::Controllers& controllers, const indicators::Primitives& indicators, const indicators::GaugeFactory_ABC& gaugeFactory )
    : controller_( &controllers.controller_ )
    , indicators_( indicators )
    , name_      ( name )
    , formula_   ()
    , gauge_     ( gaugeFactory.Create() )
    , variables_ ( new indicators::Variables() )
    , profiles_  ( new ProfileSelection( controllers ) )
{
    controller_->Create( *static_cast< Score_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: Score constructor
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
Score::Score( const Score& score )
    : controller_( 0 )
    , indicators_( score.indicators_ )
    , name_      ( score.name_ )
    , formula_   ( score.formula_ )
    , gauge_     ( new indicators::Gauge( *score.gauge_ ) )
    , variables_ ( new indicators::Variables( *score.variables_ ) )
    , profiles_  ( new ProfileSelection( *score.profiles_ ) )
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
        controller_->Delete( *static_cast< Score_ABC* >( this ) );
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
// Name: Score::GetGauge
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
const indicators::Gauge& Score::GetGauge() const
{
    if( !gauge_.get() )
        throw MASA_EXCEPTION( "Invalid gauge." );
    return *gauge_;
}

// -----------------------------------------------------------------------------
// Name: Score::GetVariables
// Created: SBO 2009-05-12
// -----------------------------------------------------------------------------
const indicators::Variables& Score::GetVariables() const
{
    if( !variables_.get() )
        throw MASA_EXCEPTION( "Invalid variables." );
    return *variables_;
}

// -----------------------------------------------------------------------------
// Name: Score::GetProfiles
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
const ProfileSelection& Score::GetProfiles() const
{
    if( !profiles_.get() )
        throw MASA_EXCEPTION( "Invalid profiles." );
    return *profiles_;
}

// -----------------------------------------------------------------------------
// Name: Score::Serialize
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
void Score::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "score")
            << xml::attribute( "name", name_.toStdString() )
            << xml::start( "formula" ) << xml::cdata( !formula_.isEmpty() ? formula_.toStdString() : "" ) << xml::end;
    xos << xml::start( "gauge" );
    GetGauge().Serialize( xos );
    xos << xml::end;
    variables_->Serialize( xos );
    SerializeIndicators( xos );
    if( profiles_->Count() )
        profiles_->Serialize( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Score::SerializeIndicators
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
void Score::SerializeIndicators( xml::xostream& xos ) const
{
    xos << xml::start( "indicators" );
    SerializeIndicator( xos, QString( "indicator( ~%1~, %2 )" ).arg( name_ ).arg( formula_ ) );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Score::SerializeIndicator
// Created: SBO 2009-04-29
// -----------------------------------------------------------------------------
void Score::SerializeIndicator( xml::xostream& xos, const QString& formula ) const
{
    indicators::ElementFactory factory( indicators_, *variables_ );
    indicators::Serializer serializer( factory );
    indicators::FormulaParser parser( serializer );
    parser.Parse( formula.toStdString() );
    serializer.Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: Score::IsValid
// Created: SBO 2009-04-28
// -----------------------------------------------------------------------------
void Score::CheckValidity() const
{
    xml::xostringstream xos;
    xos << xml::start( "indicators" );
    SerializeIndicator( xos, formula_ );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Score::SetName
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
void Score::SetName( const QString& name )
{
    if( name_ != name )
    {
        name_ = name;
        if( controller_ )
            controller_->Update( *static_cast< Score_ABC* >( this ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Score::SetFormula
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
void Score::SetFormula( const QString& formula )
{
    if( formula_ != formula )
    {
        formula_ = formula;
        if( controller_ )
            controller_->Update( *static_cast< Score_ABC* >( this ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Score::SetGauge
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
void Score::SetGauge( const indicators::Gauge& gauge )
{
    gauge_.reset( new indicators::Gauge( gauge ) );
    if( controller_ )
        controller_->Update( *static_cast< Score_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: Score::SetVariables
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
void Score::SetVariables( const indicators::Variables& variables )
{
    *variables_ = variables;
    if( controller_ )
        controller_->Update( *static_cast< Score_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: Score::SetProfiles
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void Score::SetProfiles( std::unique_ptr< ProfileSelection > profiles )
{
    profiles_.swap( profiles );
    if( controller_ )
        controller_->Update( *static_cast< Score_ABC* >( this ) );
}
