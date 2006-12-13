// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_ParameterType_AgentKnowledgeList_h_
#define __MIL_ParameterType_AgentKnowledgeList_h_

#include "MIL.h"

#include "MIL_ParameterType_ABC.h"

// =============================================================================
/** @class  MIL_ParameterType_AgentKnowledgeList
    @brief  MIL_ParameterType_AgentKnowledgeList
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_ParameterType_AgentKnowledgeList : public MIL_ParameterType_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_ParameterType_AgentKnowledgeList();
    virtual ~MIL_ParameterType_AgentKnowledgeList();
    //@}

    //! @name Operations
    //@{
    virtual void Copy                   ( const ASN1T_MissionParameter& from, DIA_Variable_ABC&       to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    virtual bool Copy                   ( const DIA_Variable_ABC&       from, DIA_Variable_ABC&       to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    virtual bool Copy                   ( const DIA_Variable_ABC&       from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    virtual void CleanAfterSerialization( ASN1T_MissionParameter& to ) const;
    //@}
};

#endif // __MIL_ParameterType_AgentKnowledgeList_h_
