// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Architecture_h_
#define __Architecture_h_

#include "clients_gui/Architecture.h"

namespace sword
{
    class UrbanAttributes;
}

namespace kernel
{
    class ObjectTypes;
    class PropertiesDictionary;
    class UrbanObject_ABC;
}

// =============================================================================
/** @class  Architecture
    @brief  Architecture
*/
// Created: LGY 2011-04-15
// =============================================================================
class Architecture : public gui::Architecture
{
public:
    //! @name Constructors/Destructor
    //@{
             Architecture( const sword::UrbanAttributes& message, kernel::UrbanObject_ABC& object, kernel::PropertiesDictionary& dictionary, const kernel::ObjectTypes& objectTypes );
    virtual ~Architecture();

    virtual void SetMaterial( kernel::MaterialCompositionType& material );
    virtual void SetRoofShape( kernel::RoofShapeType& roofShape );
    virtual void SetFloorNumber( unsigned int floorNumber );
    virtual void SetParkingFloors( unsigned int parkingFloors );
    virtual void SetOccupation( unsigned int occupation );
    virtual void SetHeight( unsigned int height );
    virtual void SetTrafficability( float trafficability);
    //@}
};

#endif // __Architecture_h_
