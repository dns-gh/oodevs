// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockFederate_h
#define hla_plugin_MockFederate_h

#include "hla_plugin/Federate_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockFederate
    @brief  Mock federate
*/
// Created: SLI 2011-06-10
// =============================================================================
MOCK_BASE_CLASS( MockFederate, Federate_ABC )
{
    MOCK_METHOD( Connect, 0 )
    MOCK_METHOD( Disconnect, 0 )
    MOCK_METHOD( Create, 2 )
    MOCK_METHOD( Destroy, 1 )
    MOCK_METHOD( Join, 3 )
    MOCK_METHOD( Resign, 0 )
    MOCK_METHOD( Step, 0 )
    MOCK_METHOD_EXT( Register, 4, void( const ::hla::ClassIdentifier&, ::hla::Class_ABC&, bool, bool ), RegisterClass )
    MOCK_METHOD_EXT( Register, 4, void( const ::hla::InteractionIdentifier&, ::hla::Interaction_ABC&, bool, bool ), RegisterInteraction )
    MOCK_METHOD_EXT( Register, 1, void( ::hla::FederateAmbassador_ABC& ), RegisterListener )
};

}
}

#endif // hla_plugin_MockFederate_h
