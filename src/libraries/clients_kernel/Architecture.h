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
    class PropertiesDictionary;

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
    virtual void Initialize( float height, unsigned int floorNumber, unsigned int parkingFloors, const std::string& roofShape,
                             const std::string& material, float occupation, float trafficability );
    virtual const std::string& GetMaterial() const;
    virtual unsigned int GetFloorNumber() const;
    virtual unsigned int GetOccupation() const;
    virtual unsigned int GetHeight() const;
    //@}

protected:
    //! @name Member Data
    //@{
    PropertiesDictionary& dictionary_;
    std::string           roofShape_;
    std::string           material_;
    float                 height_;
    unsigned int          floorNumber_;
    unsigned int          parkingFloors_;
    unsigned int          occupation_;
    float                 trafficability_;
    //@}
};

} //! namespace kernel

#endif // __kernel_Architecture_h_
