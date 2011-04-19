// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef gui_ArchitectureContainer_h
#define gui_ArchitectureContainer_h

#include "clients_kernel/Architecture_ABC.h"

namespace kernel
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
    explicit Architecture( kernel::PropertiesDictionary& dictionary );
    virtual ~Architecture();
    //@}

    //! @name Operations
    //@{
    virtual void Initialize( float height, unsigned int floorNumber, const std::string& roofShape,
                             const std::string& material, float occupation, float trafficability );
    virtual const std::string& GetMaterial() const;
    virtual unsigned int GetFloorNumber() const;
    virtual unsigned int GetOccupation() const;
    //@}

private:
    //! @name Member Data
    //@{
    kernel::PropertiesDictionary& dictionary_;
    std::string roofShape_;
    std::string material_;
    float height_;
    unsigned int floorNumber_;
    unsigned int occupation_;
    float trafficability_;
    //@}
};

}

#endif // gui_ArchitectureContainer_h
