// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Tiredness.h $
// $Author: Nld $
// $Modtime: 30/11/04 18:50 $
// $Revision: 1 $
// $Workfile: MOS_Tiredness.h $
//
// *****************************************************************************

#ifndef __MOS_Tiredness_h_
#define __MOS_Tiredness_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

// =============================================================================
// @class  MOS_Tiredness
// Created: JVT 2004-08-03
// =============================================================================
class MOS_Tiredness
{
    MT_COPYNOTALLOWED( MOS_Tiredness )

public:
    //! @name Types
    //@{
    typedef std::map< std::string, const MOS_Tiredness*, sCaseInsensitiveLess > T_TirednessMap;
    typedef T_TirednessMap::const_iterator                                      CIT_TirednessMap;
    //@}

public:
    //! @name 
    //@{
    static const MOS_Tiredness normal_;
    static const MOS_Tiredness fatigue_;
    static const MOS_Tiredness epuise_;
    //@}

public:
    //! @name Manager
    //@{
    static       void            Initialize    ( MT_InputArchive_ABC& archive );
    static       void            Terminate     ();
    static const MOS_Tiredness*  Find          ( const std::string& strName );
    static const MOS_Tiredness*  Find          ( ASN1T_EnumUnitFatigue nAsnID );
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
    bool operator==( const MOS_Tiredness& rhs ) const;
    bool operator!=( const MOS_Tiredness& rhs ) const;
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
     MOS_Tiredness( const std::string& strName, E_TirednessType nType, ASN1T_EnumUnitFatigue nAsnID );
    ~MOS_Tiredness();

    //! @name Init
    //@{
    void Read( MT_InputArchive_ABC& archive );
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

#include "MOS_Tiredness.inl"

#endif // __MOS_Tiredness_h_
