// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Categories/PHY_Protection.h $
// $Author: Nld $
// $Modtime: 11/03/05 14:04 $
// $Revision: 5 $
// $Workfile: PHY_Protection.h $
//
// *****************************************************************************

#ifndef __PHY_Protection_h_
#define __PHY_Protection_h_

#include "MIL.h"

#include "MT_Tools/MT_GaussianRandom.h"
#include "MT_Tools/MT_Random.h"

class PHY_ComposanteState;

// =============================================================================
// @class  PHY_Protection
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Protection
{
    MT_COPYNOTALLOWED( PHY_Protection )

public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_Protection*, sCaseInsensitiveLess > T_ProtectionMap;
    typedef T_ProtectionMap::const_iterator                                      CIT_ProtectionMap;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize( MIL_InputArchive& archive );
    static void Terminate ();

    static const T_ProtectionMap& GetProtections();
    static const PHY_Protection*  FindProtection( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName              () const;
          uint         GetID                () const;
          uint         GetNeutralizationTime() const;
          MT_Float     GetHumanWoundFactor  ( const PHY_ComposanteState& composanteState ) const;
          bool         IsHuman              () const;
    //@}

    //! @name Operations
    //@{
    bool CanRandomlyBreaksDownEva () const;
    bool CanRandomlyBreaksDownNeva() const;
    //@}

private:
     PHY_Protection( const std::string& strName, MIL_InputArchive& archive );
    ~PHY_Protection();

private:
    //! @name Types
    //@{
    typedef std::vector< MT_Float > T_HumanWoundFactorVector;

    enum E_Type
    {
        eHuman,
        eMaterial
    };
    //@}

private:
    const std::string               strName_;
          uint                      nID_;
          E_Type                    nType_;
          MT_Float                  rBreakdownProbabilityEva_;
          MT_Float                  rBreakdownProbabilityNeva_;
          T_HumanWoundFactorVector  humanWoundFactors_;          

    mutable MT_GaussianRandom neutralizationTime_;

private:
    static T_ProtectionMap protections_;
    static uint            nNextID_;
    static MT_Random       random_;
};

#include "PHY_Protection.inl"

#endif // __PHY_Protection_h_
