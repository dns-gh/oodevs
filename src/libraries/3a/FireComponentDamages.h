// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __FireComponentDamages_h_
#define __FireComponentDamages_h_

#include "Extractors.h"
#include "FilterHelper.h"

namespace extractors
{

// =============================================================================
/** @class  FireComponentDamages
    @brief  FireComponentDamages
            components='id1,id2,...'
*/
// Created: AGE 2007-10-24
// =============================================================================
class FireComponentDamages : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
                 FireComponentDamages();
    /*implicit*/ FireComponentDamages( xml::xistream& xis );
    virtual     ~FireComponentDamages() {}
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
    FilterHelper< int > filter_;
    //@}
};

}

#endif // __FireComponentDamages_h_
