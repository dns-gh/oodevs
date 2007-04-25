// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAutomat_h_
#define __ParamAutomat_h_

#include "clients_kernel/Automat_ABC.h"
#include "EntityParameter.h"

// =============================================================================
/** @class  ParamAutomat
    @brief  ParamAutomat
*/
// Created: AGE 2006-11-29
// =============================================================================
class ParamAutomat : public EntityParameter< kernel::Automat_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamAutomat( QObject* parent, const kernel::OrderParameter& parameter );
    virtual ~ParamAutomat();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    //@}
};

#endif // __ParamAutomat_h_
