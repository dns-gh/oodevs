// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAutomatList_h_
#define __ParamAutomatList_h_

#include "game_asn/Asn.h"
#include "EntityListParameter.h"

namespace kernel
{
    class Automat_ABC;
}


// =============================================================================
/** @class  ParamAutomatList
    @brief  ParamAutomatList
*/
// Created: AGE 2006-11-29
// =============================================================================
class ParamAutomatList : public EntityListParameter< kernel::Automat_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamAutomatList( QObject* parent, const kernel::OrderParameter& parameter );
    virtual ~ParamAutomatList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}
};

#endif // __ParamAutomatList_h_
