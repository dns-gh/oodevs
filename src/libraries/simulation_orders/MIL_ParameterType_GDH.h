// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_ParameterType_GDH_h_
#define __MIL_ParameterType_GDH_h_

#include "MIL_ParameterType_ABC.h"

// =============================================================================
/** @class  MIL_ParameterType_GDH
    @brief  MIL_ParameterType_GDH
*/
// Created: SBO 2006-11-27
// =============================================================================
class MIL_ParameterType_GDH : public MIL_ParameterType_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_ParameterType_GDH();
    virtual ~MIL_ParameterType_GDH();
    //@}

    //! @name Operations
    //@{
    virtual bool Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    //@}
};

#endif // __MIL_ParameterType_GDH_h_
