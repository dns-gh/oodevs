// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_ParameterType_NatureAtlas_h_
#define __MIL_ParameterType_NatureAtlas_h_

#include "MIL_ParameterType_ABC.h"

// =============================================================================
/** @class  MIL_ParameterType_NatureAtlas
    @brief  MIL_ParameterType_NatureAtlas
*/
// Created: SBO 2006-11-27
// =============================================================================
class MIL_ParameterType_NatureAtlas : public MIL_ParameterType_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_ParameterType_NatureAtlas();
    virtual ~MIL_ParameterType_NatureAtlas();
    //@}

    //! @name Operations
    //@{
    virtual bool Copy                   ( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    virtual void CleanAfterSerialization( ASN1T_MissionParameter& to ) const;
    //@}
};

#endif // __MIL_ParameterType_NatureAtlas_h_
