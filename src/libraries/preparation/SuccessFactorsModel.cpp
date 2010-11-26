// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "SuccessFactorsModel.h"
#include "ModelChecker_ABC.h"
#include "SuccessFactor.h"
#include "SuccessFactorFactory_ABC.h"
#include "Tools.h"
#include "tools/ExerciseConfig.h"
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>
#include <xeuseuleu/xsl.hpp>

// -----------------------------------------------------------------------------
// Name: SuccessFactorsModel constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorsModel::SuccessFactorsModel( SuccessFactorFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorsModel destructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorsModel::~SuccessFactorsModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorsModel::Purge
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorsModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorsModel::Load
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorsModel::Load( const std::string& file )
{
    xml::xifstream xis( file );
    xis >> xml::start( "factors" )
            >> xml::list( "factor", *this, &SuccessFactorsModel::ReadFactor )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorsModel::Serialize
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorsModel::Serialize( const std::string& file ) const
{
    xml::xofstream xos( file );
    Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorsModel::Serialize
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorsModel::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "factors" );
    BOOST_FOREACH( const T_Elements::value_type factor, elements_ )
        factor.second->Serialize( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorsModel::SerializeScript
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorsModel::SerializeScript( const tools::ExerciseConfig& config ) const
{
    xsl::xftransform xft( tools::GeneralConfig::BuildResourceChildFile( "SuccessFactors.xsl" ), config.BuildExerciseChildFile( "scripts/success-factors.lua" ) );
    xft << xml::xifstream( config.GetSuccessFactorsFile() );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorsModel::CheckValidity
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
bool SuccessFactorsModel::CheckValidity( ModelChecker_ABC& checker ) const
{
    try
    {
        xml::xostringstream xos;
        Serialize( xos );
        xsl::xstringtransform xst( tools::GeneralConfig::BuildResourceChildFile( "SuccessFactors.xsl" ) );
        xml::xistringstream xis( xos.str() );
        xst << xis;
    }
    catch( std::exception& e )
    {
        return checker.Reject( tools::translate( "SuccessFactorsModel", "Success factor definitions contain errors:\nReason: %1." ).arg( e.what() ) );
    }
    return checker.Validate();
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorsModel::ReadFactor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorsModel::ReadFactor( xml::xistream& xis )
{
    SuccessFactor* factor = factory_.Create( xis );
    Register( factor->GetName(), *factor );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorsModel::Create
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorsModel::Create( const QString& name )
{
    SuccessFactor* factor = factory_.Create( name );
    Register( factor->GetName(), *factor );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorsModel::Delete
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorsModel::Delete( const SuccessFactor& factor )
{
    const QString name = factor.GetName();
    if( SuccessFactor* element = Find( name ) )
    {
        Remove( name );
        delete element;
    }
}