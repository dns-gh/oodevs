// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Tiredness.h $
// $Author: Age $
// $Modtime: 1/12/04 15:39 $
// $Revision: 1 $
// $Workfile: Tiredness.h $
//
// *****************************************************************************

#ifndef __Tiredness_h_
#define __Tiredness_h_

#include "clients_kernel/Types.h"
#include "ASN_Types.h"
#include "MT_Tools/MT_Tools_Types.h"

namespace xml { class xistream; };

// =============================================================================
// @class  Tiredness
// Created: JVT 2004-08-03
// =============================================================================
class Tiredness
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const Tiredness*, sCaseInsensitiveLess > T_TirednessMap;
    typedef T_TirednessMap::const_iterator                                CIT_TirednessMap;
    //@}

public:
    //! @name 
    //@{
    static const Tiredness normal_;
    static const Tiredness fatigue_;
    static const Tiredness epuise_;
    //@}

public:
    //! @name Manager
    //@{
    static       void            Initialize    ( xml::xistream& xis );
    static       void            Terminate     ();
    static const Tiredness*  Find          ( const std::string& strName );
    static const Tiredness*  Find          ( ASN1T_EnumUnitFatigue nAsnID );
    static const T_TirednessMap& GetTirednesses();
    //@}

    //! @name Accessors
    //@{
          int                   GetID                           () const;
    const std::string&          GetName                         () const;
          ASN1T_EnumUnitFatigue GetAsnID                        () const;
          float                 GetCoefMaxSpeedModificator      () const;
          float                 GetCoefReloadingTimeModificator () const;
          float                 GetCoefPhModificator            () const;
          float                 GetCoefPostureTimeModificator   () const;
          float                 GetCoefSensorDistanceModificator() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const Tiredness& rhs ) const;
    bool operator!=( const Tiredness& rhs ) const;
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
     Tiredness( const std::string& strName, E_TirednessType nType, ASN1T_EnumUnitFatigue nAsnID );
    ~Tiredness();
    Tiredness( const Tiredness& );
    Tiredness& operator=( const Tiredness& );

    //! @name Init
    //@{
    void Read( xml::xistream& xis );
    //@}

private:
    const std::string           strName_;
    const E_TirednessType       nType_;
    const ASN1T_EnumUnitFatigue nAsnID_;

    float rCoefMaxSpeedModificator_;
    float rCoefReloadingTimeModificator_;
    float rCoefPhModificator_;
    float rCoefPostureTimeModificator_;
    float rCoefSensorDistanceModificator_;

private:
    static T_TirednessMap tirednesses_;
};

#include "Tiredness.inl"

#endif // __Tiredness_h_
