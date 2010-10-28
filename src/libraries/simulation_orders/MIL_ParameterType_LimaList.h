// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_ParameterType_LimaList_h_
#define __MIL_ParameterType_LimaList_h_

#include "MIL_ParameterType_ABC.h"

// =============================================================================
/** @class  MIL_ParameterType_LimaList
    @brief  MIL_ParameterType_LimaList
*/
// Created: MGD 2010-10-27
// =============================================================================
class MIL_ParameterType_LimaList : public MIL_ParameterType_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_ParameterType_LimaList();
    virtual ~MIL_ParameterType_LimaList();
    //@}

    //! @name Operations
    //@{
    virtual bool Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    //@}
};

#endif // __MIL_ParameterType_LimaList_h_
