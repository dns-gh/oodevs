// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_AgentParameter_h_
#define __MIL_AgentParameter_h_

#include "MIL_BaseParameter.h"

class DEC_RolePion_Decision;
class MIL_EntityManager_ABC;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_AgentParameter
    @brief  MIL_AgentParameter
*/
// Created: LDC 2009-05-25
// =============================================================================
class MIL_AgentParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_AgentParameter();
    explicit MIL_AgentParameter( DEC_RolePion_Decision* pAgent );
             MIL_AgentParameter( const sword::UnitId&, const MIL_EntityManager_ABC& entityManager );
    virtual ~MIL_AgentParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToAgent( DEC_Decision_ABC*& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@}

private:
    //! @name Member data
    //@{
    DEC_RolePion_Decision* pDecision_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_AgentParameter )

#endif // __MIL_AgentParameter_h_
