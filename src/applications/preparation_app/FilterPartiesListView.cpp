// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FilterPartiesListView.h"
#include "moc_FilterPartiesListView.cpp"
#include "clients_gui/resources.h"
#include "clients_kernel/Tools.h"

namespace
{
    enum E_Roles
    {
        PartyIdRole = Qt::UserRole,
        CheckedRole = Qt::UserRole + 1
    };
}

// -----------------------------------------------------------------------------
// Name: FilterPartiesListView constructor
// Created: ABR 2012-05-29
// -----------------------------------------------------------------------------
FilterPartiesListView::FilterPartiesListView( QWidget* parent, bool checkedByDefault /* = true */ )
    : QTreeView( parent )
    , checkedPixmap_   ( MAKE_PIXMAP( check ) )
    , uncheckedPixmap_ ( MAKE_PIXMAP( cross ) )
    , checkedByDefault_( checkedByDefault )
{
    setModel( &model_ );
    setEditTriggers( 0 );
    setRootIsDecorated( false );
    header()->hide();
    connect( this, SIGNAL( clicked( const QModelIndex& ) ), SLOT( OnItemClicked( const QModelIndex& ) ) );
}

// -----------------------------------------------------------------------------
// Name: FilterPartiesListView destructor
// Created: ABR 2012-05-29
// -----------------------------------------------------------------------------
FilterPartiesListView::~FilterPartiesListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterPartiesListView::IsPartyChecked
// Created: ABR 2012-05-29
// -----------------------------------------------------------------------------
bool FilterPartiesListView::IsPartyChecked( unsigned long partyID )
{
    for( int i = 0; i < model_.rowCount(); ++i )
        if( model_.item( i )->data( PartyIdRole ).toUInt() == partyID )
            return model_.item( i )->data( CheckedRole ).toBool();
    assert( false );
    return false;
}

// -----------------------------------------------------------------------------
// Name: FilterPartiesListView::OnItemClicked
// Created: ABR 2012-05-29
// -----------------------------------------------------------------------------
void FilterPartiesListView::OnItemClicked( const QModelIndex& index )
{
    if( !index.isValid() )
        return;
    QStandardItem* item = model_.itemFromIndex( index );
    if( !item )
        return;
    bool checked = item->data( CheckedRole ).toBool();
    checked = !checked;
    item->setData( checked, CheckedRole );
    item->setIcon( ( checked ) ? checkedPixmap_ : uncheckedPixmap_  );
    emit( ValueChanged() );
}

// -----------------------------------------------------------------------------
// Name: FilterPartiesListView::ParseOrbatFile
// Created: ABR 2012-05-29
// -----------------------------------------------------------------------------
bool FilterPartiesListView::ParseOrbatFile( const tools::Path& fileName )
{
    model_.clear();
    bool status = true;
    try
    {
        tools::Xifstream xis( fileName );
        xis >> xml::start( "orbat" )
                >> xml::start( "parties" )
                    >> xml::list( "party", *this, &FilterPartiesListView::ReadTeam );
    }
    catch( ... )
    {
        status = false;
    }
    setEnabled( status );
    return status;
}

// -----------------------------------------------------------------------------
// Name: FilterPartiesListView::ReadTeam
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
void FilterPartiesListView::ReadTeam( xml::xistream& xis )
{
    const std::string name = xis.attribute< std::string >( "name", "" );
    unsigned int id = xis.attribute< unsigned int >( "id", 0 );
    assert( !name.empty() && id != 0 );
    QStandardItem* item = new QStandardItem( ( checkedByDefault_ ) ? checkedPixmap_ : uncheckedPixmap_, name.c_str() );
    item->setData( id, PartyIdRole );
    item->setData( checkedByDefault_, CheckedRole );
    model_.appendRow( item );
}

// -----------------------------------------------------------------------------
// Name: FilterPartiesListView::GetTeamList
// Created: ABR 2012-05-29
// -----------------------------------------------------------------------------
QString FilterPartiesListView::GetTeamList()
{
    QString result = "";
    for( int i = 0; i < model_.rowCount(); ++i )
    {
        QStandardItem* item = model_.item( i );
        if( item->data( CheckedRole ).toBool() )
        {
            const QString idStr = QString::number( item->data( PartyIdRole ).toUInt() );
            result += ( result.isEmpty() ) ? idStr : QString( ";%1" ).arg( idStr );
        }
    }
    return result;
}
