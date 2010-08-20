// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ParameterType_LocationComposite_h_
#define __MIL_ParameterType_LocationComposite_h_

#include "MIL_ParameterType_ABC.h"

// =============================================================================
/** @class  MIL_ParameterType_LocationComposite
    @brief  MIL_ParameterType_LocationComposite
*/
// Created: LDC 2010-08-18
// =============================================================================
class MIL_ParameterType_LocationComposite : public MIL_ParameterType_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_ParameterType_LocationComposite( const std::string& name );
    virtual ~MIL_ParameterType_LocationComposite();
    //@}

    //! @name Operations
    //@{
    virtual bool Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& knowledgeResolver, bool bIsOptional ) const;
    //@}
};

#endif // __MIL_ParameterType_LocationComposite_h_
