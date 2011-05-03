// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FireHumanDamages_h_
#define __FireHumanDamages_h_

#include "Extractors.h"
#include "FilterHelper.h"

namespace extractors
{

// =============================================================================
/** @class  FireHumanDamages
    @brief  FireHumanDamages
*/
// Created: FPO 2011-04-29
// =============================================================================
class FireHumanDamages : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             FireHumanDamages();
    /*implicit*/ FireHumanDamages( xml::xistream& xis );
    virtual ~FireHumanDamages();
    //@}

    //! @name Operations
    //@{
    bool HasValue( const sword::SimToClient& wrapper ) const
    {
        return wrapper.message().has_stop_unit_fire();
    }
    float Extract( const sword::SimToClient& wrapper ) const;
    //@}

private:
    //! @name Member data
    //@{
    int rankMask_;
    int stateMask_;
    FilterHelper< int > filter_;
    //@}
};

}

#endif // __FireHumanDamages_h_
