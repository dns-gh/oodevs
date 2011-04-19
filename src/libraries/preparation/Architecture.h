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

#include "clients_kernel/Architecture_ABC.h"
#include <memory>

namespace urban
{
    class TerrainObject_ABC;
}

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
             Architecture( const urban::TerrainObject_ABC& object, std::auto_ptr< kernel::Architecture_ABC > pArchitecture );
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
    //! @name Member data
    //@{
    std::auto_ptr< kernel::Architecture_ABC > pArchitecture_;
    //@}
};

#endif // __Architecture_h_
