// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Composantes/PHY_ComposanteState.h $
// $Author: Nld $
// $Modtime: 11/03/05 11:04 $
// $Revision: 5 $
// $Workfile: PHY_ComposanteState.h $
//
// *****************************************************************************

#ifndef __PHY_ComposanteState_h_
#define __PHY_ComposanteState_h_

#include "MIL.h"

// =============================================================================
// @class  PHY_ComposanteState
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ComposanteState
{
    MT_COPYNOTALLOWED( PHY_ComposanteState )

public:
    //! @name 
    //@{
    static const PHY_ComposanteState undamaged_;
    static const PHY_ComposanteState repairableWithEvacuation_;
    static const PHY_ComposanteState repairableWithoutEvacuation_;
    static const PHY_ComposanteState dead_;
    static const PHY_ComposanteState maintenance_;
    static const PHY_ComposanteState prisoner_;

    typedef std::vector< const PHY_ComposanteState* > T_ComposanteStateVector;
    typedef T_ComposanteStateVector::const_iterator   CIT_ComposanteStateVector;
    //@}

public:
    //! @name Manager
    //@{
    static void                       Initialize  ();
    static void                       Terminate   ();
    static const PHY_ComposanteState& Find        ( uint nID );
    static const PHY_ComposanteState* Find        ( const std::string& strName );
    static uint                       GetNbrStates();
    static T_ComposanteStateVector&   GetStates   ();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName     () const;
          bool         IsUsable    () const;
          bool         IsDamaged   () const;
          bool         CanTransport() const;
          uint         GetID       () const;
    //@}

    //! @name Operations
    //@{
    bool operator==( const PHY_ComposanteState& rhs ) const;
    bool operator!=( const PHY_ComposanteState& rhs ) const;
    bool operator< ( const PHY_ComposanteState& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_State
    {
        eDead,
        eRepairableWithEvacuation,
        eRepairableWithoutEvacuation,
        eUndamaged,
        eMaintenance,
        ePrisoner,
        eNbrStates
    };
    //@}

private:
     PHY_ComposanteState( const std::string& strName, E_State nState, bool bUsable, bool bDamaged );
    ~PHY_ComposanteState();

private:
    const std::string strName_;
    const E_State     nState_;
    const bool        bUsable_;
    const bool        bDamaged_;

private:
    static T_ComposanteStateVector composanteStates_;
};

#include "PHY_ComposanteState.inl"

#endif // __PHY_ComposanteState_h_
