// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_ParameterType_ObjectiveList_h_
#define __MIL_ParameterType_ObjectiveList_h_

#include "MIL_ParameterType_ABC.h"

// =============================================================================
/** @class  MIL_ParameterType_ObjectiveList
    @brief  MIL_ParameterType_ObjectiveList
*/
// Created: SBO 2006-11-27
// =============================================================================
class MIL_ParameterType_ObjectiveList : public MIL_ParameterType_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_ParameterType_ObjectiveList();
    virtual ~MIL_ParameterType_ObjectiveList();
    //@}

    //! @name Operations
    //@{
    virtual bool Copy                   ( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    virtual void CleanAfterSerialization( Common::MsgMissionParameter& to ) const;
    //@}
};

#endif // __MIL_ParameterType_ObjectiveList_h_
