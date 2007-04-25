// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAgentList_h_
#define __ParamAgentList_h_

#include "game_asn/Asn.h"
#include "EntityListParameter.h"

namespace kernel
{
    class Agent_ABC;
}

// =============================================================================
/** @class  ParamAgentList
    @brief  ParamAgentList
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamAgentList : public EntityListParameter< kernel::Agent_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamAgentList( QObject* parent, const kernel::OrderParameter& parameter );
    virtual ~ParamAgentList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}
};

#endif // __ParamAgentList_h_
