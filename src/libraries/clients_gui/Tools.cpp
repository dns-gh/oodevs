// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Tools.h"

namespace gui
{

bool IsPropagationDir( const tools::Path& dir )
{
    return dir.IsDirectory() && ( dir / "propagation.xml" ).Exists();
}

}  // namespace gui

