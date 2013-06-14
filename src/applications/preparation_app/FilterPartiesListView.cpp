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

// -----------------------------------------------------------------------------
// Name: FilterPartiesListView constructor
// Created: ABR 2012-05-29
// -----------------------------------------------------------------------------
FilterPartiesListView::FilterPartiesListView( QWidget* parent, bool checkedByDefault /* = true */ )
    : Q3ListView( parent )
    , checkedPixmap_   ( MAKE_PIXMAP( check ) )
    , uncheckedPixmap_ ( MAKE_PIXMAP( cross ) )
    , checkedByDefault_( checkedByDefault )
{
    addColumn( "Checkbox" );
    addColumn( "Hidden checkbox", 0 );
    addColumn( "Hidden party id", 0 );
    addColumn( "Party" );
    header()->setResizeEnabled( false );
    header()->setMovingEnabled( false );
    hideColumn( eHiddenPartyID );
    hideColumn( eHiddenCheckbox );
    header()->hide();

    setResizeMode( Q3ListView::LastColumn );

    connect( this, SIGNAL( clicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnItemClicked( Q3ListViewItem*, const QPoint&, int ) ) );
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
    for( Q3ListViewItemIterator it( this ); it.current(); ++it )
        if( ( *it )->text( eHiddenPartyID ).toUInt() == partyID )
            return ( *it )->text( eHiddenCheckbox ).toInt() > 0;
    assert( false );
    return false;
}

// -----------------------------------------------------------------------------
// Name: FilterPartiesListView::OnItemClicked
// Created: ABR 2012-05-29
// -----------------------------------------------------------------------------
void FilterPartiesListView::OnItemClicked( Q3ListViewItem* item, const QPoint& /* point */, int column )
{
    if( column != eCheckbox || !item )
        return;

    bool checked = item->text( eHiddenCheckbox ).toInt() > 0;
    checked = !checked;
    item->setText( eHiddenCheckbox, QString::number( ( checked ) ? 1 : 0 ) );
    item->setPixmap( eCheckbox, ( checked ) ? checkedPixmap_ : uncheckedPixmap_  );
    emit( ValueChanged() );
}

// -----------------------------------------------------------------------------
// Name: FilterPartiesListView::ParseOrbatFile
// Created: ABR 2012-05-29
// -----------------------------------------------------------------------------
bool FilterPartiesListView::ParseOrbatFile( const std::string& fileName )
{
    clear();
    bool status = true;
    try
    {
        xml::xifstream xis( fileName );
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
    unsigned long id = xis.attribute< unsigned long >( "id", 0 );
    assert( !name.empty() && id != 0 );
    Q3ListViewItem* item = new Q3ListViewItem( this );
    item->setText( ePartyName, name.c_str() );
    item->setText( eHiddenPartyID, QString::number( id ) );
    item->setText( eHiddenCheckbox, QString::number( ( checkedByDefault_ ) ? 1 : 0 ) );
    item->setPixmap( eCheckbox, ( checkedByDefault_ ) ? checkedPixmap_ : uncheckedPixmap_ );
}

// -----------------------------------------------------------------------------
// Name: FilterPartiesListView::GetTeamList
// Created: ABR 2012-05-29
// -----------------------------------------------------------------------------
QString FilterPartiesListView::GetTeamList()
{
    QString result = "";
    for( Q3ListViewItemIterator it( this ); it.current(); ++it )
        if( ( *it )->text( eHiddenCheckbox ).toInt() > 0 )
            result += ( result.isEmpty() ) ? ( *it )->text( eHiddenPartyID ) : QString( ";%1" ).arg( ( *it )->text( eHiddenPartyID ) );
    return result;
}
