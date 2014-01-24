// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "NameResolver.h"
#include "dispatcher/Agent_ABC.h"
#include "dispatcher/Automat_ABC.h"
#include "dispatcher/Formation_ABC.h"
#include "dispatcher/Model_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/BreakdownType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/StaticModel.h"
#include "ENT/ENT_Tr.h"

namespace plugins
{
namespace logistic
{

// -----------------------------------------------------------------------------
// Name: NameResolver constructor
// Created: PMD 2012-09-02
// -----------------------------------------------------------------------------
NameResolver::NameResolver( const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel )
    : model_( model ),
      staticModel_( staticModel )
{
}

// -----------------------------------------------------------------------------
// Name: NameResolver destructor
// Created: PMD 2012-09-02
// -----------------------------------------------------------------------------
NameResolver::~NameResolver()
{
}

// -----------------------------------------------------------------------------
// Name: NameResolver constructor
// Created: PMD 2012-09-02
// -----------------------------------------------------------------------------
void NameResolver::GetAgentName( int id, std::string& name ) const
{
    const dispatcher::Agent_ABC* pAgent = model_.Agents().Find( static_cast< unsigned int >( id ) );
    if( pAgent && !pAgent->GetExtension( "NomLong" , name ) )
        name = pAgent->GetName().toStdString();
}

// -----------------------------------------------------------------------------
// Name: NameResolver::GetAutomatName
// Created: PMD 2012-09-02
// -----------------------------------------------------------------------------
void NameResolver::GetAutomatName( int id, std::string& name ) const
{
    const dispatcher::Automat_ABC* pAutomat = model_.Automats().Find( static_cast< unsigned int >( id ) );
    if( pAutomat && !pAutomat->GetExtension( "NomLong" , name ) )
        name = pAutomat->GetName().toStdString();
}

// -----------------------------------------------------------------------------
// Name: NameResolver::GetFormationName
// Created: PMD 2012-09-02
// -----------------------------------------------------------------------------
void NameResolver::GetFormationName( int id, std::string& name ) const
{
    const dispatcher::Formation_ABC* pFormation = model_.Formations().Find( static_cast< unsigned int >( id ) );
    if( pFormation && !pFormation->GetExtension( "NomLong" , name ) )
        name = pFormation->GetName().toStdString();
}

// -----------------------------------------------------------------------------
// Name: NameResolver::GetSupplykName
// Created: PMD 2012-09-02
// -----------------------------------------------------------------------------
void NameResolver::GetSupplykName( const sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus& eSupply, std::string& name ) const
{
    name = ENT_Tr::ConvertFromLogSupplyHandlingStatus( static_cast< E_LogSupplyHandlingStatus >( eSupply ), ENT_Tr::eToTr );
}

// -----------------------------------------------------------------------------
// Name: NameResolver::GetRankName
// Created: PMD 2012-09-02
// -----------------------------------------------------------------------------
void NameResolver::GetRankName( const sword::EnumHumanRank& eRank, std::string& name ) const
{
    name = ENT_Tr::ConvertFromHumanRank( static_cast< E_HumanRank >( eRank ), ENT_Tr::eToTr );
}

// -----------------------------------------------------------------------------
// Name: NameResolver::GetWoundName
// Created: PMD 2012-09-02
// -----------------------------------------------------------------------------
void NameResolver::GetWoundName( const sword::EnumHumanWound& eWound, std::string& name ) const
{
    name = ENT_Tr::ConvertFromHumanWound( static_cast< E_HumanWound >( eWound ), ENT_Tr::eToTr );
}

// -----------------------------------------------------------------------------
// Name: NameResolver::GetMedicalName
// Created: PMD 2012-09-02
// -----------------------------------------------------------------------------
void NameResolver::GetMedicalName( const sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus& eMedical, std::string& name ) const
{
    name = ENT_Tr::ConvertFromLogMedicalHandlingStatus( static_cast< E_LogMedicalHandlingStatus >( eMedical ), ENT_Tr::eToTr );
}

// -----------------------------------------------------------------------------
// Name: NameResolver::GetMaintenanceName
// Created: PMD 2012-09-02
// -----------------------------------------------------------------------------
void NameResolver::GetMaintenanceName( const sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus& eMaintenance, std::string& name ) const
{
    name = ENT_Tr::ConvertFromLogMaintenanceHandlingStatus( eMaintenance );
}

// -----------------------------------------------------------------------------
// Name: NameResolver::GetFuneralName
// Created: PMD 2012-09-02
// -----------------------------------------------------------------------------
void NameResolver::GetFuneralName( const sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus& eFuneral, std::string& name ) const
{
    name = ENT_Tr::ConvertFromLogFuneralHandlingStatus( eFuneral );
}

// -----------------------------------------------------------------------------
// Name: NameResolver::GetEquipmentName
// Created: PMD 2012-09-02
// -----------------------------------------------------------------------------
void NameResolver::GetEquipmentName( const sword::Id& equipmentType, std::string& name ) const
{
    kernel::ComponentType* pEquipment = staticModel_.types_.tools::Resolver< kernel::ComponentType >::Find( equipmentType.id() );
    if( pEquipment )
        name = pEquipment->GetName();
}

// -----------------------------------------------------------------------------
// Name: NameResolver::GetBreakdownName
// Created: PMD 2012-09-02
// -----------------------------------------------------------------------------
void NameResolver::GetBreakdownName( const sword::Id& breakdownType, std::string& name ) const
{
     kernel::BreakdownType* pBreakdown = staticModel_.objectTypes_.kernel::Resolver2< kernel::BreakdownType >::Find( breakdownType.id() );
    if( pBreakdown )
        name = pBreakdown->GetName();
}

// -----------------------------------------------------------------------------
// Name: NameResolver::GetResourceName
// Created: PMD 2012-09-02
// -----------------------------------------------------------------------------
void NameResolver::GetResourceName( const sword::Id& resourceType, std::string& name ) const
{
    kernel::DotationType* pDotation = staticModel_.objectTypes_.kernel::Resolver2< kernel::DotationType >::Find( resourceType.id() );
    if( pDotation )
        name = pDotation->GetName();
}

} //namespace logistic
} //namespace plugins