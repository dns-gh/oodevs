// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __GaugeFactory_ABC_h_
#define __GaugeFactory_ABC_h_

namespace xml
{
    class xistream;
}

namespace indicators
{
    class Gauge;
    class GaugeTypes;

// =============================================================================
/** @class  GaugeFactory_ABC
    @brief  GaugeFactory_ABC
*/
// Created: SBO 2009-05-06
// =============================================================================
class GaugeFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             GaugeFactory_ABC() {}
    virtual ~GaugeFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual Gauge* Create() const = 0;
    virtual Gauge* Create( xml::xistream& xis ) const = 0;
    //@}
};

}

#endif // __GaugeFactory_ABC_h_
