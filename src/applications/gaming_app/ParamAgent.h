// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAgent_h_
#define __ParamAgent_h_

#include "EntityParameter.h"

namespace kernel
{
    class Agent_ABC;
}

// =============================================================================
/** @class  ParamAgent
    @brief  ParamAgent
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamAgent : public EntityParameter< kernel::Agent_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamAgent( QWidget* pParent, ASN1T_OID& id, const QString& label, const QString& menu );
    virtual ~ParamAgent();
    //@}
};

#endif // __ParamAgent_h_
