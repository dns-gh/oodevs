// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_MockFOM_Serialization_h
#define hla_plugin_MockFOM_Serialization_h

#include "hla_plugin/FOM_Serializer_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockFOM_Serialization
    @brief  MockFOM_Serialization
*/
// Created: AHC 2012-09-03
// =============================================================================
MOCK_BASE_CLASS( MockFOM_Serialization, FOM_Serializer_ABC )
{
    MOCK_METHOD( ReadSpatial, 4 )
    MOCK_METHOD( ReadForceIdentifier, 4 )
    MOCK_METHOD( ReadAggregateMarking, 4 )
    MOCK_METHOD( ReadEntityType, 4 )
    MOCK_METHOD( ReadNumberOfSilentEntities, 4 )
    MOCK_METHOD( ReadSilentEntities, 4 )
    MOCK_METHOD( ReadEntityIdentifier, 5 )
    MOCK_METHOD( ReadNothing, 3 )
    MOCK_METHOD( ReadUnsignedChar, 4 )
    MOCK_METHOD( ReadMarking, 4 )
    MOCK_METHOD( ReadIsPartOf, 4 )

    MOCK_METHOD( ReadCallsign, 4 )
    MOCK_METHOD( ReadUniqueId, 4 )
    MOCK_METHOD( ReadMounted, 4 )
    MOCK_METHOD( ReadSymbol, 4 )
    MOCK_METHOD( ReadStatus, 4 )
    MOCK_METHOD( ReadEmbeddedUnitList, 3 )

    MOCK_METHOD( GetUniqueIdSerializer, 0)
};

}
}

#endif // hla_plugin_MockFOM_Serialization_h
