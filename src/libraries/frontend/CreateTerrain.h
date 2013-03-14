// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef FRONTEND_CREATETERRAIN_H
#define FRONTEND_CREATETERRAIN_H

#include <boost/shared_ptr.hpp>

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{

class ProcessObserver_ABC;
class ProcessWrapper;

// Return true if the terrain generation application is available in the
// current environment, and CreateTerrain() has a chance to succeed.
bool IsTerrainAvailable();

// Spawn terrain creation application in a new attached process and return it.
boost::shared_ptr< frontend::ProcessWrapper > CreateTerrain(
    ProcessObserver_ABC& observer, const tools::GeneralConfig& config,
    const tools::Path& name );

}  // namespace frontend

#endif // FRONTEND_CREATETERRAIN_H
