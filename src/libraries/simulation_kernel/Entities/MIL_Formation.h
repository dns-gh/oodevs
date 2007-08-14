// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Formation_h_
#define __MIL_Formation_h_

#include "MIL.h"

class MIL_Army;
class PHY_NatureLevel;
class MIL_EntityManager;
class MIL_TacticalLineManager;

// =============================================================================
/** @class  MIL_Formation
    @brief  MIL_Formation
*/
// Created: NLD 2006-10-11
// =============================================================================
class MIL_Formation
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Formation( MIL_EntityManager& manager, MIL_TacticalLineManager& tacticalLines, unsigned int nID, MIL_Army& army, MIL_InputArchive& archive, MIL_Formation* pParent = 0 );
             MIL_Formation();
    virtual ~MIL_Formation();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()   
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;

    void WriteODB             ( MT_XXmlOutputArchive& archive ) const;
    void WriteLogisticLinksODB( MT_XXmlOutputArchive& archive ) const;
    //@}

    //! @name Hierarchy
    //@{
    void RegisterAutomate   ( MIL_Automate& automate );
    void RegisterFormation  ( MIL_Formation& formation );
    void UnregisterAutomate ( MIL_Automate& automate );
    void UnregisterFormation( MIL_Formation& formation );
    //@}

    //! @name Accessors
    //@{
    MIL_Army& GetArmy() const;
    uint      GetID  () const;
    //@}

    //! @name Network
    //@{
    void SendCreation () const;
    void SendFullState() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MIL_Formation( const MIL_Formation& );            //!< Copy constructor
    MIL_Formation& operator=( const MIL_Formation& ); //!< Assignement operator
    //@}

    //! @name Tools
    //@{
    void InitializeSubordinates( MIL_EntityManager& manager, MIL_TacticalLineManager& tacticalLines, MIL_InputArchive& archive );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MIL_Formation* >     T_FormationSet;
    typedef T_FormationSet::const_iterator CIT_FormationSet;

    typedef std::set< MIL_Automate* >      T_AutomateSet;
    typedef T_AutomateSet::const_iterator  CIT_AutomateSet;
    //@}

private:
    const uint              nID_;
          MIL_Army*         pArmy_;
          MIL_Formation*    pParent_;
    const PHY_NatureLevel*  pLevel_;
          std::string       strName_;
          T_FormationSet    formations_;
          T_AutomateSet     automates_;
};

#include "MIL_Formation.inl"

#endif // __MIL_Formation_h_
