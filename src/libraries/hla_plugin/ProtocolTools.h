// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_protocol_tools_h_
#define plugins_hla_protocol_tools_h_

#include <geometry/Types.h>

namespace sword
{
    class MissionParameter;
}

namespace plugins
{
namespace hla
{

/// =============================================================================
/// @class ProtocolTools
/// @brief ProtocolTools
/// @thread This type is not thread safe
///
/// Created: ahc 2013/06/11
/// =============================================================================
class ProtocolTools
{
public:
    // In returned value X is latitude, Y is longitude
    static geometry::Point2d ConvertToPoint( const sword::MissionParameter& parameter );
};

}
}
#endif // plugins_hla_protocol_tools_h_
