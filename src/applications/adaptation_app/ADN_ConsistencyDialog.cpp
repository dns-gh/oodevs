// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ConsistencyDialog.h"
#include "moc_ADN_ConsistencyDialog.cpp"
#include "ADN_Workspace.h"
#include "clients_gui/FilterProxyModel.h"
#include "clients_kernel/VariantPointer.h"
#include <boost/assign/list_of.hpp>

namespace
{
    bool IsError( E_ConsistencyCheck type )
    {
        return type == eMissingBreakdown ||
               type == eMissingConvoy ||
               type == eMissingConvoyMission ||
               type == eMissingGeometry ||
               type == eMissingDecisionalModel ||
               type == eMissingUnitOnAutomat ||
               type == eMissingPCOnAutomat ||
               type == eMissingArmor ||
               type == eMissingDisaster ||
               type == eMissionTypeUniqueness ||
               type == eObjectTypeUniqueness ||
               type == eMissingRepairType ||
               type == eInvalidTowCapacity ||
               type == eInvalidMaxMassCarried ||
               type == eInvalidMaxVolumeCarried ||
               type == eInvalidCargoTransportCapacity ||
               type == eInvalidCrowdTransportCapacity ||
               type == eInvalidPtrInVector ||
               type == eBadBreakdownSum ||
               type == eInhabitantSchedule ||
               type == eIndirectFireRange ||
               type == eNoCrew;
    }

