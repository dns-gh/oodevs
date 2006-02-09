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

#include "Types.h"
#include "ASN_Types.h"

// =============================================================================
// @class  Tiredness
// Created: JVT 2004-08-03
// =============================================================================
class Tiredness
{
    MT_COPYNOTALLOWED( Tiredness )

public:
    //! @name Types
    //@{
    typedef std::map< std::string, const Tiredness*, sCaseInsensitiveLess > T_TirednessMap;
    typedef T_TirednessMap::const_iterator                                      CIT_TirednessMap;
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
    static       void            Initialize    ( InputArchive& archive );
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
          MT_Float              GetCoefMaxSpeedModificator      () const;
          MT_Float              GetCoefReloadingTimeModificator () const;
          MT_Float              GetCoefPhModificator            () const;
          MT_Float              GetCoefPostureTimeModificator   () const;
          MT_Float              GetCoefSensorDistanceModificator() const;
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

    //! @name Init
    //@{
    void Read( InputArchive& archive );
    //@}

private:
    const std::string           strName_;
    const E_TirednessType       nType_;
    const ASN1T_EnumUnitFatigue nAsnID_;

    MT_Float rCoefMaxSpeedModificator_;
    MT_Float rCoefReloadingTimeModificator_;
    MT_Float rCoefPhModificator_;
    MT_Float rCoefPostureTimeModificator_;
    MT_Float rCoefSensorDistanceModificator_;

private:
    static T_TirednessMap tirednesses_;
};

#include "Tiredness.inl"

#endif // __Tiredness_h_
