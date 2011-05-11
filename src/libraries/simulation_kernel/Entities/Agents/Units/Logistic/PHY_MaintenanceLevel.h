// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/PHY_MaintenanceLevel.h $
// $Author: Jvt $
// $Modtime: 26/04/05 15:05 $
// $Revision: 3 $
// $Workfile: PHY_MaintenanceLevel.h $
//
// *****************************************************************************

#ifndef __PHY_MaintenanceLevel_h_
#define __PHY_MaintenanceLevel_h_

#include "MT_Tools/MT_String.h"

// =============================================================================
// @class  PHY_MaintenanceLevel
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MaintenanceLevel : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_MaintenanceLevel* > T_MaintenanceLevelMap;
    typedef T_MaintenanceLevelMap::const_iterator              CIT_MaintenanceLevelMap;

    static PHY_MaintenanceLevel nti1_;
    static PHY_MaintenanceLevel nti2_;
    static PHY_MaintenanceLevel nti3_;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const T_MaintenanceLevelMap& GetMaintenanceLevels();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    //@}

    //! @name Operators
    //@{
    bool operator == ( const PHY_MaintenanceLevel& rhs ) const;
    bool operator != ( const PHY_MaintenanceLevel& rhs ) const;
    //@}

private:
    explicit PHY_MaintenanceLevel( const std::string& strName );
            ~PHY_MaintenanceLevel();

private:
    const std::string strName_;

private:
    static T_MaintenanceLevelMap maintenanceLevels_;
};

#endif // __PHY_MaintenanceLevel_h_
