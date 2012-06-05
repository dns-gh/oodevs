// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __kernel_Architecture_h_
#define __kernel_Architecture_h_

#include "Architecture_ABC.h"

namespace kernel
{
    class MaterialCompositionType;
    class PropertiesDictionary;
    class RoofShapeType;
    class ObjectTypes;

// =============================================================================
/** @class  Architecture
    @brief  Architecture
*/
// Created: LGY 2011-04-14
// =============================================================================
class Architecture : public Architecture_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Architecture( PropertiesDictionary& dictionary );
    virtual ~Architecture();
    //@}

    //! @name Operations
    //@{
    virtual void Initialize( const ObjectTypes& objectTypes, float height, unsigned int floorNumber, unsigned int parkingFloors, float occupation,
                             float trafficability, const std::string& material = "", const std::string& roofShape = "" );
    virtual const MaterialCompositionType& GetMaterial() const;
    virtual const RoofShapeType& GetRoofShape() const;
    virtual unsigned int GetFloorNumber() const;
    virtual unsigned int GetParkingFloors() const;
    virtual unsigned int GetOccupation() const;
    virtual unsigned int GetHeight() const;
    virtual float GetTrafficability() const;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void CreateDictionnary( bool readOnly );
    //@}

protected:
    //! @name Member Data
    //@{
    PropertiesDictionary&    dictionary_;
    MaterialCompositionType* material_;
    RoofShapeType*           roofShape_;
    float                    height_;
    unsigned int             floorNumber_;
    unsigned int             parkingFloors_;
    unsigned int             occupation_;
    float                    trafficability_;
    //@}
};

} //! namespace kernel

#endif // __kernel_Architecture_h_