    E_ConsistencyCheck Convert( E_ConsistencyCheck type )
    {
        if( type < eUniquenessMask )
            return eUniquenessMask;
        if( type < eMissingMask )
            return eMissingMask;
        return eOthersMask;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyDialog constructor
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
ADN_ConsistencyDialog::ADN_ConsistencyDialog( QWidget* parent )
    : T_Parent( "ADNConsistencyDialog", parent, *new ADN_ConsistencyChecker(), *new gui::FilterProxyModel< E_ConsistencyCheck >( IsError, Convert ) )
{
    // Base size
    setMinimumSize( 600, 500 );

    // Headers
    horizontalHeaders_ << "" << tr( "Description" );

    // Type checkbox
    CreateCheckbox( boost::assign::map_list_of( eMissingMask, tr( "Initialization" ) )
                                              ( eUniquenessMask, tr( "Unicity" ) )
                                              ( eOthersMask, tr( "Others" ) ) );
    QString error = tr( ", because of this, the simulation can't start with this database." );

    // Fill errors text
    errorDescriptions_[ eMissingChoiceComposite ] = tr( "The mission %1 has no type defined for a localisation composite parameter." );
    errorDescriptions_[ eMissingSymbols ]         = tr( "The symbols %1 are defined in symbols.xml but not found in Symbols directory." );
    errorDescriptions_[ eMissionAttachmentInvalid ] = tr( "The file %2 included in the mission %1 is invalid or not present in the attachment list." );
    errorDescriptions_[ eMissingRepairType ]      = tr( "Maintenance system definition is incorrect for equipment '%1'. At least one breakdown type must be defined for each severity." );
    errorDescriptions_[ eInvalidTowCapacity ]     = tr( "Tow capacity must be a positive number for equipment '%1'." );
    errorDescriptions_[ eInvalidMaxMassCarried ]  = tr( "Max weight carried must be a positive number for equipment '%1'." );
    errorDescriptions_[ eInvalidMaxVolumeCarried ] = tr( "Max volume carried must be a positive number for equipment '%1'." );
    errorDescriptions_[ eInvalidCargoTransportCapacity ] = tr( "Cargo transport capacity must be a positive number for equipment '%1'." );
    errorDescriptions_[ eInvalidCrowdTransportCapacity ] = tr( "Crowd transport capacity must be a positive number for equipment '%1'." );

    errorDescriptions_[ eMissionTypeUniqueness ]  = tr( "Duplicate type for missions %1" ) + error;
    errorDescriptions_[ eObjectTypeUniqueness ]   = tr( "Duplicate type for objects %1" ) + error;

    errorDescriptions_[ eMissingPart ]            = tr( "The breakdown %1 has no replacement part" );
    errorDescriptions_[ eMissingBreakdown ]       = tr( "Equipment %1 requires at least one breakdown" ) + error;
    errorDescriptions_[ eMissingConvoy ]          = tr( "Convoy unit type not defined in Log/Supply" ) + error;
    errorDescriptions_[ eMissingConvoyMission ]   = tr( "Convoy mission not defined in Log/Supply" ) + error;
    errorDescriptions_[ eMissingGeometry ]        = tr( "No geometry defined for object %1" ) + error;
    errorDescriptions_[ eMissingDecisionalModel ] = tr( "No decisional model defined for unit %1" ) + error;
    errorDescriptions_[ eMissingUnitOnAutomat ]   = tr( "Automat %1 requires at least one sub unit" ) + error;
    errorDescriptions_[ eMissingPCOnAutomat ]     = tr( "Automat %1 requires at least one PC" ) + error;
    errorDescriptions_[ eMissingArmor ]           = tr( "At least one armor must be defined" ) + error;
    errorDescriptions_[ eMissingDisaster ]        = tr( "No disaster model for object %1" ) + error;
    errorDescriptions_[ eMissingObjectParameter ] = tr( "Mission parameter %1 should have at least one object: All objects will be selectable instead of none." );
    errorDescriptions_[ eNoCrew ]                 = tr( "Unit %1 has no crew in equipment '%2'" ) + error;
    errorDescriptions_[ eInvalidPtrInVector ]     = tr( "Field '%2' is invalid in '%1'" ) + error;
    errorDescriptions_[ eInvalidCrossedRef ]      = tr( "'%1' has one unfound field of type '%2', it will be deleted at next save." );
    errorDescriptions_[ eInvalidIdInVector ]      = tr( "'%1' has the same id. These id will be replaced at next save." );
    errorDescriptions_[ eDuplicateTranslation ]   = tr( "Some translations on tab '%1' have the same key and different translation values. Only one will be used by the simulation." );
    errorDescriptions_[ eInvalidProbe ]           = tr( "Invalid scan depth, should be greater than scan width." );
    errorDescriptions_[ eLogUnitInNonLogAutomat ] = tr( "The non-logistic automat '%1' contains one or more logistic units." );
    errorDescriptions_[ eNonLogUnitInLogAutomat ] = tr( "The logistic automat '%1' contains one or more non-logistic units." );
    errorDescriptions_[ eBadBreakdownSum ]        = tr( "Equipment '%1' - Invalid breakdown data : sum != 100" ) + error;
    errorDescriptions_[ eInhabitantSchedule ]     = tr( "%1" );
    errorDescriptions_[ eInvalidSupplyClass ]     = tr( "Supply class name is empty" );
    errorDescriptions_[ eInvalidResourceNature ]  = tr( "Resource nature name is empty" );
    errorDescriptions_[ eInvalidFireInjuries ]    = tr( "Fire '%1' - Injuries data sum > 100" );
    errorDescriptions_[ eInvalidPoisoningData ]   = tr( "NBC - Agent '%1' - Poisoning effect data sum < 100" );
    errorDescriptions_[ eBadFloodInjuries ]       = tr( "Object '%1' - Flood - Injuries data sum > 100" );
    errorDescriptions_[ eInvalidRoof ]            = tr( "RoofShape name is empty" );
    errorDescriptions_[ eInvalidVolume ]          = tr( "Volume name is empty" );
    errorDescriptions_[ eIndirectFireRange ]      = tr( "Weapon %1 - Indirect fire - max range < min range" ) + error;
    errorDescriptions_[ eRepartitionError ]       = tr( "%1 %2" );
    errorDescriptions_[ eDuplicateEnumeration ]   = tr( "Mission parameter %1 defines duplicate enumeration values." );

    // Connection
    connect( this, SIGNAL( GoToRequested( const ADN_NavigationInfos::GoTo& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnGoToRequested( const ADN_NavigationInfos::GoTo& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyDialog destructor
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
ADN_ConsistencyDialog::~ADN_ConsistencyDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyDialog::OnSelectionChanged
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
void ADN_ConsistencyDialog::OnSelectionChanged( const QModelIndex& index )
{
    // Go select the right item
    const ADN_ConsistencyChecker::T_Items* gotoList = static_cast< const ADN_ConsistencyChecker::T_Items* >( index.data( Qt::UserRole ).value< kernel::VariantPointer >().ptr_ );
    assert( gotoList );
    if( gotoList->size() == 1 )
    {
        emit GoToRequested( *gotoList->front() );
    }
    else
    {
        QMenu* menu = new QMenu( this );
        // Create menu
        for( auto it = gotoList->begin(); it != gotoList->end(); ++it )
            menu->addAction( ( *it )->targetName_ );

        if( QAction* action = menu->exec( QCursor::pos() ) )
            for( ADN_ConsistencyChecker::CIT_Items it = gotoList->begin(); it != gotoList->end(); ++it )
                if( ( *it )->targetName_ == action->menuText() )
                {
                    emit GoToRequested( **it );
                    break;
                }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyDialog::UpdateDataModel
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
void ADN_ConsistencyDialog::UpdateDataModel()
{
    T_Parent::UpdateDataModel();
    DoUpdateDataModel( static_cast< ADN_ConsistencyChecker& >( checker_ ).GetConsistencyErrors() );
    DoUpdateDataModel( ADN_ConsistencyChecker::GetLoadingErrors() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyDialog::DoUpdateDataModel
// Created: JSR 2013-04-12
// -----------------------------------------------------------------------------
void ADN_ConsistencyDialog::DoUpdateDataModel( const ADN_ConsistencyChecker::T_ConsistencyErrors& errors )
{
    for( auto it = errors.begin(); it != errors.end(); ++it )
    {
        const ADN_ConsistencyChecker::ConsistencyError& error = *it;
        QList< QStandardItem* > items;

        QString text = errorDescriptions_[ error.type_ ];
        if( text.contains( "%1" ) )
        {
            QString itemList;
            for( ADN_ConsistencyChecker::CIT_Items it = error.items_.begin(); it != error.items_.end(); ++it )
                itemList += ( ( itemList.isEmpty() ) ? "'" : ( it + 1 == error.items_.end() ) ? tr( " and '" ) : ", '" ) + ( *it )->targetName_ + "'";
            text = ( error.optional_.empty() ) ? text.arg( itemList ) : text.arg( itemList ).arg( error.optional_.c_str() );
        }
        AddIcon( error.items_, error.type_, items );
        AddItem( text, text, error.items_, error.type_, items );

        assert( items.size() == 2 );
        dataModel_->appendRow( items );
    }
}
