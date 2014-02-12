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

// =============================================================================
// @class  PHY_ComposanteState
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ComposanteState : private boost::noncopyable
{
public:
    //! @name
    //@{
    static const PHY_ComposanteState undamaged_;
    static const PHY_ComposanteState repairableWithEvacuation_;
    static const PHY_ComposanteState repairableWithoutEvacuation_;
    static const PHY_ComposanteState dead_;
    static const PHY_ComposanteState maintenance_;
    static const PHY_ComposanteState prisoner_;
    //@}

public:
    //! @name Manager
    //@{
    static void                       Initialize  ();
    static void                       Terminate   ();
    static const PHY_ComposanteState& Find        ( unsigned int nID );
    static const PHY_ComposanteState* Find        ( const std::string& strName );
    static unsigned int               GetNbrStates();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName     () const;
          bool         IsUsable    () const;
          bool         IsDamaged   () const;
          bool         CanTransport() const;
          unsigned int GetID       () const;
    //@}

    //! @name Operations
    //@{
    bool operator==( const PHY_ComposanteState& rhs ) const;
    bool operator!=( const PHY_ComposanteState& rhs ) const;
    bool operator< ( const PHY_ComposanteState& rhs ) const;
    //@}

private:
    PHY_ComposanteState( const std::string& strName, unsigned int nState, bool bUsable, bool bDamaged );

private:
    const std::string  strName_;
    const unsigned int nState_;
    const bool         bUsable_;
    const bool         bDamaged_;
};

#endif // __PHY_ComposanteState_h_
