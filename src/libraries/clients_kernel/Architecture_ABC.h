// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Architecture_ABC_h_
#define __Architecture_ABC_h_

#include "Displayable_ABC.h"
#include "Extension_ABC.h"
#include "Serializable_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class MaterialCompositionType;
    class RoofShapeType;

// =============================================================================
/** @class  Architecture_ABC
    @brief  Architecture declaration
*/
// Created: LGY 2011-04-14
// =============================================================================
class Architecture_ABC : public Extension_ABC
                       , public Displayable_ABC
                       , public Serializable_ABC
                       , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Architecture_ABC() {}
    virtual ~Architecture_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Initialize( unsigned int height, unsigned int floorNumber, unsigned int parkingFloors, float occupation,
                             float trafficability, const std::string& material = "", const std::string& roofShape = "" ) = 0;
    virtual const MaterialCompositionType& GetMaterial() const = 0;
    virtual const RoofShapeType& GetRoofShape() const = 0;
    virtual unsigned int GetFloorNumber() const = 0;
    virtual unsigned int GetParkingFloors() const = 0;
    virtual unsigned int GetOccupation() const = 0;
    virtual unsigned int GetHeight() const = 0;
    virtual float GetTrafficability() const = 0;
    virtual void SetMaterial( MaterialCompositionType& material ) = 0;
    virtual void SetRoofShape( RoofShapeType& roofShape ) = 0;
    virtual void SetFloorNumber( unsigned int floorNumber ) = 0;
    virtual void SetParkingFloors( unsigned int parkingFloors ) = 0;
    virtual void SetOccupation( unsigned int occupation ) = 0;
    virtual void SetHeight( unsigned int height ) = 0;
    virtual void SetTrafficability( float trafficability) = 0;
    virtual const std::string& GetInvalidMaterial() const = 0;
    virtual const std::string& GetInvalidRoofShape() const = 0;
    //@}
};

}

#endif // __Architecture_ABC_h_
