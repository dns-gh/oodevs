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

#include "Extension_ABC.h"
#include "Displayable_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
// =============================================================================
/** @class  Architecture_ABC
    @brief  Architecture declaration
*/
// Created: LGY 2011-04-14
// =============================================================================
class Architecture_ABC : public Extension_ABC
                       , public Displayable_ABC
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
    virtual void Initialize( unsigned int height, unsigned int floorNumber, unsigned int parkingFloors, const std::string& roofShape,
                             const std::string& material, float occupation, float trafficability ) = 0;
    virtual const std::string& GetMaterial() const = 0;
    virtual unsigned int GetFloorNumber() const = 0;
    virtual unsigned int GetOccupation() const = 0;
    //@}
};

}

#endif // __Architecture_ABC_h_
