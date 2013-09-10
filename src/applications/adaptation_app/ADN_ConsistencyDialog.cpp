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
#include "clients_gui/FilterProxyModel.h"
#include "clients_kernel/VariantPointer.h"
#include <boost/assign/list_of.hpp>

namespace
{
#define CONVERT_TO_MASK( mask ) { if( type & mask ) return mask; }

    E_ConsistencyCheck Convert( E_ConsistencyCheck type )
    {
        CONVERT_TO_MASK( eUniquenessMask )
        CONVERT_TO_MASK( eMissingMask )
        return eOthersMask;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyDialog constructor
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
ADN_ConsistencyDialog::ADN_ConsistencyDialog( QWidget* parent )
    : T_Parent( parent, *new ADN_ConsistencyChecker(), *new gui::FilterProxyModel< E_ConsistencyCheck >( Convert ) )
{
    // Base size
    setMinimumSize( 600, 500 );

    // Headers
    horizontalHeaders_ << "" << tr( "Description" );

    // Type checkbox
    CreateCheckbox( boost::assign::map_list_of( eMissingMask,    tr( "Initialization" ) )
                                              ( eUniquenessMask, tr( "Unicity" ) )
                                              ( eOthersMask,     tr( "Others" ) ) );

    ShowLevelFilter( false );

    // Fill errors text
    errorDescriptions_[ eNNoUniqueness  ] = tr( "Duplicate NNO code for %1." );
    errorDescriptions_[ eEmatUniqueness ] = tr( "Duplicate EMAT8 code for %1." );

    errorDescriptions_[ eMissingNNo     ] = tr( "%1 has no NNO code defined." );
    errorDescriptions_[ eMissingEmat    ] = tr( "%1 has no EMAT8 code defined." );
    errorDescriptions_[ eLowerCaseEmat  ] = tr( "%1 has lower case EMAT8 code." );
    errorDescriptions_[ eMissingRepairType ] = tr( "%1 has no repair type defined in maintenance system in a 'gravity' category." );

    errorDescriptions_[ eMissionTypeUniqueness ]  = tr( "Duplicate type for missions %1." );
    errorDescriptions_[ eMissingPart ]            = tr( "The breakdown '%1' has no replacement part." );
    errorDescriptions_[ eMissingChoiceComposite ] = tr( "The mission '%1' has no type defined for a localisation composite parameter." );
    errorDescriptions_[ eInvalidIdInVector ]      = tr( "'%1' has the same id. These id will be replaced at next save." );
    errorDescriptions_[ eDuplicateSensor ]        = tr( "%1 has duplicate sensors." );

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
    DoUpdateDataModel( static_cast< ADN_ConsistencyChecker& >( checker_ ).GetConsistencyErrors() );
    DoUpdateDataModel( ADN_ConsistencyChecker::GetLoadingErrors() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ConsistencyDialog::DoUpdateDataModel
// Created: JSR 2013-04-12
// -----------------------------------------------------------------------------
void ADN_ConsistencyDialog::DoUpdateDataModel( const ADN_ConsistencyChecker::T_ConsistencyErrors& errors )
{
    for( ADN_ConsistencyChecker::CIT_ConsistencyErrors it = errors.begin(); it != errors.end(); ++it )
    {
        const ADN_ConsistencyChecker::ConsistencyError& error = *it;
        QList< QStandardItem* > items;

        if( ( error.type_ & eMissingMask ) != 0 )
        {
            assert( error.optional_.empty() && error.items_.size() == 1 );
            QString text = errorDescriptions_[ error.type_ ];
            assert( text.contains( "%1" ) );
            if( error.items_.front()->targetTab_ == eComposantes )
                text = text.arg( tr( "The equipement '" ) + error.items_.front()->targetName_ + "'" );
            else if ( error.items_.front()->targetTab_ == eEquipement )
                text = text.arg( tr( "The resource '" ) + error.items_.front()->targetName_ + "'" );
            else
                text = text.arg( error.items_.front()->targetName_ );
            AddIcon( error.items_, error.type_, items, false );
            AddItem( text, text, error.items_, error.type_, items );
        }
        else if( ( error.type_ & eUniquenessMask ) != 0 || ( error.type_ & eOthersMask ) != 0 )
        {
            QString text = errorDescriptions_[ error.type_ ];
            assert( text.contains( "%1" ) );
            QString itemList;
            for( ADN_ConsistencyChecker::CIT_Items it = error.items_.begin(); it != error.items_.end(); ++it )
                itemList += ( ( itemList.isEmpty() ) ? "'" : ( it + 1 == error.items_.end() ) ? tr( " and '" ) : ", '" ) + ( *it )->targetName_ + "'";
            text = text.arg( itemList );
            AddIcon( error.items_, error.type_, items, false );
            AddItem( text, text, error.items_, error.type_, items );
        }
        else
        {
            assert( false );
        }
        assert( !items.empty() );
        dataModel_->appendRow( items );
    }
}
