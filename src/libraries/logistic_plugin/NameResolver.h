// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef LOGISTIC_PLUGIN_NAMERESOLVER_H
#define LOGISTIC_PLUGIN_NAMERESOLVER_H

#include "NameResolver_ABC.h"
#include "protocol/Protocol.h"
#include <string>

namespace dispatcher
{
    class Model_ABC;
}

namespace kernel
{
    class StaticModel;
}

namespace plugins
{
namespace logistic
{

// =============================================================================
/** @class  NameResolver
    @brief  NameResolver_ABC
*/
// Created: PMD 2012-09-02
// =============================================================================
class NameResolver : public NameResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
            NameResolver( const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel );
    virtual ~NameResolver();
    //@}

    //! @name Operations
    //@{
    virtual void GetAgentName( int id, std::string& name ) const;
    virtual void GetAutomatName( int id, std::string& name ) const;
    virtual void GetFormationName( int id, std::string& name ) const;
    virtual void GetSupplykName( const sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus& eSupply, std::string& name ) const;
    virtual void GetRankName( const sword::EnumHumanRank& eRank, std::string& name ) const;
    virtual void GetWoundName( const sword::EnumHumanWound& eWound, std::string& name ) const;
    virtual void GetMedicalName( const sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus& eMedical, std::string& name ) const;
    virtual void GetMaintenanceName( const sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus& eMaintenance, std::string& name ) const;
    virtual void GetFuneralName( const sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus& eFuneral, std::string& name ) const;
    virtual void GetEquipmentName( const sword::Id& equipmentType, std::string& name ) const;
    virtual void GetBreakdownName( const sword::Id& breakdownType, std::string& name ) const;
    virtual void GetResourceName( const sword::Id& resourceType, std::string& name ) const;
    //@}

private:
    const dispatcher::Model_ABC& model_;
    const kernel::StaticModel& staticModel_;
};

} // namespace logistic
} // namespace plugins

#endif // LOGISTIC_PLUGIN_NAMERESOLVER_H
