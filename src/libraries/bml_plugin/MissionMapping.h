// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionMapping_h_
#define __MissionMapping_h_

#include "tools/Resolver_ABC.h"

namespace kernel
{
    class MissionType;
}

namespace plugins
{
namespace bml
{
    unsigned long GetMissionIdFromCode( const tools::Resolver_ABC< kernel::MissionType >& missions, const std::string& code );
    std::string GetCodeFromMissionId( const tools::Resolver_ABC< kernel::MissionType >& missions, unsigned int id );
    std::string GetParameterTypeFromCode( const std::string& code );
}
}

#endif // __MissionMapping_h_