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
#define CONVERT_TO_MASK( mask ) { if( type < mask ) return mask; }
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
ModelConsistencyDialog::ModelConsistencyDialog( QWidget* parent, Model& model, const StaticModel& staticModel, kernel::Controllers& controllers, tools::RealFileLoaderObserver_ABC& fileLoaderObserver )
    : T_Parent( parent, *new ModelConsistencyChecker( model, staticModel, controllers, fileLoaderObserver ), *new gui::FilterProxyModel< E_ConsistencyCheck >( Convert ) )
    , actionController_( controllers.actions_ )
    , emptyEntity_( controllers )
{
    // Base size
    setMinimumSize( 900, 500 );

    // Connnect to parent
    connect( this, SIGNAL( ClearLoadingErrors() ), parent, SLOT( ClearLoadingErrors() ) );
    connect( parent, SIGNAL( CheckConsistency() ), this, SLOT( CheckConsistency() ) );

    // Headers
    horizontalHeaders_ << "" << tools::translate( "ModelConsistencyDialog", "ID" ) << tools::translate( "ModelConsistencyDialog", "Name" ) << tools::translate( "ModelConsistencyDialog", "Long name" ) << tools::translate( "ModelConsistencyDialog", "Description" ); // $$$$ LGY 2012-06-18: 

    // Type checkbox
    CreateCheckbox( boost::assign::map_list_of( eUniquenessMask,  tools::translate( "ModelConsistencyDialog", "Unicity" ) )
                                              ( eLogisticMask,    tools::translate( "ModelConsistencyDialog", "Logistic" ) )
                                              ( eProfileMask,     tools::translate( "ModelConsistencyDialog", "Profile" ) )
                                              ( eGhostMask,       tools::translate( "ModelConsistencyDialog", "Ghost" ) )
                                              ( eCommandPostMask, tools::translate( "ModelConsistencyDialog", "Command Post" ) )
                                              ( eOthersMask,      tools::translate( "ModelConsistencyDialog", "Others" ) ) );

    // Fill errors text
    // Uniqueness
    errorDescriptions_[ eLongNameUniqueness ] = tools::translate( "ModelConsistencyDialog", "Duplicate long name for units %1." );
    errorDescriptions_[ eTeamNameUniqueness ] = tools::translate( "ModelConsistencyDialog", "Duplicate name for parties %1." );
    errorDescriptions_[ eObjectNameUniqueness ] = tools::translate( "ModelConsistencyDialog", "Duplicate name for objects %1." );
    errorDescriptions_[ eLimaNameUniqueness ] = tools::translate( "ModelConsistencyDialog", "Duplicate name for limas %1." );
    errorDescriptions_[ eLimitNameUniqueness ] = tools::translate( "ModelConsistencyDialog", "Duplicate name for limits %1." );
    errorDescriptions_[ eUniquenessMask ] = "%1";

    // Logistic
    errorDescriptions_[ eStockInitialization ] = tools::translate( "ModelConsistencyDialog", "No stocks initialized." );
    errorDescriptions_[ eStockMaxExceeded ] = tools::translate( "ModelConsistencyDialog", "Allocated stocks of nature %1 exceed max capacity." );
    errorDescriptions_[ eStockMissing ] = tools::translate( "ModelConsistencyDialog", "Some resources used by logistic subordinate(s) are not initialized: %1." );
    errorDescriptions_[ eLogisticInitialization ] = tools::translate( "ModelConsistencyDialog", "No logistic link initialized." );
    errorDescriptions_[ eNoLogisticBase ] = tools::translate( "ModelConsistencyDialog", "No valid logistic base defined." );
    errorDescriptions_[ eNoLogisticFormation ] = tools::translate( "ModelConsistencyDialog", "Logistic sector not included in a logistic base." );
    errorDescriptions_[ eLogisticBaseNotSameTeam ] = tools::translate( "ModelConsistencyDialog", "Logistic base's party differs from object's party." );
    errorDescriptions_[ eStockInvalidDotation ] = tools::translate( "ModelConsistencyDialog", "Invalid stock resource '%1' in orbat.xml. This resource will not be saved." );
    errorDescriptions_[ eBadLogisticSubordinate ] = tools::translate( "ModelConsistencyDialog", "Invalid logistic subordinate for '%1' in orbat.xml. The link will deleted at next save." );
    errorDescriptions_[ eLogisticMask ] = "%1";

    // Profile
    errorDescriptions_[ eProfileUniqueness ] = tools::translate( "ModelConsistencyDialog", "Association with multiple profiles: %1." );
    errorDescriptions_[ eProfileUnreadable ] = tools::translate( "ModelConsistencyDialog", "Not 'readable' to any user profile. You will not be able to see it on the game." );
    errorDescriptions_[ eProfileUnwritable ] = tools::translate( "ModelConsistencyDialog", "Not 'writable' to any user profile. You will not be able to give orders to it on the game." );
    errorDescriptions_[ eProfileNumberTooHigh ] = tools::translate( "ModelConsistencyDialog", "The profile \"%1\" contains more than 12 automats and/or crowds." );
    errorDescriptions_[ eProfileNoRole ] = tools::translate( "ModelConsistencyDialog", "No user role defined for profile \"%1\"." );
    errorDescriptions_[ eProfileMask ] = "%1";

    // Ghost
    errorDescriptions_[ eGhostExistence ] = tools::translate( "ModelConsistencyDialog", "A ghost unit is present." );
    errorDescriptions_[ eGhostConverted ] = tools::translate( "ModelConsistencyDialog", "Unknown type '%1', a ghost unit has been created instead." );
    errorDescriptions_[ eGhostMask ] = "%1";

    // Command Post
    errorDescriptions_[ eNoCommandPost ] = tools::translate( "ModelConsistencyDialog", "Automat has no command post." );
    errorDescriptions_[ eSeveralCommandPost ] = tools::translate( "ModelConsistencyDialog", "Automat has more than one command post." );
    errorDescriptions_[ eCommandPostMask ] = "%1";

    // Others
    errorDescriptions_[ eLongNameSize ] = tools::translate( "ModelConsistencyDialog", "Long name size limit exceeded : %1." );
    errorDescriptions_[ eUnknownObjectTypes ] = tools::translate( "ModelConsistencyDialog", "Unknown object type \"%1\", some objects could not be loaded." );
    errorDescriptions_[ eUnknownCrowdTypes ] = tools::translate( "ModelConsistencyDialog", "Unknown crowd type \"%1\", some crowds could not be loaded." );
    errorDescriptions_[ eUnknownPopulationTypes ] = tools::translate( "ModelConsistencyDialog", "Unknown population type \"%1\", some populations could not be loaded." );
    errorDescriptions_[ eNoKnowledgeGroup ] = tools::translate( "ModelConsistencyDialog", "Automat has no knowledge group." );
    errorDescriptions_[ eScoreError ] = tools::translate( "ModelConsistencyDialog", "Score definitions contain errors: %1" );
    errorDescriptions_[ eSuccessFactorError ] = tools::translate( "ModelConsistencyDialog", "Success factor definitions contain errors: %1" );
    errorDescriptions_[ eUnknownInfrastructure] = tools::translate( "ModelConsistencyDialog", "Unknown infrastructure \"%1\". It will be deleted at next save." );
    errorDescriptions_[ eUnknownResourceNetwork] = tools::translate( "ModelConsistencyDialog", "Unknown resource network type \"%1\". Related networks will deleted at next save." );
    errorDescriptions_[ eNoOrbat ] = tools::translate( "ModelConsistencyDialog", "No orbat.xml found. The orbat.xml file will be created after the first save." );
    errorDescriptions_[ eBadCPExtensions ] = tools::translate( "ModelConsistencyDialog", "CP type or SIOC type extensions defined for a non CP unit." );
    errorDescriptions_[ eDiffusionList ] = tools::translate( "ModelConsistencyDialog", "Cycle detected in diffusion list." );
    errorDescriptions_[ eDeletedUrbanBlocks ] = tools::translate( "ModelConsistencyDialog", "Some resource network nodes point on nonexistent urban blocks. They will be deleted at next save.");
    errorDescriptions_[ eMelmil ] = tools::translate( "ModelConsistencyDialog", "Melmil file is missing.");
    errorDescriptions_[ eDeletedPopulationUrbanBlocks ] = "%1";
    errorDescriptions_[ eDiffusionListCleaned ] = tools::translate( "ModelConsistencyDialog", "Invalid unit removed from diffusion list." );
    errorDescriptions_[ eSignature ] = "%1";
    errorDescriptions_[ eUnitOutsideMap ] = tools::translate( "ModelConsistencyDialog", "Entity '%1' is outside the map. It will be repositioned to the origin." );
    errorDescriptions_[ eCityAreaLimitExceeded ] = tools::translate( "ModelConsistencyDialog", "City area exceeds %1 km²" );
    errorDescriptions_[ eImpossibleObjectCreation ] = tools::translate( "ModelConsistencyDialog", "The following object is invalid and will be deleted at next save: %1" );
    errorDescriptions_[ eInhabitantBlockRemoved ] = tools::translate( "ModelConsistencyDialog", "Some urban blocks used by population %1 were removed from terrain, population will be modified at next save." );
    errorDescriptions_[ eOthers ] = "%1";
    errorDescriptions_[ eOthersMask ] = "%1";
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
    for( auto it = errors.begin(); it != errors.end(); ++it )
    {
        const ModelConsistencyChecker::ConsistencyError& error = *it;
        QString idList;
        for( auto entityIt = error.items_.begin(); entityIt != error.items_.end(); ++entityIt )
        {
            if( *entityIt )
                idList += ( ( idList.isEmpty() ) ? "" : ( entityIt + 1 == error.items_.end() ) ? tools::translate( "ModelConsistencyDialog", " and " ) : ", " ) + locale().toString( static_cast< unsigned int >( ( *entityIt )->GetId() ) );
        }

        for( auto entityIt = error.items_.begin(); entityIt != error.items_.end(); ++entityIt, ++currentRow )
        {
            QList< QStandardItem* > items;
            if( *entityIt )
            {
                const kernel::SafePointer< kernel::Entity_ABC >& entity = *entityIt;
                AddIcon( entity, error.type_, items, error.IsError() );
                AddItem( static_cast< unsigned int >( entity->GetId() ), locale().toString( static_cast< unsigned int >( entity->GetId() ) ), entity, error.type_, items );
                AddItem( entity->GetName(), entity->GetName(), entity, error.type_, items );
                const std::string longName = DiplayLongName( gui::LongNameHelper::GetEntityLongName( *entity ) );
                AddItem( longName.c_str(), longName.c_str(), entity, error.type_, items );
            }
            else
            {
                AddIcon( emptyEntity_, error.type_, items, error.IsError() );
                AddItem( 0, "---", emptyEntity_, error.type_, items );
                AddItem( "---", "---", emptyEntity_, error.type_, items );
                AddItem( "---", "---", emptyEntity_, error.type_, items );
            }
            QString text = errorDescriptions_[ error.type_ ];
            if( text.contains( "%1" ) )
                text = text.arg( ( error.optional_.empty() ) ? idList : error.optional_.c_str() );
            AddItem( text, text, *entityIt ? *entityIt : emptyEntity_, error.type_, items  );
            dataModel_->appendRow( items );
        }
    }
}
