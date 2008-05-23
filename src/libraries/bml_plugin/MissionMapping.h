// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionMapping_h_
#define __MissionMapping_h_

#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class MissionType;
}

namespace bml
{
    std::string GetMissionNameFromCode( const std::string& code );
    std::string GetCodeFromMissionId( const kernel::Resolver_ABC< kernel::MissionType >& missions, unsigned int id );
    std::string GetParameterTypeFromCode( const std::string& code );
}

#endif // __MissionMapping_h_
