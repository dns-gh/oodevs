// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __UrbanPositions_h_
#define __UrbanPositions_h_

#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/UrbanPositions.h"
#include "clients_kernel/UrbanTypes.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class UrbanObject_ABC;
    class PropertiesDictionary;
}

// =============================================================================
/** @class  UrbanPositions
    @brief  Urban positions
*/
// Created: JSR 2010-09-06
// =============================================================================
class UrbanPositions : public kernel::UrbanPositions
                     , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanPositions( kernel::PropertiesDictionary& dictionary, EUrbanLevel level, const kernel::UrbanObject_ABC& object, const kernel::CoordinateConverter_ABC& converter );
             UrbanPositions( const geometry::Polygon2f& location, kernel::PropertiesDictionary& dictionary, EUrbanLevel level, const kernel::UrbanObject_ABC& object, const kernel::CoordinateConverter_ABC& converter );
             UrbanPositions( xml::xistream& xis, kernel::PropertiesDictionary& dictionary, EUrbanLevel level, const kernel::UrbanObject_ABC& object, const kernel::CoordinateConverter_ABC& converter );
    virtual ~UrbanPositions();
    //@}

    //! @name Serializable_ABC
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    EUrbanLevel level_;
    //@}

};

#endif // __UrbanPositions_h_
