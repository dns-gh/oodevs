// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
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
class FireComponentDamages: public Extractor< NumericValue >
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
    //@}

    //! @name Operations
    //@{
    bool HasValue( const ASN1T_MsgsSimToClient& message ) const
    { 
        return message.msg.t == T_MsgsSimToClient_msg_msg_stop_unit_fire;
    }
    float Extract( const ASN1T_MsgsSimToClient& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    FilterHelper< int > filter_;
    //@}
};

}

#endif // __FireComponentDamages_h_
