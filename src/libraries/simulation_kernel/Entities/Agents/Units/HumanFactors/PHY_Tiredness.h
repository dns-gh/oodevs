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

#include "MT_Tools/MT_String.h"

namespace sword
{
    enum UnitAttributes_EnumUnitTiredness;
}

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  PHY_Tiredness
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Tiredness : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_Tiredness* > T_TirednessMap;
    typedef T_TirednessMap::const_iterator              CIT_TirednessMap;
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
    static       void           Initialize( xml::xistream& xis );
    static       void           Terminate ();
    static const PHY_Tiredness* Find      ( const std::string& strName );
    static const PHY_Tiredness* Find      ( sword::UnitAttributes_EnumUnitTiredness nAsnID );
    static const PHY_Tiredness* Find      ( unsigned int nID );
    //@}

    //! @name Accessors
    //@{
    const std::string&          GetName                       () const;
          unsigned int                  GetID                 () const;
          sword::UnitAttributes_EnumUnitTiredness GetAsnID    () const;
          double              GetWeight                       () const;
          double              GetCoefMaxSpeedModificator      () const;
          double              GetCoefReloadingTimeModificator () const;
          double              GetCoefPhModificator            () const;
          double              GetCoefPostureTimeModificator   () const;
          double              GetCoefSensorDistanceModificator() const;
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
     PHY_Tiredness( const std::string& strName, E_TirednessType nType, sword::UnitAttributes_EnumUnitTiredness nAsnID, double rWeight );
    ~PHY_Tiredness();

    //! @name Init
    //@{
    void Read( xml::xistream& xis );
    //@}

private:
    const std::string           strName_;
    const E_TirednessType       nType_;
    const sword::UnitAttributes_EnumUnitTiredness nAsnID_;
    const double              rDIAWeight_;

    double rCoefMaxSpeedModificator_;
    double rCoefReloadingTimeModificator_;
    double rCoefPhModificator_;
    double rCoefPostureTimeModificator_;
    double rCoefSensorDistanceModificator_;

private:
    //! @name Statics
    //@{
    static T_TirednessMap tirednesses_;
    struct LoadingWrapper;
    static void ReadTiredness( xml::xistream& xis );
    //@}
};

#endif // __PHY_Tiredness_h_
