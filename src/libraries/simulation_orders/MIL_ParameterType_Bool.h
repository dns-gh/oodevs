// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_ParameterType_Bool_h_
#define __MIL_ParameterType_Bool_h_

#include "MIL_ParameterType_ABC.h"

// =============================================================================
/** @class  MIL_ParameterType_Bool
    @brief  MIL_ParameterType_Bool
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_ParameterType_Bool : public MIL_ParameterType_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_ParameterType_Bool();
    virtual ~MIL_ParameterType_Bool();
    //@}

    //! @name Operations
    //@{
    virtual bool Copy                   ( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    virtual void CleanAfterSerialization( ASN1T_MissionParameter& to ) const;
    //@}
};

#endif // __MIL_ParameterType_Bool_h_
