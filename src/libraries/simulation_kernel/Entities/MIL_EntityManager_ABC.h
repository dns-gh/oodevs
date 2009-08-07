// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_EntityManager_ABC_h_
#define __MIL_EntityManager_ABC_h_

class MIL_AgentPion;
class MIL_Automate;
class MIL_ObjectType_ABC;

// =============================================================================
/** @class  MIL_EntityManager_ABC
    @brief  MIL_EntityManager_ABC
*/
// Created: LDC 2009-06-09
// =============================================================================
class MIL_EntityManager_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_EntityManager_ABC() {}
    virtual ~MIL_EntityManager_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual       MIL_AgentPion*      FindAgentPion ( unsigned int nID ) const = 0;
    virtual       MIL_Automate*       FindAutomate  ( unsigned int nID ) const = 0;
    virtual const MIL_ObjectType_ABC& FindObjectType( const std::string& type ) const = 0;
    //@}
};

#endif // __MIL_EntityManager_ABC_h_
