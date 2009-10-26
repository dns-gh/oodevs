// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_ParameterType_PointList_h_
#define __MIL_ParameterType_PointList_h_

#include "MIL_ParameterType_ABC.h"

// =============================================================================
/** @class  MIL_ParameterType_PointList
    @brief  MIL_ParameterType_PointList
*/
// Created: SBO 2006-11-27
// =============================================================================
class MIL_ParameterType_PointList : public MIL_ParameterType_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_ParameterType_PointList( const std::string& name );
    virtual ~MIL_ParameterType_PointList();
    //@}

    //! @name Operations
    //@{
    virtual bool Copy                   ( const MIL_MissionParameter_ABC& from, ASN1T_MissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    virtual void CleanAfterSerialization( ASN1T_MissionParameter& to ) const;
    //@}
};

#endif // __MIL_ParameterType_PointList_h_
