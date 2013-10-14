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
#include "tools/Language.h"
#include "tools/SessionConfig.h"
#include "tools/XmlStreamOperators.h"
#include <tools/Path.h>
#pragma warning( push, 0 )
#include "QtCore/qTranslator.h"
#include <QtGui/qapplication.h>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace bpt = boost::posix_time;
namespace bg = boost::gregorian;

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
LogisticPlugin::LogisticPlugin( const boost::shared_ptr<const NameResolver_ABC>& nameResolver, const tools::Path& maintenanceFile,
                                const tools::Path& supplyFile, const tools::Path& funeralFile, const tools::Path& medicalFile )
    : nameResolver_( nameResolver )
    , localAppli_ ( !qApp ? new QApplication( localAppliArgc, localAppliArgv ) : 0 )
{
    if( qApp )
    {
        tools::AddTranslator( *qApp, tools::Language::Current(), "ENT" );
        tools::AddTranslator( *qApp, tools::Language::Current(), "logistic_plugin" );
    }
    ENT_Tr::InitTranslations();

    resolvers_.resize( eNbrLogisticType );
    resolvers_[ eLogisticType_Maintenance ] = new MaintenanceResolver( maintenanceFile, *nameResolver );
    resolvers_[ eLogisticType_Supply ]      = new SupplyResolver( supplyFile, *nameResolver );
    resolvers_[ eLogisticType_Funeral ]     = new FuneralResolver( funeralFile, *nameResolver );
    resolvers_[ eLogisticType_Medical ]     = new MedicalResolver( medicalFile, *nameResolver );
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
    Receive( message, bpt::second_clock::local_time().date() );
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin::Receive
// Created: MMC 2012-09-11
// -----------------------------------------------------------------------------
void LogisticPlugin::Receive( const sword::SimToClient& message, const bg::date& today )
{
    if( message.message().has_control_begin_tick() )
    {
        int currentTick = message.message().control_begin_tick().current_tick();
        std::string simTime = message.message().control_begin_tick().date_time().data();        
        for( IT_ConsignResolvers r = resolvers_.begin(); r != resolvers_.end(); ++r )
            (*r)->SetTime( currentTick, simTime );
    }
    else
    {
        for( IT_ConsignResolvers r = resolvers_.begin(); r != resolvers_.end(); ++r )
            if( (*r)->Receive( message, today ) )
                break;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin::GetConsignCount
// Created: MMC 2012-09-11
// -----------------------------------------------------------------------------
int LogisticPlugin::GetConsignCount( E_LogisticType eLogisticType ) const
{
    size_t resolverIndex = static_cast< size_t >( eLogisticType );
    if( resolverIndex < resolvers_.size() )
    {
        const ConsignResolver_ABC* pResolver = resolvers_[ resolverIndex ];
        if( pResolver )
            return pResolver->GetConsignCount();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin::SetMaxLinesInFile
// Created: MMC 2012-09-11
// -----------------------------------------------------------------------------
void LogisticPlugin::SetMaxLinesInFile( int maxLines )
{
    for( IT_ConsignResolvers r = resolvers_.begin(); r != resolvers_.end(); ++r )
        (*r)->SetMaxLinesInFile( maxLines );
}


LogisticPlugin* CreateLogisticPlugin(
    const boost::shared_ptr<const NameResolver_ABC>& nameResolver,
    const tools::SessionConfig& config, xml::xistream& xis )
{
    return new LogisticPlugin(
        nameResolver,
        config.BuildSessionChildFile( xis.attribute< tools::Path >( "maintenancefile", "LogMaintenance" ) ),
        config.BuildSessionChildFile( xis.attribute< tools::Path >( "supplyfile", "LogSupply" ) ),
        config.BuildSessionChildFile( xis.attribute< tools::Path >( "funeralfile", "LogFuneral" ) ),
        config.BuildSessionChildFile( xis.attribute< tools::Path >( "medicalfile", "LogMedical" ) ) );
}

}  // namespace logistic
}  // namespace plugins