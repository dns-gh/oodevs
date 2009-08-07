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
    explicit MIL_AgentParameter( DEC_RolePion_Decision* pAgent );
             MIL_AgentParameter( const ASN1T_Unit&, MIL_EntityManager_ABC& entityManager );
    virtual ~MIL_AgentParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToAgent( ASN1T_Unit& ) const;
    virtual bool ToAgent( DEC_Decision_ABC*& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_AgentParameter( const MIL_AgentParameter& );            //!< Copy constructor
    MIL_AgentParameter& operator=( const MIL_AgentParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    DEC_RolePion_Decision* pDecision_;
    //@}
};

#endif // __MIL_AgentParameter_h_
