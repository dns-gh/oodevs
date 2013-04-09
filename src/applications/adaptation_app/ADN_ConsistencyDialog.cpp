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
               type == eInvalidCrossedRef ||
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
    errorDescriptions_[ eNNoUniqueness  ]         = tr( "Duplicate NNO code for %1." );
    errorDescriptions_[ eEmatUniqueness ]         = tr( "Duplicate EMAT8 code for %1." );

    errorDescriptions_[ eMissingNNo     ]         = tr( "%1 has no NNO code defined." );
    errorDescriptions_[ eMissingEmat    ]         = tr( "%1 has no EMAT8 code defined." );
    errorDescriptions_[ eMissingChoiceComposite ] = tr( "The mission %1 has no type defined for a localisation composite parameter." );
    errorDescriptions_[ eMissingSymbols ]         = tr( "The symbols %1 are defined in symbols.xml but not found in Symbols directory." );
    errorDescriptions_[ eMissionAttachmentInvalid ] = tr( "The file %2 included in the mission %1 is invalid or not present in the attachment list." );
    errorDescriptions_[ eLowerCaseEmat  ]         = tr( "%1 has lower case EMAT8 code." );
    errorDescriptions_[ eMissingRepairType ]      = tr( "Equipment %1 has no repair type defined in maintenance system in a 'gravity' category." );

    errorDescriptions_[ eMissionTypeUniqueness ]  = tr( "Duplicate type for missions %1" ) + error;
    errorDescriptions_[ eObjectTypeUniqueness ]   = tr( "Duplicate type for objects %1" ) + error;

    errorDescriptions_[ eMissingPart ]            = tr( "The breakdown %1 has no replacement part" ) + error;
    errorDescriptions_[ eMissingBreakdown ]       = tr( "Equipment %1 requires at least one breakdown" ) + error;
    errorDescriptions_[ eMissingConvoy ]          = tr( "Convoy unit type not defined in Log/Supply" ) + error;
    errorDescriptions_[ eMissingConvoyMission ]   = tr( "Convoy mission not defined in Log/Supply" ) + error;
    errorDescriptions_[ eMissingGeometry ]        = tr( "No geometry defined for object %1" ) + error;
    errorDescriptions_[ eMissingDecisionalModel ] = tr( "No decisional model defined for unit %1" ) + error;
    errorDescriptions_[ eMissingUnitOnAutomat ]   = tr( "Automat %1 requires at least one sub unit" ) + error;
    errorDescriptions_[ eMissingPCOnAutomat ]     = tr( "Automat %1 requires at least one PC" ) + error;
    errorDescriptions_[ eMissingArmor ]           = tr( "At least one armor must be defined" ) + error;
    errorDescriptions_[ eMissingDisaster ]        = tr( "No disaster model for object %1" ) + error;
    errorDescriptions_[ eNoCrew ]                 = tr( "Unit %1 has no crew in equipment '%2'" ) + error;
    errorDescriptions_[ eInvalidCrossedRef ]      = tr( "Field '%2' is invalid in '%1'" ) + error;

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
        for( ADN_ConsistencyChecker::CIT_Items it = gotoList->begin(); it != gotoList->end(); ++it )
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

    const ADN_ConsistencyChecker::T_ConsistencyErrors& errors = static_cast< ADN_ConsistencyChecker& >( checker_ ).GetConsistencyErrors();
    for( ADN_ConsistencyChecker::CIT_ConsistencyErrors it = errors.begin(); it != errors.end(); ++it )
    {
        const ADN_ConsistencyChecker::ConsistencyError& error = *it;
        QList< QStandardItem* > items;

        QString text = errorDescriptions_[ error.type_ ];
        if( text.contains( "%1" ) )
        {
            if( error.type_ == eMissingNNo || error.type_ == eMissingEmat ) // exception
            {
                if( error.items_.front()->targetTab_ == eEquipments )
                    text = text.arg( tr( "The equipement '" ) + error.items_.front()->targetName_ + "'" );
                else if ( error.items_.front()->targetTab_ == eResources )
                    text = text.arg( tr( "The resource '" ) + error.items_.front()->targetName_ + "'" );
                else
                {
                    assert( false );
                }
            }
            else // standard
            {
                QString itemList;
                for( ADN_ConsistencyChecker::CIT_Items it = error.items_.begin(); it != error.items_.end(); ++it )
                    itemList += ( ( itemList.isEmpty() ) ? "'" : ( it + 1 == error.items_.end() ) ? tr( " and '" ) : ", '" ) + ( *it )->targetName_ + "'";
                text = ( error.optional_.empty() ) ? text.arg( itemList ) : text.arg( itemList ).arg( error.optional_.c_str() );
            }
        }
        AddIcon( error.items_, error.type_, items );
        AddItem( text, text, error.items_, error.type_, items );

        assert( items.size() == 2 );
        dataModel_->appendRow( items );
    }
}
