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

    typedef std::vector< ConsignResolver_ABC* >::iterator IT_ConsignResolvers;
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
LogisticPlugin::LogisticPlugin( const boost::shared_ptr<const NameResolver_ABC>& nameResolver, const std::string& maintenanceFile,
    const std::string& supplyFile, const std::string& funeralFile, const std::string& medicalFile, const char* localeStr )
    : nameResolver_( nameResolver )
    , localAppli_ ( !qApp ? new QApplication( localAppliArgc, localAppliArgv ) : 0 )
{
    QLocale locale = tools::readLocale();
    if( localeStr != 0)
        locale = QLocale( localeStr );
    if( qApp )
    {
        tools::AddTranslator( *qApp, locale, "ENT" );
        tools::AddTranslator( *qApp, locale, "logistic_plugin" );
    }
    ENT_Tr::InitTranslations();

    resolvers_.push_back( new MaintenanceResolver( maintenanceFile, *nameResolver ));
    resolvers_.push_back( new SupplyResolver( supplyFile, *nameResolver ));
    resolvers_.push_back( new FuneralResolver( funeralFile, *nameResolver ));
    resolvers_.push_back( new MedicalResolver( medicalFile, *nameResolver ));
    for( IT_ConsignResolvers r = resolvers_.begin(); r != resolvers_.end(); ++r )
        (*r)->InitHeader();
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin destructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
LogisticPlugin::~LogisticPlugin()
{
    for( IT_ConsignResolvers r = resolvers_.begin(); r != resolvers_.end(); ++r )
        delete *r;
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin::Receive
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void LogisticPlugin::Receive( const sword::SimToClient& message )
{
    if( message.message().has_control_begin_tick() )
    {
        int currentTick = message.message().control_begin_tick().current_tick();
        std::string simTime = message.message().control_begin_tick().date_time().data();        
        for( IT_ConsignResolvers r = resolvers_.begin(); r != resolvers_.end(); ++r )
            (*r)->SetTime( currentTick, simTime );
    }
    for( IT_ConsignResolvers r = resolvers_.begin(); r != resolvers_.end(); ++r )
        if( (*r)->Receive( message ))
            break;
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
        config.BuildSessionChildFile( xis.attribute( "medicalfile", "LogMedical" )),
        0 );  // localeStr
}

}  // namespace logistic
}  // namespace plugins