// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ScoresModel.h"
#include "ModelChecker_ABC.h"
#include "Score_ABC.h"
#include "ScoreFactory_ABC.h"
#include "Tools.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: ScoresModel constructor
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
ScoresModel::ScoresModel( const ScoreFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoresModel destructor
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
ScoresModel::~ScoresModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Purge
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoresModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Load
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
void ScoresModel::Load( const std::string& file )
{
    xml::xifstream xis( file );
    xis >> xml::start( "scores" )
            >> xml::list( "score", *this, &ScoresModel::ReadScore )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::CheckValidity
// Created: SBO 2009-05-26
// -----------------------------------------------------------------------------
bool ScoresModel::CheckValidity( ModelChecker_ABC& checker ) const
{
    try
    {
        xml::xostringstream xos;
        Serialize( xos );
    }
    catch( std::exception& e )
    {
        return checker.Reject( tools::translate( "ScoresModel", "Score definitions contain errors:\nReason: %1." ).arg( e.what() ) );
    }
    return checker.Validate();
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Serialize
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
void ScoresModel::Serialize( const std::string& file ) const
{
    xml::xofstream xos( file );
    Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Serialize
// Created: SBO 2009-05-26
// -----------------------------------------------------------------------------
void ScoresModel::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "scores" )
            << xml::attribute( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" )
            << xml::attribute( "xsi:noNamespaceSchemaLocation", "schemas/exercise/scores.xsd" );
    BOOST_FOREACH( const T_Elements::value_type factor, elements_ )
        factor.second->Serialize( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::ReadScore
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
void ScoresModel::ReadScore( xml::xistream& xis )
{
    Score_ABC* score = factory_.CreateScore( xis );
    Register( score->GetName(), *score );
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Create
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoresModel::Create( const QString& name )
{
    Score_ABC* score = factory_.CreateScore( name );
    Register( score->GetName(), *score );
}

// -----------------------------------------------------------------------------
// Name: ScoresModel::Delete
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoresModel::Delete( const Score_ABC& score )
{
    const QString name = score.GetName();
    if( Score_ABC* element = Find( name ) )
    {
        Remove( name );
        delete element;
    }
}
