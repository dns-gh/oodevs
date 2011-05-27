// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __CrowdHumanDamages_h_
#define __CrowdHumanDamages_h_

#include "Extractors.h"
#include "FilterHelper.h"

namespace extractors
{

// =============================================================================
/** @class  CrowdHumanDamages
    @brief  CrowdHumanDamages
*/
// Created: FPO 2011-05-26
// =============================================================================
class CrowdHumanDamages : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
                 CrowdHumanDamages();
    /*implicit*/ CrowdHumanDamages( xml::xistream& xis );
    virtual     ~CrowdHumanDamages();
    //@}

    //! @name Operations
    //@{
    bool HasValue( const sword::SimToClient& wrapper ) const
    {
        return wrapper.message().has_unit_damaged_by_crowd_fire();
    }
    int Extract( const sword::SimToClient& wrapper ) const;
    //@}

private:
    //! @name Member data
    //@{
    int rankMask_;
    int stateMask_;
    //@}
};

}

#endif // __CrowdHumanDamages_h_
