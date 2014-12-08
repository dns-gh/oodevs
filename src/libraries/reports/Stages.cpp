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
#include "tools/PhyLoader.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>
#include <QtCore/qstring.h>

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
    config.GetPhyLoader().LoadPhysicalFile( "stages", boost::bind( &Stages::Read, this, _1 ) );
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
    if( !key.empty() )
    {
        if( stages_.find( key ) != stages_.end() )
            MT_LOG_ERROR_MSG( "Warning: Stage " << key << " translation is already registered. '" << stages_[ key ] << "' will become '" << translation << "'." );
        if( !translation.empty() )
            stages_[ key ] = translation;
        else
            stages_[ key ] = key;
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
    else
    {
        const std::string convertedKey = QString::fromLatin1( key.c_str() ).toStdString(); // $$$$ ABR 2013-02-21: key is coming from the decisional, so Latin1 encoding.
        if( stages_.find( convertedKey ) != stages_.end() )
            return stages_.at( convertedKey );
    }
    return key;
}
