// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
    explicit MIL_ParameterType_Path( const std::string& name );
    virtual ~MIL_ParameterType_Path();
    //@}

    //! @name Operations
    //@{
    virtual bool Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    //@}
};

#endif // __MIL_ParameterType_Path_h_
