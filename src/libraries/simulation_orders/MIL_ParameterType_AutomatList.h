// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_ParameterType_AutomatList_h_
#define __MIL_ParameterType_AutomatList_h_

#include "MIL_ParameterType_ABC.h"

// =============================================================================
/** @class  MIL_ParameterType_AutomatList
    @brief  MIL_ParameterType_AutomatList
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_ParameterType_AutomatList : public MIL_ParameterType_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_ParameterType_AutomatList( const std::string& name );
    virtual ~MIL_ParameterType_AutomatList();
    //@}

    //! @name Operations
    //@{
    virtual bool Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    //@}
};

#endif // __MIL_ParameterType_AutomatList_h_
