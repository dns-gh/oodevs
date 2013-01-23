// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_MockRtiAmbassador_h
#define hla_MockRtiAmbassador_h

#include <hla/HLA_lib.h>

namespace hla
{
// =============================================================================
/** @class  MockRtiAmbassador
    @brief  Mock Rti ambassador
*/
// Created: SLI 2011-01-11
// =============================================================================
MOCK_BASE_CLASS( MockRtiAmbassador, hla::RtiAmbassador_ABC )
{
    MOCK_METHOD( Tick, 0 );
    MOCK_METHOD_EXT( Create, 2, bool( const std::string&, const std::string& ), CreateFom )
    MOCK_METHOD_EXT( Create, 2, bool( const std::string&, const std::vector< std::string >& ), CreateFoms )
    MOCK_METHOD( Destroy, 1 );
    MOCK_METHOD( Connect, 1 );
    MOCK_METHOD( Disconnect, 0 );
    MOCK_METHOD( Join, 3 );
    MOCK_METHOD( Resign, 0 );
    MOCK_METHOD_EXT( Subscribe, 2, void( const ClassIdentifier&, const Class_ABC& ), SubscribeClass );
    MOCK_METHOD_EXT( Publish, 2, void( const ClassIdentifier&, const Class_ABC& ), PublishClass );
    MOCK_METHOD_EXT( Subscribe, 1, void( const InteractionIdentifier& ), SubscribeInteraction );
    MOCK_METHOD_EXT( Publish, 1, void( const InteractionIdentifier& ), PublishInteraction );
    MOCK_METHOD( ReserveObjectInstance, 1 );
    MOCK_METHOD( RegisterObjectInstance, 2 );
    MOCK_METHOD( DeleteObjectInstance, 1 );
    MOCK_METHOD( UpdateAttributeValues, 3 );
    MOCK_METHOD( RequestObjectAttributeValueUpdate, 2 );
    MOCK_METHOD( SendInteraction, 3 );
    MOCK_METHOD( EnableTimeConstrained, 0 );
    MOCK_METHOD( EnableTimeRegulation, 1 );
    MOCK_METHOD( NextEventRequestAvailable, 1 );
    MOCK_METHOD( SynchronizationPointAchieved, 1 );
    MOCK_METHOD( RegisterSynchronizationPoint, 1 );
    MOCK_METHOD( UnconditionalOwnershipDivestiture, 2 );
    MOCK_METHOD( NegotiatedOwnershipDivestiture, 3 );
    MOCK_METHOD( ConfirmDivestiture, 3 );
    MOCK_METHOD( OwnershipAcquisition, 3 );
    MOCK_METHOD( OwnershipAcquisitionIfAvailable, 2 );
    MOCK_METHOD( OwnershipDivestitureIfWanted, 2 );
    MOCK_METHOD( CancelNegotiatedOwnershipDivestiture, 2 );
    MOCK_METHOD( CancelOwnershipAcquisition, 2 );
    MOCK_METHOD( QueryAttributeOwnership, 2 );
    MOCK_METHOD( IsAttributeOwnedByFederate, 2 );
};

}

#endif // hla_MockRtiAmbassador_h
