// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_ParameterType_Automat_h_
#define __MIL_ParameterType_Automat_h_

#include "MIL_ParameterType_ABC.h"

// =============================================================================
/** @class  MIL_ParameterType_Automat
    @brief  MIL_ParameterType_Automat
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_ParameterType_Automat : public MIL_ParameterType_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_ParameterType_Automat( const std::string& name );
    virtual ~MIL_ParameterType_Automat();
    //@}

    //! @name Operations
    //@{
    virtual bool Copy                   ( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    virtual void CleanAfterSerialization( Common::MsgMissionParameter& to ) const;
    //@}
};

#endif // __MIL_ParameterType_Automat_h_
