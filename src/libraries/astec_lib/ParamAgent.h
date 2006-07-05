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

class Agent;

// =============================================================================
/** @class  ParamAgent
    @brief  ParamAgent
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamAgent : public EntityParameter< Agent >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamAgent( QWidget* pParent, ASN1T_OID& id, const std::string& label, const std::string& menu );
    virtual ~ParamAgent();
    //@}
};

#endif // __ParamAgent_h_
