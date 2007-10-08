// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CreateExercise_h_
#define __CreateExercise_h_

namespace tools
{
    class GeneralConfig;
}

void CreateExercise( const tools::GeneralConfig& config, const std::string& name,
                     const std::string& terrain, const std::string& model, const std::string& physical = "" );

#endif // __CreateExercise_h_
