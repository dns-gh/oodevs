// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ModelConsistencyDialog.h"
#include "clients_gui/FilterProxyModel.h"
#include "clients_gui/Tools.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/VariantPointer.h"
#include "preparation/ModelConsistencyChecker.h"
#include "tools/Loader_ABC.h"
#include <boost/assign/list_of.hpp>

namespace
{
    bool IsError( E_ConsistencyCheck type )
    {
        return type == eNoLogisticBase || type ==  eNoCommandPost || type ==  eSeveralCommandPost
            || type ==  eNoKnowledgeGroup || type ==  eScoreError || type ==  eSuccessFactorError;
    }

#define CONVERT_TO_MASK( mask ) { if( type & mask ) return mask; }
    E_ConsistencyCheck Convert( E_ConsistencyCheck type )
    {
        CONVERT_TO_MASK( eUniquenessMask )
        CONVERT_TO_MASK( eLogisticMask )
        CONVERT_TO_MASK( eProfileMask )
        CONVERT_TO_MASK( eGhostMask )
        CONVERT_TO_MASK( eCommandPostMask )
        return eOthersMask;
    }

    std::string DiplayLongName( const std::string& name )
    {
        return name == "" ? "---" : name;
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog constructor
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
ModelConsistencyDialog::ModelConsistencyDialog( QWidget* parent, Model& model, const StaticModel& staticModel, kernel::Controllers& controllers, const tools::RealFileLoaderObserver_ABC& fileLoaderObserver )
    : T_Parent( parent, *new ModelConsistencyChecker( model, staticModel, controllers, fileLoaderObserver ), *new gui::FilterProxyModel< E_ConsistencyCheck >( IsError, Convert ) )
    , actionController_( controllers.actions_ )
{
    // Base size
    setMinimumSize( 900, 500 );

    // Connnect to parent
    connect( this, SIGNAL( ClearLoadingErrors() ), parent, SLOT( ClearLoadingErrors() ) );
    connect( parent, SIGNAL( CheckConsistency() ), this, SLOT( CheckConsistency() ) );

    // Headers
    horizontalHeaders_ << "" << tr( "ID" ) << tr( "Name" ) << tr( "Long name" ) << tr( "Description" );

    // Type checkbox
    CreateCheckbox( boost::assign::map_list_of( eUniquenessMask,  tr( "Unicity" ) )
                                              ( eLogisticMask,    tr( "Logistic" ) )
                                              ( eProfileMask,     tr( "Profile" ) )
                                              ( eGhostMask,       tr( "Ghost" ) )
                                              ( eCommandPostMask, tr( "Command Post" ) )
                                              ( eOthersMask,      tr( "Others" ) ) );

    // Fill errors text
    // Uniqueness
    errorDescriptions_[ eLongNameUniqueness ]              = tr( "Duplicate long name for units %1." );
    errorDescriptions_[ eTeamNameUniqueness ]              = tr( "Duplicate name for parties %1." );
    errorDescriptions_[ eObjectNameUniqueness ]            = tr( "Duplicate name for objects %1." );
    errorDescriptions_[ eLimaNameUniqueness ]              = tr( "Duplicate name for limas %1." );
    errorDescriptions_[ eLimitNameUniqueness ]             = tr( "Duplicate name for limits %1." );

    // Logistic
    errorDescriptions_[ eStockInitialization ]             = tr( "No stocks initialized." );
    errorDescriptions_[ eStockMaxExceeded ]                = tr( "Allocated stocks of nature %1 exceed max capacity." );
    errorDescriptions_[ eStockMissing ]                    = tr( "Some resources used by logistic subordinate(s) are not initialized: %1." );
    errorDescriptions_[ eLogisticInitialization ]          = tr( "No logistic link initialized." );
    errorDescriptions_[ eNoLogisticBase ]                  = tr( "No valid logistic base defined." );
    errorDescriptions_[ eNoLogisticFormation ]             = tr( "Logistic sector not included in a logistic base." );
    errorDescriptions_[ eLogisticBaseNotSameTeam ]         = tr( "Logistic base's party differs from object's party." );
    errorDescriptions_[ eStockInvalidDotation ]            = tr( "Invalid stock resource '%1' in orbat.xml. This resource will not be saved." );

    // Profile
    errorDescriptions_[ eProfileUniqueness ]               = tr( "Association with multiple profiles: %1." );
    errorDescriptions_[ eProfileUnreadable ]               = tr( "Not 'readable' to any user profile. You will not be able to see it on the game." );
    errorDescriptions_[ eProfileUnwritable ]               = tr( "Not 'writable' to any user profile. You will not be able to give orders to it on the game." );

    // Ghost
    errorDescriptions_[ eGhostExistence ]                  = tr( "A phantom unit is present." );
    errorDescriptions_[ eGhostConverted ]                  = tr( "Unknown type '%1', a phantom unit has been created instead." );

    // Command Post
    errorDescriptions_[ eNoCommandPost ]                   = tr( "Automat has no command post." );
    errorDescriptions_[ eSeveralCommandPost ]              = tr( "Automat has more than one command post." );

    // Others
    errorDescriptions_[ eLongNameSize ]                    = tr( "Long name size limit exceeded : %1." );
    errorDescriptions_[ eUnknownObjectTypes ]              = tr( "Unknown object type \"%1\", some objects could not be loaded." );
    errorDescriptions_[ eUnknownCrowdTypes ]               = tr( "Unknown crowd type \"%1\", some crowds could not be loaded." );
    errorDescriptions_[ eUnknownPopulationTypes ]          = tr( "Unknown population type \"%1\", some populations could not be loaded." );
    errorDescriptions_[ eNoKnowledgeGroup ]                = tr( "Automat has no knowledge group." );
    errorDescriptions_[ eScoreError ]                      = tr( "Score definitions contain errors: %1" );
    errorDescriptions_[ eSuccessFactorError ]              = tr( "Success factor definitions contain errors: %1" );
    errorDescriptions_[ eNoOrbat ]                         = tr( "No orbat.xml found. The orbat.xml file will be created after the first save." );
    errorDescriptions_[ eOthers ]                          = "%1";
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog destructor
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
ModelConsistencyDialog::~ModelConsistencyDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::OnSelectionChanged
// Created: ABR 2011-09-26
// -----------------------------------------------------------------------------
void ModelConsistencyDialog::OnSelectionChanged( const QModelIndex& index )
{
    const kernel::SafePointer< kernel::Entity_ABC >* entity = static_cast< const kernel::SafePointer< kernel::Entity_ABC >* >( index.data( Qt::UserRole ).value< kernel::VariantPointer >().ptr_ );
    if( entity && *entity )
    {
        ( *entity )->Select( actionController_ );
        ( *entity )->Activate( actionController_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyDialog::UpdateErrorListView
// Created: ABR 2011-09-23
// -----------------------------------------------------------------------------
void ModelConsistencyDialog::UpdateDataModel()
{
    T_Parent::UpdateDataModel();
    int currentRow = 0;
    const ModelConsistencyChecker::T_ConsistencyErrors& errors = static_cast< ModelConsistencyChecker& >( checker_ ).GetConsistencyErrors();
    for( ModelConsistencyChecker::CIT_ConsistencyErrors it = errors.begin(); it != errors.end(); ++it )
    {
        const ModelConsistencyChecker::ConsistencyError& error = *it;
        QString idList;
        for( ModelConsistencyChecker::CIT_Items entityIt = error.items_.begin(); entityIt != error.items_.end(); ++entityIt )
        {
            if( *entityIt && **entityIt )
                idList += ( ( idList.isEmpty() ) ? "" : ( entityIt + 1 == error.items_.end() ) ? tr( " and " ) : ", " ) + locale().toString( static_cast< unsigned int >( ( **entityIt )->GetId() ) );
        }

        for( ModelConsistencyChecker::CIT_Items entityIt = error.items_.begin(); entityIt != error.items_.end(); ++entityIt, ++currentRow )
        {
            QList< QStandardItem* > items;
            const kernel::SafePointer< kernel::Entity_ABC >& entity = **entityIt;
            if( entity )
            {
                AddIcon( entity, error.type_, items );
                AddItem( static_cast< unsigned int >( entity->GetId() ), locale().toString( static_cast< unsigned int >( entity->GetId() ) ), entity, error.type_, items );
                AddItem( entity->GetName(), entity->GetName(), entity, error.type_, items );
                const std::string longName = DiplayLongName( gui::LongNameHelper::GetEntityLongName( *entity ) );
                AddItem( longName.c_str(), longName.c_str(), entity, error.type_, items );
            }
            else
            {
                AddIcon( entity, error.type_, items );
                AddItem( 0, "---", entity, error.type_, items );
                AddItem( "---", "---", entity, error.type_, items );
                AddItem( "---", "---", entity, error.type_, items );
            }

            QString text = errorDescriptions_[ error.type_ ];
            if( text.contains( "%1" ) )
                text = text.arg( ( error.optional_.empty() ) ? idList : error.optional_.c_str() );
            AddItem( text, text, entity, error.type_, items  );
            dataModel_->appendRow( items );
        }
    }
}
