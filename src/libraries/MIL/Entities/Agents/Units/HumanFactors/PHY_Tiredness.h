// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/HumanFactors/PHY_Tiredness.h $
// $Author: Jvt $
// $Modtime: 14/04/05 16:30 $
// $Revision: 4 $
// $Workfile: PHY_Tiredness.h $
//
// *****************************************************************************

#ifndef __PHY_Tiredness_h_
#define __PHY_Tiredness_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

class PHY_ConsumptionType;

// =============================================================================
// @class  PHY_Tiredness
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Tiredness
{
    MT_COPYNOTALLOWED( PHY_Tiredness )

public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_Tiredness*, sCaseInsensitiveLess > T_TirednessMap;
    typedef T_TirednessMap::const_iterator                                      CIT_TirednessMap;
    //@}

public:
    //! @name 
    //@{
    static const PHY_Tiredness normal_;
    static const PHY_Tiredness fatigue_;
    static const PHY_Tiredness epuise_;
    //@}

public:
    //! @name Manager
    //@{
    static       void           Initialize( MIL_InputArchive& archive );
    static       void           Terminate ();
    static const PHY_Tiredness* Find      ( const std::string& strName );
    static const PHY_Tiredness* Find      ( ASN1T_EnumUnitFatigue nAsnID );
    static const PHY_Tiredness* Find      ( uint nID );
    //@}

    //! @name Accessors
    //@{
    const std::string&          GetName                         () const;
          uint                  GetID                           () const;
          ASN1T_EnumUnitFatigue GetAsnID                        () const;
          MT_Float              GetWeight                       () const;
          MT_Float              GetCoefMaxSpeedModificator      () const;
          MT_Float              GetCoefReloadingTimeModificator () const;
          MT_Float              GetCoefPhModificator            () const;
          MT_Float              GetCoefPostureTimeModificator   () const;
          MT_Float              GetCoefSensorDistanceModificator() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const PHY_Tiredness& rhs ) const;
    bool operator!=( const PHY_Tiredness& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_TirednessType
    {
        eNormal  = 0,
        eFatigue = 1,
        eEpuise  = 2
    };
    //@}

private:
     PHY_Tiredness( const std::string& strName, E_TirednessType nType, ASN1T_EnumUnitFatigue nAsnID, MT_Float rWeight );
    ~PHY_Tiredness();

    //! @name Init
    //@{
    void Read( MIL_InputArchive& archive );
    //@}

private:
    const std::string           strName_;
    const E_TirednessType       nType_;
    const ASN1T_EnumUnitFatigue nAsnID_;
    const MT_Float              rDIAWeight_;

    MT_Float rCoefMaxSpeedModificator_;
    MT_Float rCoefReloadingTimeModificator_;
    MT_Float rCoefPhModificator_;
    MT_Float rCoefPostureTimeModificator_;
    MT_Float rCoefSensorDistanceModificator_;

private:
    static T_TirednessMap tirednesses_;
};

#include "PHY_Tiredness.inl"

#endif // __PHY_Tiredness_h_
