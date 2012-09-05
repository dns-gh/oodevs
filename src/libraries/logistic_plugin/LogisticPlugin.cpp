// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "LogisticPlugin.h"
#include "ConsignData_ABC.h"
#include "FuneralResolver.h"
#include "MaintenanceResolver.h"
#include "MedicalResolver.h"
#include "SupplyResolver.h"
#include "clients_kernel/Tools.h"
#include "ENT/ENT_Tr_Gen.h"
#include "protocol/Protocol.h"
#include "protocol/ServerPublisher_ABC.h"
#include "tools/SessionConfig.h"
#pragma warning( push, 0 )
#include <QtCore/qsettings.h>
#include <QtCore/qtextcodec.h>
#include "QtCore/qTranslator.h"
#include "QtCore/qLocale.h"
#include <QtGui/qapplication.h>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>

namespace plugins 
{
namespace logistic
{

namespace
{
    int localAppliArgc( 1 );
    char* localAppliArgv[] = { " " };
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
LogisticPlugin::LogisticPlugin( const boost::shared_ptr<const NameResolver_ABC>& nameResolver, const std::string& maintenanceFile,
    const std::string& supplyFile, const std::string& funeralFile, const std::string& medicalFile )
    : currentTick_( 0 )
    , nameResolver_( nameResolver )
    , maintenanceResolver_  ( new MaintenanceResolver( maintenanceFile, *nameResolver ) )
    , supplyResolver_       ( new SupplyResolver( supplyFile, *nameResolver ) )
    , funeralResolver_      ( new FuneralResolver( funeralFile, *nameResolver ) )
    , medicalResolver_      ( new MedicalResolver( medicalFile, *nameResolver ) )
    , localAppli_ ( !qApp ? new QApplication( localAppliArgc, localAppliArgv ) : 0 )
{
    std::string lang = tools::readLang();
    if( qApp )
    {
        tools::AddTranslator( *qApp, tools::readLocale(), "ENT" );
        tools::AddTranslator( *qApp, tools::readLocale(), "logistic_plugin" );
    }
    ENT_Tr::InitTranslations();
    maintenanceResolver_->InitHeader();
    supplyResolver_->InitHeader();
    funeralResolver_->InitHeader();
    medicalResolver_->InitHeader();
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin destructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
LogisticPlugin::~LogisticPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin::Receive
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void LogisticPlugin::Receive( const sword::SimToClient& message )
{
    if( message.message().has_control_begin_tick() )
    {
        currentTick_= message.message().control_begin_tick().current_tick();
        simTime_    = message.message().control_begin_tick().date_time().data();        
        maintenanceResolver_->SetTime( currentTick_, simTime_ );
        supplyResolver_->SetTime( currentTick_, simTime_ );
        funeralResolver_->SetTime( currentTick_, simTime_ );
        medicalResolver_->SetTime( currentTick_, simTime_ );
    }
    maintenanceResolver_->Receive( message );
    supplyResolver_->Receive( message );
    funeralResolver_->Receive( message );
    medicalResolver_->Receive( message );
}


LogisticPlugin* CreateLogisticPlugin(
    const boost::shared_ptr<const NameResolver_ABC>& nameResolver,
    const tools::SessionConfig& config, xml::xistream& xis )
{
    return new LogisticPlugin(
        nameResolver,
        config.BuildSessionChildFile( xis.attribute( "maintenancefile", "LogMaintenance" )),
        config.BuildSessionChildFile( xis.attribute( "supplyfile", "LogSupply" )),
        config.BuildSessionChildFile( xis.attribute( "funeralfile", "LogFuneral" )),
        config.BuildSessionChildFile( xis.attribute( "medicalfile", "LogMedical" )));
}

}  // namespace logistic
}  // namespace plugins