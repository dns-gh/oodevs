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
#include "dispatcher/Model_ABC.h"
#include "ENT/ENT_Tr_Gen.h"
#include "protocol/Protocol.h"
#include "protocol/ServerPublisher_ABC.h"
#include "tools/SessionConfig.h"
#pragma warning( push, 0 )
#include <QtCore/qsettings.h>
#include <QtCore/qtextcodec.h>
#include "QtCore/QTranslator.h"
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>

using namespace plugins::logistic;

namespace
{
    QString ReadLang()
    {
        QSettings settings( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() );
    }

    void InitTranslator( const QString& name, const QString& lang )
    {
        QTranslator* translator = new QTranslator( qApp );
        QString filename( name );
        filename += "_";
        filename += lang;
        if( translator->load( filename, "." ) || translator->load( filename, "resources/locales" ) )
            qApp->installTranslator( translator ); 
        else
            delete translator;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
LogisticPlugin::LogisticPlugin( const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, const tools::SessionConfig& config, xml::xistream& xis )
    : sessionConfig_( config )
    , staticModel_ ( staticModel )
    , currentTick_( 0 )
    , maintenanceResolver_  ( new MaintenanceResolver( config.BuildSessionChildFile( xis.attribute( "maintenancefile", "LogMaintenance" ) ), model, staticModel ) )
    , supplyResolver_       ( new SupplyResolver( config.BuildSessionChildFile( xis.attribute( "supplyfile", "LogSupply" ) ), model, staticModel ) )
    , funeralResolver_      ( new FuneralResolver( config.BuildSessionChildFile( xis.attribute( "funeralfile", "LogFuneral" ) ), model, staticModel ) )
    , medicalResolver_      ( new MedicalResolver( config.BuildSessionChildFile( xis.attribute( "medicalfile", "LogMedical" ) ), model, staticModel ) )
{
    QString lang = ReadLang();
    InitTranslator( "ENT", lang );
    InitTranslator( "logistic_plugin", lang );
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
