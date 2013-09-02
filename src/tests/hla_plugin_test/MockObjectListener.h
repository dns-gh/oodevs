// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef hla_plugin_MockObjectListener_h
#define hla_plugin_MockObjectListener_h

#include "hla_plugin/ObjectListener_ABC.h"
#include "rpr/Coordinates.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MockObjectListener
    @brief  Mock object MockObjectListener
*/
// Created: AHC 2012-02-27
// =============================================================================

MOCK_BASE_CLASS( MockObjectListener, ObjectListener_ABC )
{
    MOCK_METHOD( Moved, 3 )
    MOCK_METHOD( SideChanged, 2 )
    MOCK_METHOD( NameChanged, 2 )
    MOCK_METHOD( TypeChanged, 2 )
    MOCK_METHOD( EquipmentUpdated, 6 )
    MOCK_METHOD( UniqueIdChanged, 2 )
    MOCK_METHOD( CallsignChanged, 2 )
    MOCK_METHOD( EmbeddedUnitListChanged, 2 )
    MOCK_METHOD( GeometryChanged, 3 )
    MOCK_METHOD( ParentChanged, 2 )
    MOCK_METHOD( SubAgregatesChanged, 2 )
    MOCK_METHOD( SubEntitiesChanged, 2 )
    MOCK_METHOD( PropagationChanged, 8 )
};
}
}
#endif // hla_plugin_MockObjectListener_h
