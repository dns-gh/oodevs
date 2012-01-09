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
#include "SuccessFactor.h"
#include "SuccessFactorFactory_ABC.h"
#include "clients_kernel/Tools.h"
#include "tools/ExerciseConfig.h"
#include "tools/Loader_ABC.h"
#include "tools/SchemaWriter_ABC.h"
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
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
void SuccessFactorsModel::Load( const tools::Loader_ABC& fileLoader, const std::string& file )
{
    fileLoader.LoadFile( file, boost::bind( &SuccessFactorsModel::Read, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorsModel::Read
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorsModel::Read( xml::xistream& xis )
{
    xis >> xml::start( "factors" )
            >> xml::list( "factor", *this, &SuccessFactorsModel::ReadFactor )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorsModel::Serialize
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorsModel::Serialize( const std::string& file, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    xml::xofstream xos( file );
    Serialize( xos, schemaWriter );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorsModel::Serialize
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorsModel::Serialize( xml::xostream& xos, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    xos << xml::start( "factors" );
    schemaWriter.WriteExerciseSchema( xos, "success-factors" );
    BOOST_FOREACH( const T_Elements::value_type factor, elements_ )
        factor.second->Serialize( xos );
    xos << xml::end;
}

namespace
{
    void EraseLine( std::string filename, unsigned lineToErase )
    {
        std::string buffer = "";
        std::ifstream inputFile( filename.c_str() );
        if( inputFile )
        {
            std::string line;
            for( unsigned lineNumber = 0; std::getline( inputFile, line ); ++lineNumber )
                if( lineNumber != lineToErase )
                    buffer += line + "\n";
        }
        inputFile.close();

        std::ofstream outputFile( filename.c_str() );
        outputFile << buffer;
        outputFile.close();
    }
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorsModel::SerializeScript
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorsModel::SerializeScript( const tools::ExerciseConfig& config ) const
{
    {
        xsl::xftransform xft( tools::GeneralConfig::BuildResourceChildFile( "SuccessFactors.xsl" ), config.BuildExerciseChildFile( "scripts/success-factors.lua" ) );
        xft << xml::xifstream( config.GetSuccessFactorsFile() );
    }
    EraseLine( config.BuildExerciseChildFile( "scripts/success-factors.lua" ), 0 ); // $$$$ ABR 2011-07-26: Temporaire, la transformation xsl de Xalan crash en vc100 (Mantis 5915)
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