// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LogisticBase.h"
#include "PropertiesDictionary.h"
#include "LogisticHierarchiesBase.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "ENT/ENT_Tr.h"
#include "protocol/Protocol.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LogisticBase::LogisticBase
// Created: ABR 2014-01-17
// -----------------------------------------------------------------------------
LogisticBase::LogisticBase( kernel::Controllers& controllers,
                            const kernel::Entity_ABC& entity,
                            gui::PropertiesDictionary& dictionary,
                            bool active,
                            bool isBase )
    : controller_( controllers.controller_ )
    , isBase_( isBase )
    , entity_( entity )
    , isMaintenanceManual_( false )
{
    CreateDictionary( dictionary, active, ( controllers.GetCurrentMode() & eModes_AllGaming ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: LogisticBase constructor
// Created: ABR 2014-01-17
// -----------------------------------------------------------------------------
LogisticBase::LogisticBase( kernel::Controllers& controllers,
                            const kernel::Entity_ABC& entity,
                            gui::PropertiesDictionary& dictionary,
                            bool active,
                            xml::xistream& xis )
    : controller_( controllers.controller_ )
    , isBase_( xis.has_attribute( "logistic-level" ) &&
               xis.attribute( "logistic-level", "" ) == ENT_Tr::ConvertFromLogisticLevel( sword::logistic_base, ENT_Tr::eToSim ) )
    , entity_( entity )
    , isMaintenanceManual_( false )
{
    CreateDictionary( dictionary, active, ( controllers.GetCurrentMode() & eModes_AllGaming ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: LogisticBase::~LogisticBase
// Created: ABR 2014-01-17
// -----------------------------------------------------------------------------
LogisticBase::~LogisticBase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticBase::CreateDictionary
// Created: ABR 2014-01-20
// -----------------------------------------------------------------------------
void LogisticBase::CreateDictionary( gui::PropertiesDictionary& dictionary,
                                     bool active,
                                     bool readOnly )
{
    if( active )
        dictionary.Register( entity_, tools::translate( "LogisticBase", "Info/Logistic base" ), isBase_, *this, &LogisticBase::SetBase, readOnly );
}

// -----------------------------------------------------------------------------
// Name: LogisticBase::IsBase
// Created: ABR 2014-01-17
// -----------------------------------------------------------------------------
bool LogisticBase::IsBase() const
{
    return isBase_;
}

// -----------------------------------------------------------------------------
// Name: LogisticBase::SetLevel
// Created: ABR 2014-01-17
// -----------------------------------------------------------------------------
void LogisticBase::SetBase( const bool& isBase )
{
    if( !isBase )
        if( const LogisticHierarchiesBase* logHierarchy = entity_.Retrieve< LogisticHierarchiesBase >() )
        {
            tools::Iterator< const kernel::Entity_ABC& > children = logHierarchy->CreateSubordinateIterator();
            if( children.HasMoreElements() )
            {
                int result = QMessageBox::question( 0,
                                                    tools::translate( "LogisticBase", "SWORD" ),
                                                    tools::translate( "LogisticBase", "By disabling the logistic function on this formation, all the logistic subordinates superiors will be reset. Do you want to proceed ?" ),
                                                    QMessageBox::Yes, QMessageBox::Cancel );
                if( result == QMessageBox::Cancel )
                    return;
                while( children.HasMoreElements() )
                {
                    const kernel::Entity_ABC& entity = children.NextElement();
                    if( LogisticHierarchiesBase* logEntityHierarchy = const_cast< kernel::Entity_ABC& >( entity ).Retrieve< LogisticHierarchiesBase >() )
                        logEntityHierarchy->SetLogisticSuperior( kernel::LogisticBaseSuperior() );
                }
            }
        }
    isBase_ = isBase;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticBase::GetEntity
// Created: ABR 2014-01-20
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& LogisticBase::GetEntity() const
{
    return entity_;
}

// -----------------------------------------------------------------------------
// Name: LogisticBase::SerializeAttributes
// Created: ABR 2014-01-17
// -----------------------------------------------------------------------------
void LogisticBase::SerializeAttributes( xml::xostream& xos ) const
{
    if( isBase_ )
        xos << xml::attribute( "logistic-level", ENT_Tr::ConvertFromLogisticLevel( sword::logistic_base, ENT_Tr::eToSim ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticBase::IsMaintenanceManual
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
bool LogisticBase::IsMaintenanceManual() const
{
    return isMaintenanceManual_;
}

// -----------------------------------------------------------------------------
// Name: LogisticBase::SetMaintenanceManual
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
void LogisticBase::SetMaintenanceManual( bool manual )
{
    if( manual == isMaintenanceManual_ )
        return;
    isMaintenanceManual_ = manual;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticBase::DoUpdate
// Created: ABR 2014-01-23
// -----------------------------------------------------------------------------
void LogisticBase::DoUpdate( const sword::AutomatCreation& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticBase::DoUpdate
// Created: ABR 2014-01-23
// -----------------------------------------------------------------------------
void LogisticBase::DoUpdate( const sword::AutomatAttributes& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticBase::DoUpdate
// Created: ABR 2014-01-23
// -----------------------------------------------------------------------------
void LogisticBase::DoUpdate( const sword::FormationCreation& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticBase::DoUpdate
// Created: ABR 2014-01-23
// -----------------------------------------------------------------------------
void LogisticBase::DoUpdate( const sword::FormationUpdate& message )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticBase::Update
// Created: ABR 2014-01-23
// -----------------------------------------------------------------------------
template< typename T >
void LogisticBase::Update( const T& message )
{
    if( message.has_log_maintenance_manual() )
        SetMaintenanceManual( message.log_maintenance_manual() );
}
