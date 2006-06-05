// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/PHY_BreakdownType.h $
// $Author: Jvt $
// $Modtime: 14/04/05 17:14 $
// $Revision: 3 $
// $Workfile: PHY_BreakdownType.h $
//
// *****************************************************************************

#ifndef __PHY_BreakdownType_h_
#define __PHY_BreakdownType_h_

#include "MIL.h"

#include "MT_Tools/MT_GaussianRandom.h"

class PHY_MaintenanceLevel;
class PHY_DotationCategory;

// =============================================================================
// @class  PHY_BreakdownType
// Created: JVT 2004-08-03
// =============================================================================
class PHY_BreakdownType
{
    MT_COPYNOTALLOWED( PHY_BreakdownType )

public:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, uint > T_PartMap;
    typedef T_PartMap::const_iterator                     CIT_PartMap;
    //@}

public:
    //! @name Manager
    //@{
    static void                     Initialize       ( MIL_InputArchive& archive );
    static void                     Terminate        ();
    static uint                     GetDiagnosticTime();
    static const PHY_BreakdownType* Find             ( const std::string& strName );
    static const PHY_BreakdownType* Find             ( uint nID );
    //@}

    //! @name Accessors
    //@{
    const std::string&          GetName             () const;
          uint                  GetID               () const;
          bool                  AffectMobility      () const;
          bool                  AffectElectronic    () const;
    const PHY_MaintenanceLevel& GetMaintenanceLevel () const;
    const T_PartMap&            GetParts            () const;
          uint                  GetTheoricRepairTime() const;
          uint                  ChooseARepairTime   () const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
        eMobility,
        eElectronic,
        eUnknown
    };

    typedef std::map< std::string, const PHY_BreakdownType*, sCaseInsensitiveLess > T_BreakdownMap;
    typedef T_BreakdownMap::const_iterator                                          CIT_BreakdownMap;
    //@}

private:
     PHY_BreakdownType( const std::string& strName, const PHY_MaintenanceLevel& maintenanceLevel, E_Type nType, MIL_InputArchive& archive );
    ~PHY_BreakdownType();

    //! @name Tools
    //@{
    static E_Type ConvertType( const std::string& strType );
    //@}

private:
    const std::string           strName_;
    const PHY_MaintenanceLevel& maintenanceLevel_;
    const E_Type                nType_;
          uint                  nID_;
          T_PartMap             parts_;
          uint                  nTheoricRepairTime_;

    mutable MT_GaussianRandom repairTime_;

private:
    static T_BreakdownMap breakdowns_;
    static uint           nDiagnosticTime_;
};

#include "PHY_BreakdownType.inl"

#endif // __PHY_BreakdownType_h_
