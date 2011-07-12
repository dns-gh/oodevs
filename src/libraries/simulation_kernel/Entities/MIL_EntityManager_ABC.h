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
class MIL_Army_ABC;
class MIL_Automate;
class MIL_EffectManager;
class MIL_EntityManager;
class MIL_KnowledgeGroup; // LTO
class MIL_Object_ABC;
class MIL_Population;
class MIL_ObjectBuilder_ABC;
class MIL_ObjectType_ABC;
class TER_Localisation;
class MIL_Object_ABC;

namespace sword
{
    class MissionParameter_Value;
}

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
             MIL_EntityManager_ABC();
    virtual ~MIL_EntityManager_ABC();
    //@}

    //! @name Operations
    //@{
    virtual       MIL_AgentPion*      FindAgentPion     ( unsigned int nID ) const = 0;
    virtual       MIL_Automate*       FindAutomate      ( unsigned int nID ) const = 0;
    virtual       MIL_Population*     FindPopulation    ( unsigned int nID ) const = 0;
    virtual       MIL_Object_ABC*     FindObject        ( unsigned int nID ) const = 0;
    virtual       MIL_KnowledgeGroup* FindKnowledgeGroup( unsigned int nID ) const = 0; // LTO
    virtual const MIL_ObjectType_ABC& FindObjectType    ( const std::string& type ) const = 0;

    virtual MIL_Object_ABC* CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation ) = 0;
    virtual MIL_Object_ABC* CreateObject( MIL_Army_ABC& army, const MIL_ObjectBuilder_ABC& builder ) = 0;

    static MIL_EntityManager_ABC& GetSingleton();
    //@}
};

#endif // __MIL_EntityManager_ABC_h_
