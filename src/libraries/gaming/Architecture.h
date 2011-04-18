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

namespace sword
{
    class UrbanAttributes;
}

// =============================================================================
/** @class  Architecture
    @brief  Architecture
*/
// Created: LGY 2011-04-15
// =============================================================================
class Architecture : public kernel::Architecture_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Architecture( const sword::UrbanAttributes& message, std::auto_ptr< kernel::Architecture_ABC > pArchitecture );
    virtual ~Architecture();
    //@}

    //! @name Operations
    //@{
    virtual void Initialize( float height, unsigned int floorNumber, const std::string& roofShape,
                             const std::string& material, float occupation, float trafficability );
    virtual const std::string& GetMaterial() const;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< kernel::Architecture_ABC > pArchitecture_;
    //@}
};

#endif // __Architecture_h_
