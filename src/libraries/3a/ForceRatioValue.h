// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ForceRatioValue_h_
#define __ForceRatioValue_h_

#include "Extractors.h"
#include "FilterHelper.h"

#include <map>

namespace extractors
{

// =============================================================================
/** @class  ForceRatioValue
    @brief  ForceRatioValue
*/
// Created: FPO 2011-07-07
// =============================================================================
class ForceRatioValue : public Extractor< NumericValue >
{

public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ForceRatioValue();
    explicit ForceRatioValue( xml::xistream& xis );
    virtual ~ForceRatioValue();
    //@}

    //! @name Operations
    //@{
    bool HasFlag( const sword::UnitAttributes& attributes ) const
    {
        return ( attributes.has_force_ratio() && attributes.force_ratio() != sword::ForceRatio::none );
    }
    int Extract( const sword::UnitAttributes& attributes );
    //@}

private:
    //! @name Member data
    //@{
    bool favorable_;
    bool neutral_;
    bool unfavorable_;
    //@}

};

}

#endif // __ForceRatioValue_h_
