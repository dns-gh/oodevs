// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_GeneralConfig.h"
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace po = boost::program_options;

// -----------------------------------------------------------------------------
// Name: ADN_GeneralConfig constructor
// Created: LGY 2013-05-16
// -----------------------------------------------------------------------------
ADN_GeneralConfig::ADN_GeneralConfig( int argc, char** argv, const tools::Path& defaultRoot )
    : tools::GeneralConfig( defaultRoot )
{
    po::options_description desc( "Adaptation options" );
    desc.add_options()
        ( "input,i" , po::value( &inputFile_  )->default_value( "" ), "specify root input file (physical.xml)" )
        ( "output,o", po::value( &outputFile_ )->default_value( "" ), "specify output file (physical.xml) (open/save-mode: input must be specified)" )
        ( "create,c", po::value( &newFile_    )->default_value( "" ), "specify root file for creating new base (physical.xml)" )
        ( "swap,s", po::value( &swapLanguage_ )->default_value( "" ), "specify the language to swap with the master (input and output must be specified)" )
        ( "debug-qt-names", po::value( &qtNamesPath_ )->default_value( "" ), "Qt hierarchy names debug path" )
        ( "noreadonly", "disable read-only protection" )
        ;
    AddOptions( desc );
    Parse( argc, argv );
    noreadonly_ = IsSet( "noreadonly" );
}

// -----------------------------------------------------------------------------
// Name: ADN_GeneralConfig destructor
// Created: LGY 2013-05-16
// -----------------------------------------------------------------------------
ADN_GeneralConfig::~ADN_GeneralConfig()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_GeneralConfig::IsNoReadOnly
// Created: ABR 2013-09-11
// -----------------------------------------------------------------------------
bool ADN_GeneralConfig::IsNoReadOnly() const
{
    return noreadonly_;
}

// -----------------------------------------------------------------------------
// Name: ADN_GeneralConfig::GetInputFile
// Created: ABR 2013-09-11
// -----------------------------------------------------------------------------
const tools::Path& ADN_GeneralConfig::GetInputFile() const
{
    return inputFile_;
}

// -----------------------------------------------------------------------------
// Name: ADN_GeneralConfig::GetOutputFile
// Created: ABR 2013-09-11
// -----------------------------------------------------------------------------
const tools::Path& ADN_GeneralConfig::GetOutputFile() const
{
    return outputFile_;
}

// -----------------------------------------------------------------------------
// Name: ADN_GeneralConfig::GetNewFile
// Created: ABR 2013-09-11
// -----------------------------------------------------------------------------
const tools::Path& ADN_GeneralConfig::GetNewFile() const
{
    return newFile_;
}

// -----------------------------------------------------------------------------
// Name: ADN_GeneralConfig::GetSwapLanguage
// Created: ABR 2013-10-18
// -----------------------------------------------------------------------------
const std::string& ADN_GeneralConfig::GetSwapLanguage() const
{
    return swapLanguage_;
}

// -----------------------------------------------------------------------------
// Name: ADN_GeneralConfig::Config::GetQtNamesPath
// Created: ABR 2013-10-21
// -----------------------------------------------------------------------------
const tools::Path& ADN_GeneralConfig::GetQtNamesPath() const
{
    return qtNamesPath_;
}
