// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CLIENTS_GUI_ARCHITECTURE_H__
#define CLIENTS_GUI_ARCHITECTURE_H__

#include "clients_kernel/Architecture_ABC.h"
#include "clients_kernel/Units.h"

namespace kernel
{
    class MaterialCompositionType;
    class RoofShapeType;
    class ObjectTypes;
    class UrbanObject_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace gui
{
// =============================================================================
/** @class  Architecture
    @brief  Architecture
*/
// Created: LGY 2011-04-14
// =============================================================================
class Architecture : public kernel::Architecture_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Architecture( kernel::UrbanObject_ABC& object, PropertiesDictionary& dictionary, const kernel::ObjectTypes& objectTypes );
    virtual ~Architecture();
    //@}

    //! @name Operations
    //@{
    virtual bool IsDefault() const;
    virtual void Initialize( unsigned int height, unsigned int floorNumber, unsigned int parkingFloors, float occupation,
                             float trafficability, const std::string& material = "", const std::string& roofShape = "" );
    virtual const kernel::MaterialCompositionType& GetMaterial() const;
    virtual const kernel::RoofShapeType& GetRoofShape() const;
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
    kernel::UrbanObject_ABC&            object_;
    PropertiesDictionary&               dictionary_;
    const kernel::ObjectTypes&          objectTypes_;
    kernel::MaterialCompositionType*    material_;
    kernel::RoofShapeType*              roofShape_;
    kernel::UnitedValue< unsigned int > height_;
    unsigned int                        floorNumber_;
    unsigned int                        parkingFloors_;
    kernel::UnitedValue< unsigned int > occupation_;
    kernel::UnitedValue< float >        trafficability_;
    //@}
};

} //! namespace kernel

#endif // CLIENTS_GUI_ARCHITECTURE_H__
