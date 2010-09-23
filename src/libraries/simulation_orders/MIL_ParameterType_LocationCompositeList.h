// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ParameterType_LocationCompositeList_h_
#define __MIL_ParameterType_LocationCompositeList_h_

#include "MIL_ParameterType_ABC.h"

// =============================================================================
/** @class  MIL_ParameterType_LocationCompositeList
    @brief  MIL_ParameterType_LocationCompositeList
*/
// Created: LDC 2010-09-21
// =============================================================================
class MIL_ParameterType_LocationCompositeList : public MIL_ParameterType_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_ParameterType_LocationCompositeList( const std::string& name );
    virtual ~MIL_ParameterType_LocationCompositeList();
    //@}

    //! @name Operations
    //@{
    virtual bool Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    //@}
};

#endif // __MIL_ParameterType_LocationCompositeList_h_
