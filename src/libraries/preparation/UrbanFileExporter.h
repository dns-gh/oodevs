// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanFileExporter_h_
#define __UrbanFileExporter_h_

#include <string>

class PointProjector_ABC;
class UrbanModel;

void ExportUrbanFiles( const tools::Path& directory, const tools::Path& name,
                       PointProjector_ABC& projector, const UrbanModel& model );

#endif // __UrbanFileExporter_h_
