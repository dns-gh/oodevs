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

#include "ASN_Types.h"
#include "EntityListParameter.h"

class Agent;

// =============================================================================
/** @class  ParamAgentList
    @brief  ParamAgentList
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamAgentList : public EntityListParameter< Agent >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamAgentList( QWidget* pParent, ASN1T_ListAgent& asn, const std::string& label, const std::string& menu );
    virtual ~ParamAgentList();
    //@}
};

#endif // __ParamAgentList_h_
