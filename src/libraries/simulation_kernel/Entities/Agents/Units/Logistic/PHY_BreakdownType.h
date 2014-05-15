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

#include "MT_Tools/MT_GaussianRandom.h"
#include <tools/Map.h>

namespace xml
{
    class xistream;
}

class PHY_MaintenanceLevel;
class PHY_DotationCategory;

// =============================================================================
// @class  PHY_BreakdownType
// Created: JVT 2004-08-03
// =============================================================================
class PHY_BreakdownType : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef tools::Map< const PHY_DotationCategory*, unsigned int > T_PartMap;
    //@}

public:
    //! @name Manager
    //@{
    static void                     Initialize       ( xml::xistream& xis );
    static void                     Terminate        ();
    static unsigned int             GetDiagnosticTime();
    static bool                     GetRepairDurationInManHours();
    static const PHY_BreakdownType* Find             ( const std::string& strName );
    static const PHY_BreakdownType* Find             ( unsigned int nID );
    //@}

    //! @name Accessors
    //@{
    const std::string&          GetName             () const;
          unsigned int          GetID               () const;
          bool                  AffectMobility      () const;
          bool                  AffectElectronic    () const;
    const PHY_MaintenanceLevel& GetMaintenanceLevel () const;
    const T_PartMap&            GetParts            () const;
          unsigned int          GetTheoricRepairTime() const;
          unsigned int          ChooseARepairTime   () const;
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
    //@}

    typedef std::map< std::string, const PHY_BreakdownType* > T_BreakdownMap;
    typedef T_BreakdownMap::const_iterator                  CIT_BreakdownMap;
    //@}
    //! @name Helpers
    //@{
    static void ReadCategory ( xml::xistream& xis );
    static void ReadBreakdown( xml::xistream& xis, const PHY_MaintenanceLevel& maintenanceLevel );
           void ReadPart     ( xml::xistream& xis );
    //@}
private:
     PHY_BreakdownType( const std::string& strName, const PHY_MaintenanceLevel& maintenanceLevel, E_Type nType, xml::xistream& xis );
    ~PHY_BreakdownType();

    //! @name Tools
    //@{
    static E_Type ConvertType( const std::string& strType );
    //@}

private:
    const std::string           strName_;
    const PHY_MaintenanceLevel& maintenanceLevel_;
    const E_Type                nType_;
          unsigned int          nID_;
          T_PartMap             parts_;
          unsigned int          nTheoricRepairTime_;

    mutable MT_GaussianRandom repairTime_;

private:
    static T_BreakdownMap breakdowns_;
    static unsigned int   nDiagnosticTime_;
    static bool           repairDurationInManHours_;
};

#endif // __PHY_BreakdownType_h_
