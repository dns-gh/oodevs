// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Equipments_h_
#define __Equipments_h_

#include "Extractors.h"
#include "FilterHelper.h"
#pragma warning( push )
#pragma warning( disable : 4702 )
#include <map>
#pragma warning( pop )

namespace extractors
{

// =============================================================================
/** @class  Equipments
    @brief  Equipments
            equipments='id1,id2,...'
            states='available,unavailable,repairable,repairing,prisoner'
*/
// Created: AGE 2007-10-29
// =============================================================================
class Equipments : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
                 Equipments();
    /*implicit*/ Equipments( xml::xistream& xis );
    virtual     ~Equipments() {}
    //@}

    //! @name Operations
    //@{
    bool HasFlag( const sword::UnitAttributes& attributes ) const
    {
        return attributes.has_equipment_dotations();
    }
    int Extract( const sword::UnitAttributes& attributes );
    //@}

private:
    //! @name Member data
    //@{
    FilterHelper< int > filter_;
    std::map< int, int > equipments_;
    int stateMask_;
    //@}
};

}

#endif // __Equipments_h_
