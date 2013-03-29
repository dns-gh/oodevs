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
    MOCK_METHOD( Create, 2, bool ( const std::string& federation, const std::string& fomFile ), Create1 )
    MOCK_METHOD( Create, 2, bool ( const std::string& federation, const T_FomFiles& fomFiles ), CreateV )
    MOCK_METHOD( Destroy, 1 )
    MOCK_METHOD( Join, 3 )
    MOCK_METHOD( Resign, 0 )
    MOCK_METHOD( Step, 0 )
    MOCK_METHOD( Tick, 0 )
    MOCK_METHOD( Register, 4, void( const ::hla::ClassIdentifier&, ::hla::Class_ABC&, bool, bool ), RegisterClass )
    MOCK_METHOD( Register, 4, void( const ::hla::InteractionIdentifier&, ::hla::Interaction_ABC&, bool, bool ), RegisterInteraction )
    MOCK_METHOD( Register, 1, void( ::hla::FederateAmbassador_ABC& ), RegisterListener )
    MOCK_METHOD( DivestRequest, 3 )
    MOCK_METHOD( UnconditionalDivest, 2 )
    MOCK_METHOD( AcquisitionRequest, 2 )
    MOCK_METHOD( UnconditionalAcquisition, 3 )
    MOCK_METHOD( GetFederateHandle, 0 )
};

}
}

#endif // hla_plugin_MockFederate_h
