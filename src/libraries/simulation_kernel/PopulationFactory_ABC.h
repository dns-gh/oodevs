// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PopulationFactory_ABC_h_
#define __PopulationFactory_ABC_h_

#include "tools/Resolver.h"
#include "Entities/Populations/MIL_Population.h"

namespace xml
{
    class xistream;
}

class MIL_Army;
// =============================================================================
/** @class  PopulationFactory_ABC
    @brief  PopulationFactory_ABC
*/
// Created: MGD 2009-10-24
// =============================================================================
class PopulationFactory_ABC : public tools::Resolver< MIL_Population >
                      , private boost::noncopyable
{

public:
    //! @name Destructor
    //@{
    virtual ~PopulationFactory_ABC() { DeleteAll(); };
    //@}

    //! @name Operations
    //@{
    virtual MIL_Population& Create( xml::xistream& xis, MIL_Army& army ) = 0;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive& file, const uint );
    //@}

protected:
    //! @name Constructor
    //@{
    PopulationFactory_ABC() {};
    //@}
};

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void PopulationFactory_ABC::serialize
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void PopulationFactory_ABC::serialize( Archive& file, const uint )
{
    file & elements_;
}

#endif // __PopulationFactory_ABC_h_
