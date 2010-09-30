// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/PHY_MaintenanceWorkRate.h $
// $Author: Jvt $
// $Modtime: 12/04/05 14:20 $
// $Revision: 4 $
// $Workfile: PHY_MaintenanceWorkRate.h $
//
// *****************************************************************************

#ifndef __PHY_MaintenanceWorkRate_h_
#define __PHY_MaintenanceWorkRate_h_

#include "MT_Tools/MT_Tools_Types.h"
#include "MT_Tools/MT_Stl.h"

namespace Common
{
    enum EnumLogMaintenanceRegimeTravail;
}

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  PHY_MaintenanceWorkRate
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MaintenanceWorkRate : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    static PHY_MaintenanceWorkRate r1_;
    static PHY_MaintenanceWorkRate r2_;
    static PHY_MaintenanceWorkRate r3_;
    static PHY_MaintenanceWorkRate r4_;
    //@}

public:
    //! @name Manager
    //@{
    static       void                     Initialize( xml::xistream& xis );
    static       void                     Terminate ();
    static const PHY_MaintenanceWorkRate* Find      ( Common::EnumLogMaintenanceRegimeTravail nID );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    Common::EnumLogMaintenanceRegimeTravail GetAsnID() const;
    unsigned int GetNbrWorkerAllowedToWork( unsigned int nNbrAvailable ) const;
    unsigned int GetDelayBeforeWarningRC() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_MaintenanceWorkRate*, sCaseInsensitiveLess > T_WorkRateMap;
    typedef T_WorkRateMap::const_iterator                                                 CIT_WorkRateMap;
    //@}

private:
     PHY_MaintenanceWorkRate( const std::string& strName, Common::EnumLogMaintenanceRegimeTravail asn );
    ~PHY_MaintenanceWorkRate();

    //! @name Init
    //@{
    struct LoadingWrapper;
    static void ReadWorkRate( xml::xistream& xis );
    //@}

private:
    const std::string                           strName_;
    const Common::EnumLogMaintenanceRegimeTravail asn_;
          MT_Float                              rWorkerRatio_;
          MT_Float                              rWorkTime_;
          unsigned int                          nDelayBeforeWarningRC_;

private:
    static T_WorkRateMap workRates_;
};

#endif // __PHY_MaintenanceWorkRate_h_
