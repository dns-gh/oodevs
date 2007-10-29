// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Humans_h_
#define __Humans_h_

#include "Extractors.h"
#include "FilterHelper.h"
#pragma warning( push )
#pragma warning( disable : 4702 )
#include <map>
#pragma warning( pop )

namespace extractors
{

// =============================================================================
/** @class  Humans
    @brief  Humans
            ranks='officer,sub-officer,troopers'
            states='total,operational,dead,wounded,mental,nbc,in-treatment,in-maintenance'
*/
// Created: AGE 2007-10-29
// =============================================================================
class Humans : public Extractor< int >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
                 Humans();
    /*implicit*/ Humans( xml::xistream& xis );
    //@}

    //! @name Operations
    //@{
    bool HasFlag( const ASN1T_MsgUnitAttributes& attributes ) const
    { 
        return attributes.m.dotation_eff_personnelPresent;
    }
    int Extract( const ASN1T_MsgUnitAttributes& attributes );
    //@}

private:
    //! @name Member data
    //@{
    std::vector< int > humans_;
    int rankMask_;
    int stateMask_;
    //@}
};

}

#endif // __Humans_h_
