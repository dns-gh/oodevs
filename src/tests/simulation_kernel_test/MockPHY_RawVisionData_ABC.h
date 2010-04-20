// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockPHY_RawVisionData_ABC_h_
#define __MockPHY_RawVisionData_ABC_h_

#include "meteo/PHY_RawVisionData_ABC.h"
#include "geometry/Types.h"

class PHY_Meteo;

// =============================================================================
/** @class  MockPHY_RawVisionData_ABC
@brief  MockPHY_RawVisionData_ABC
*/
// Created: SLG 2010-03-29
// =============================================================================
class MockPHY_RawVisionData_ABC
    : public mockpp::ChainableMockObject
    , public PHY_RawVisionData_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    MockPHY_RawVisionData_ABC()
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockPHY_RawVisionData_ABC" ) )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( RegisterMeteoPatchShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( UnregisterMeteoPatchShadow ) {}
    virtual ~MockPHY_RawVisionData_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void RegisterMeteoPatch( const geometry::Point2d& point1, const geometry::Point2d& point2, PHY_Meteo* meteo )
    {
        RegisterMeteoPatchShadow( &point1, &point2, meteo );
    }

    virtual void UnregisterMeteoPatch( const geometry::Point2d& point1, const geometry::Point2d& point2, PHY_Meteo* meteo )
    {
        UnregisterMeteoPatchShadow( &point1, &point2, meteo );
    }

    MOCKPP_VOID_CHAINABLE3( MockPHY_RawVisionData_ABC, RegisterMeteoPatchShadow, const geometry::Point2d* point1, const geometry::Point2d* point2, PHY_Meteo* meteo );
    MOCKPP_VOID_CHAINABLE3( MockPHY_RawVisionData_ABC, UnregisterMeteoPatchShadow, const geometry::Point2d* point1, const geometry::Point2d* point2, PHY_Meteo* meteo );
    //@}
};

#endif // __MockPHY_RawVisionData_ABC_h_
