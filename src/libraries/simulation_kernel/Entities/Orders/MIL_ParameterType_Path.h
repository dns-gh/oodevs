// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_ParameterType_Path_h_
#define __MIL_ParameterType_Path_h_

#include "MIL_ParameterType_ABC.h"

// =============================================================================
/** @class  MIL_ParameterType_Path
    @brief  MIL_ParameterType_Path
*/
// Created: SBO 2006-11-27
// =============================================================================
class MIL_ParameterType_Path : public MIL_ParameterType_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_ParameterType_Path();
    virtual ~MIL_ParameterType_Path();
    //@}

    //! @name Operations
    //@{
    virtual void Copy                   ( const ASN1T_MissionParameter& from, DIA_Variable_ABC&       to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    virtual bool Copy                   ( const DIA_Variable_ABC&       from, DIA_Variable_ABC&       to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    virtual bool Copy                   ( const DIA_Variable_ABC&       from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    virtual void CleanAfterSerialization( ASN1T_MissionParameter& to ) const;
    //@}
};

#endif // __MIL_ParameterType_Path_h_
