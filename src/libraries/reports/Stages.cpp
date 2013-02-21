// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "Stages.h"
#include "tools/ExerciseConfig.h"
#include "tools/Loader_ABC.h"
#include <tools/Exception.h>
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: Stages constructor
// Created: ABR 2013-02-20
// -----------------------------------------------------------------------------
Stages::Stages()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Stages destructor
// Created: ABR 2013-02-20
// -----------------------------------------------------------------------------
Stages::~Stages()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: Stages::Purge
// Created: ABR 2013-02-20
// -----------------------------------------------------------------------------
void Stages::Purge()
{
    stages_.clear();
}

// -----------------------------------------------------------------------------
// Name: Stages::Load
// Created: ABR 2013-02-20
// -----------------------------------------------------------------------------
void Stages::Load( const tools::ExerciseConfig& config )
{
    config.GetLoader().LoadPhysicalFile( "stages", boost::bind( &Stages::Read, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: Stages::Read
// Created: ABR 2013-02-20
// -----------------------------------------------------------------------------
void Stages::Read( xml::xistream& xis )
{
    xis >> xml::start( "stages" )
            >> xml::list( "stage", *this, &Stages::ReadStage )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: Stages::ReadStage
// Created: ABR 2013-02-20
// -----------------------------------------------------------------------------
void Stages::ReadStage( xml::xistream& xis )
{
    const std::string key = xis.attribute( "key", "" );
    const std::string translation = xis.attribute( "value", "" );
    if( !key.empty() && !translation.empty() )
    {
        if( stages_.find( key ) != stages_.end() )
        {
            std::stringstream str;
            str << "Stage already registered [key,value]: [" << key << "," << translation << "]";
            throw MASA_EXCEPTION( str.str() );
        }
        stages_[ key ] = translation;
    }
}

// -----------------------------------------------------------------------------
// Name: Stages::FindTranslation
// Created: ABR 2013-02-20
// -----------------------------------------------------------------------------
std::string Stages::FindTranslation( const std::string& key ) const
{
    if( stages_.find( key ) != stages_.end() )
        return stages_.at( key );
    return key;
}