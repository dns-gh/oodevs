// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef CLIENTS_GUI_GAMINGCOMMONCONFIG_H
#define CLIENTS_GUI_GAMINGCOMMONCONFIG_H

#include "tools/ExerciseConfig.h"

namespace boost
{
    namespace program_options
    {
        class options_description;
    }
}

namespace tools
{
    class RealFileLoaderObserver_ABC;
}

namespace gui
{

struct GamingCommonConfig
{
    GamingCommonConfig()
        : hasMapnik( false )
        , mapnikThreads( 0 )
    {}

    bool hasMapnik;
    uint32_t mapnikThreads;
};

void AddGamingCommonOptions( boost::program_options::options_description& desc,
       GamingCommonConfig& config );

}  // namespace gui

#endif // CLIENTS_GUI_GAMINGCOMMONCONFIG_H
