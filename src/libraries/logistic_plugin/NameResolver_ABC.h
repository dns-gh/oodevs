// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef LOGISTIC_PLUGIN_NAMERESOLVER_ABC_H
#define LOGISTIC_PLUGIN_NAMERESOLVER_ABC_H

#include "protocol/Protocol.h"
#include <boost/noncopyable.hpp>
#include <string>

namespace plugins
{
namespace logistic
{

// =============================================================================
/** @class  NameResolver_ABC
    @brief  NameResolver_ABC
*/
// Created: PMD 2012-09-02
// =============================================================================
class NameResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    virtual ~NameResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void GetAgentName( int id, std::string& name ) const = 0;
    virtual void GetAutomatName( int id, std::string& name ) const = 0;
    virtual void GetFormationName( int id, std::string& name ) const = 0;
    virtual void GetSupplykName( const sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus& eSupply, std::string& name ) const = 0;
    virtual void GetRankName( const sword::EnumHumanRank& eRank, std::string& name ) const = 0;
    virtual void GetWoundName( const sword::EnumHumanWound& eWound, std::string& name ) const = 0;
    virtual void GetMedicalName( const sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus& eMedical, std::string& name ) const = 0;
    virtual void GetMaintenanceName( const sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus& eMaintenance, std::string& name ) const = 0;
    virtual void GetFuneralName( const sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus& eFuneral, std::string& name ) const = 0;
    virtual void GetEquipmentName( const sword::Id& equipmentType, std::string& name ) const = 0;
    virtual void GetBreakdownName( const sword::Id& breakdownType, std::string& name ) const = 0;
    virtual void GetResourceName( const sword::Id& resourceType, std::string& name ) const = 0;
    //@}
};

} // namespace logistic
} // namespace plugins

#endif // LOGISTIC_PLUGIN_NAMERESOLVER_ABC_H
