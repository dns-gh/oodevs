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
#include "clients_kernel/Tools.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "preparation/Model.h"
#include "preparation/ModelConsistencyChecker.h"
#include "tools/Loader_ABC.h"
#include "clients_kernel/VariantPointer.h"
#include <boost/assign/list_of.hpp>

namespace
{
    bool IsError( E_ConsistencyCheck type )
    {
        return type == eNoLogisticBase || type ==  eNoCommandPost || type ==  eSeveralCommandPost
            || type ==  eNoKnowledgeGroup || type ==  eScoreError || type ==  eSuccessFactorError
            || type == eBadLogisticSubordinate 
            || type == eUnknownInfrastructure || type == eUnknownResourceNetwork
            || type == eUnknownMaterial || type == eUnknownRoofShape
            || type == eDeletedUrbanBlocks || type == eDeletedPopulationUrbanBlocks || type == eSignature
            || type == eImpossibleObjectCreation;
    }

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
    : T_Parent( "ModelConsistencyDialog", parent, *new ModelConsistencyChecker( model, staticModel, controllers, fileLoaderObserver ), *new gui::FilterProxyModel< E_ConsistencyCheck >( IsError, Convert ) )
    , model_           ( model )
    , actionController_( controllers.actions_ )
{
    // Base size
    setMinimumSize( 500, 500 );

    // Connnect to parent
    connect( this, SIGNAL( ClearLoadingErrors() ), parent, SLOT( ClearLoadingErrors() ) );
    connect( parent, SIGNAL( CheckConsistency() ), this, SLOT( CheckConsistency() ) );

    // Headers
    horizontalHeaders_ << "" << tools::translate( "ModelConsistencyDialog", "ID" ) << tools::translate( "ModelConsistencyDialog", "Name" ) << tools::translate( "ModelConsistencyDialog", "Description" );

    // Type checkbox
    CreateCheckbox( boost::assign::map_list_of( eUniquenessMask,  tools::translate( "ModelConsistencyDialog", "Unicity" ) )
                                              ( eLogisticMask,    tools::translate( "ModelConsistencyDialog", "Logistic" ) )
                                              ( eProfileMask,     tools::translate( "ModelConsistencyDialog", "Profile" ) )
                                              ( eGhostMask,       tools::translate( "ModelConsistencyDialog", "Ghost" ) )
                                              ( eCommandPostMask, tools::translate( "ModelConsistencyDialog", "Command Post" ) )
                                              ( eOthersMask,      tools::translate( "ModelConsistencyDialog", "Others" ) ) );

    // Fill errors text
    // Uniqueness
    errorDescriptions_[ eLongNameUniqueness ]              = tools::translate( "ModelConsistencyDialog", "Duplicate long name for units %1." );
    errorDescriptions_[ eTeamNameUniqueness ]              = tools::translate( "ModelConsistencyDialog", "Duplicate name for parties %1." );
    errorDescriptions_[ eObjectNameUniqueness ]            = tools::translate( "ModelConsistencyDialog", "Duplicate name for objects %1." );
    errorDescriptions_[ eLimaNameUniqueness ]              = tools::translate( "ModelConsistencyDialog", "Duplicate name for limas %1." );
    errorDescriptions_[ eLimitNameUniqueness ]             = tools::translate( "ModelConsistencyDialog", "Duplicate name for limits %1." );

    // Logistic
    errorDescriptions_[ eStockInitialization ]             = tools::translate( "ModelConsistencyDialog", "No stocks initialized." );
    errorDescriptions_[ eStockMaxExceeded ]                = tools::translate( "ModelConsistencyDialog", "Allocated stocks of nature %1 exceed max capacity." );
    errorDescriptions_[ eStockMissing ]                    = tools::translate( "ModelConsistencyDialog", "Some resources used by logistic subordinate(s) are not initialized: %1." );
    errorDescriptions_[ eLogisticInitialization ]          = tools::translate( "ModelConsistencyDialog", "No logistic link initialized." );
    errorDescriptions_[ eNoLogisticBase ]                  = tools::translate( "ModelConsistencyDialog", "No valid logistic base defined." );
    errorDescriptions_[ eNoLogisticFormation ]             = tools::translate( "ModelConsistencyDialog", "Logistic sector not included in a logistic base." );
    errorDescriptions_[ eLogisticBaseNotSameTeam ]         = tools::translate( "ModelConsistencyDialog", "Logistic base's party differs from object's party." );
    errorDescriptions_[ eStockInvalidDotation ]            = tools::translate( "ModelConsistencyDialog", "Invalid stock resource '%1' in orbat.xml. This resource will not be saved." );
    errorDescriptions_[ eBadLogisticSubordinate ]          = tools::translate( "ModelConsistencyDialog", "Invalid logistic subordinate for '%1' in orbat.xml. The link will be deleted at next save." );
    errorDescriptions_[ eBadQuotas ]                       = tools::translate( "ModelConsistencyDialog", "No quotas defined for '%1' : It will not be resupplied." );

    // Profile
    errorDescriptions_[ eProfileUniqueness ]               = tools::translate( "ModelConsistencyDialog", "Association with multiple profiles: %1." );
    errorDescriptions_[ eProfileUnreadable ]               = tools::translate( "ModelConsistencyDialog", "Not 'readable' to any user profile. You will not be able to see it on the game." );
    errorDescriptions_[ eProfileUnwritable ]               = tools::translate( "ModelConsistencyDialog", "Not 'writable' to any user profile. You will not be able to give orders to it on the game." );

    // Ghost
    errorDescriptions_[ eGhostExistence ]                  = tools::translate( "ModelConsistencyDialog", "A ghost unit is present." );
    errorDescriptions_[ eGhostConverted ]                  = tools::translate( "ModelConsistencyDialog", "Unknown type '%1', a ghost unit has been created instead." );

    // Command Post
    errorDescriptions_[ eNoCommandPost ]                   = tools::translate( "ModelConsistencyDialog", "Automat has no command post." );
    errorDescriptions_[ eSeveralCommandPost ]              = tools::translate( "ModelConsistencyDialog", "Automat has more than one command post." );

    // Others
    errorDescriptions_[ eLongNameSize ]                    = tools::translate( "ModelConsistencyDialog", "Long name size limit exceeded : %1." );
    errorDescriptions_[ eUnknownObjectTypes ]              = tools::translate( "ModelConsistencyDialog", "Unknown object type \"%1\", some objects could not be loaded." );
    errorDescriptions_[ eUnknownCrowdTypes ]               = tools::translate( "ModelConsistencyDialog", "Unknown crowd type \"%1\", some crowds could not be loaded." );
    errorDescriptions_[ eUnknownPopulationTypes ]          = tools::translate( "ModelConsistencyDialog", "Unknown population type \"%1\", some populations could not be loaded." );
    errorDescriptions_[ eNoKnowledgeGroup ]                = tools::translate( "ModelConsistencyDialog", "Automat has no knowledge group." );
    errorDescriptions_[ eScoreError ]                      = tools::translate( "ModelConsistencyDialog", "Score definitions contain errors: %1" );
    errorDescriptions_[ eSuccessFactorError ]              = tools::translate( "ModelConsistencyDialog", "Success factor definitions contain errors: %1" );
    errorDescriptions_[ eUnknownInfrastructure]            = tools::translate( "ModelConsistencyDialog", "Unknown infrastructure \"%1\". It will be deleted at next save." );
    errorDescriptions_[ eUnknownResourceNetwork]           = tools::translate( "ModelConsistencyDialog", "Unknown resource network type \"%1\". Related networks will deleted at next save." );
    errorDescriptions_[ eDeletedUrbanBlocks ]              = tools::translate( "ModelConsistencyDialog", "Some resource network nodes point on nonexistent urban blocks. They will be deleted at next save.");
    errorDescriptions_[ eDeletedPopulationUrbanBlocks ]    = "%1";
    errorDescriptions_[ eNoOrbat ]                         = tools::translate( "ModelConsistencyDialog", "No orbat.xml found" );
    errorDescriptions_[ eSignature ]                       = "%1";
    errorDescriptions_[ eUnitOutsideMap ]                  = tools::translate( "ModelConsistencyDialog", "Entity '%1' is outside the map. It will be repositioned to the origin." );
    errorDescriptions_[ eImpossibleObjectCreation ]        = tools::translate( "ModelConsistencyDialog", "The following object is invalid and will be deleted at next save: %1" );
    errorDescriptions_[ eOthers ]                          = "%1";
    errorDescriptions_[ eEquipmentState ]                  = tools::translate( "ModelConsistencyDialog", "Equipments in maintenance or surrendered state have been reset to on site fixable." );
    errorDescriptions_[ eUnknownMaterial ]                 = tools::translate( "ModelConsistencyDialog", "Unknown material \"%1\". It will be replaced by default material at next save." );
    errorDescriptions_[ eUnknownRoofShape ]                = tools::translate( "ModelConsistencyDialog", "Unknown roof shape \"%1\". It will be replaced by default roof shape at next save." );
    errorDescriptions_[ eUnknownResource ]                 = tools::translate( "ModelConsistencyDialog", "Unknown resource \"%1\". It will deleted at next save." );
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
        std::vector< const kernel::GraphicalEntity_ABC* > elements;
        elements.push_back( *entity );
        ( *entity )->MultipleSelect( actionController_, elements );
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
    bool isValid = true;
    const ModelConsistencyChecker::T_ConsistencyErrors& errors = static_cast< ModelConsistencyChecker& >( checker_ ).GetConsistencyErrors();
    for( ModelConsistencyChecker::CIT_ConsistencyErrors it = errors.begin(); it != errors.end(); ++it )
    {
        const ModelConsistencyChecker::ConsistencyError& error = *it;
        QString idList;
        for( ModelConsistencyChecker::CIT_Items entityIt = error.items_.begin(); entityIt != error.items_.end(); ++entityIt )
        {
            if( *entityIt && **entityIt )
                idList += ( ( idList.isEmpty() ) ? "" : ( entityIt + 1 == error.items_.end() ) ? tools::translate( "ModelConsistencyDialog", " and " ) : ", " ) + locale().toString( static_cast< unsigned int >( ( **entityIt )->GetId() ) );
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
            }
            else
            {
                AddIcon( entity, error.type_, items );
                AddItem( 0, "---", entity, error.type_, items );
                AddItem( "---", "---", entity, error.type_, items );
            }

            QString text = errorDescriptions_[ error.type_ ];
            if( text.contains( "%1" ) )
                text = text.arg( ( error.optional_.empty() ) ? idList : error.optional_.c_str() );
            AddItem( text, text, entity, error.type_, items  );
            dataModel_->appendRow( items );
        }

        if( isValid && IsError( it->type_ ) )
            isValid = false;
    }
    model_.SetExerciseValidity( isValid );
}
