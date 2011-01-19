// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Schedule_Table.h"
#include "moc_ADN_Schedule_Table.cpp"
#include "ADN_Urban_Data.h"
#include "ADN_People_Data.h"
#include <qdatetimeedit.h>
#pragma warning( push )
#pragma warning( disable : 4996 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table constructor
// Created: LGY 2011-01-17
// -----------------------------------------------------------------------------
ADN_Schedule_Table::ADN_Schedule_Table( QWidget* pParent )
    : QTable( pParent )
{
    setNumCols( 4 );
    horizontalHeader()->setLabel( 0, qApp->translate( "ADN_Schedule_Table", "Day" ) );
    horizontalHeader()->setLabel( 1, qApp->translate( "ADN_Schedule_Table", "From" ) );
    horizontalHeader()->setLabel( 2, qApp->translate( "ADN_Schedule_Table", "To" ) );
    horizontalHeader()->setLabel( 3, qApp->translate( "ADN_Schedule_Table", "Motivation" ) );
    setEnabled( false );
    connect( this, SIGNAL( contextMenuRequested( int, int, const QPoint& ) ), this, SLOT( OnContextMenu( int, int, const QPoint& ) ) );
    connect( this, SIGNAL( valueChanged( int, int ) ), this, SLOT( OnValueChanged( int, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table destructor
// Created: LGY 2011-01-17
// -----------------------------------------------------------------------------
ADN_Schedule_Table::~ADN_Schedule_Table()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table::CreateWeek
// Created: LGY 2011-01-17
// -----------------------------------------------------------------------------
QStringList ADN_Schedule_Table::CreateWeek() const
{
    QStringList list;
    list.append( "monday" );
    list.append( "tuesday" );
    list.append( "wednesday" );
    list.append( "thursday" );
    list.append( "friday" );
    list.append( "saturday" );
    list.append( "sunday" );
    return list;
}

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table::CreateAccommodations
// Created: LGY 2011-01-17
// -----------------------------------------------------------------------------
QStringList ADN_Schedule_Table::CreateAccommodations() const
{
    QStringList list;
    ADN_Urban_Data::T_AccommodationInfos_Vector& accommodations = ADN_Workspace::GetWorkspace().GetUrban().GetData().GetAccommodationsInfos();
    for( ADN_Urban_Data::CIT_AccommodationInfos_Vector it = accommodations.begin(); it != accommodations.end(); ++it )
        list.append( (*it)->strName_.GetData().c_str() );
    return list;
}

namespace
{
    QTimeEdit* CreateItem( QWidget* pParent, const std::string& date )
    {
        QTimeEdit* pItem = new QTimeEdit( pParent );
        pItem->setDisplay( QTimeEdit::Hours + QTimeEdit::Minutes );
        std::vector< std::string > result;
        boost::algorithm::split( result, date, boost::algorithm::is_any_of( ":" ) );
        if( result.size() != 2 )
            throw std::runtime_error( "Invalid date event" );
        try
        {
            QTime time( boost::lexical_cast< int >( result.front() ), boost::lexical_cast< int >( result.back() ) );
            pItem->setTime( time );
        }
        catch( boost::bad_lexical_cast& /*e*/ )
        {
            throw std::runtime_error( "Invalid date event" );
        }
        return pItem;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table::OnContextMenu
// Created: LGY 2011-01-17
// -----------------------------------------------------------------------------
void ADN_Schedule_Table::OnContextMenu( int row, int col, const QPoint& point )
{
    QPopupMenu menu( this );
    menu.insertItem( qApp->translate( "ADN_Schedule_Table", "Add" ), 0 );
    menu.insertItem( qApp->translate( "ADN_Schedule_Table", "Remove" ), 1 );
    int nMenuResult = menu.exec( point );
    if( nMenuResult == -1 )
        return;
    else if( nMenuResult == 0 )
    {
        int rows = numRows();
        AddRow( rows );
        QTime toTime = static_cast< QTimeEdit* >( cellWidget( rows, 2 ) )->time();
        QTime fromTime = static_cast< QTimeEdit* >( cellWidget( rows, 1 ) )->time();
        AddEvent( rows, item( rows, 0 )->text().ascii(), fromTime.toString( "hh:mm" ).ascii(),
                  toTime.toString( "hh:mm" ).ascii(), item( rows, 3 )->text().ascii() );
    }
    else if( item( row, col ) )
            Remove( row );
}

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table::OnPeopleChanged
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
void ADN_Schedule_Table::OnPeopleChanged( void* pData )
{
    for( int i = numRows() - 1; i >= 0; --i )
        removeRow( i );
    pCurData_ = pData;
    if( pCurData_ )
    {
        setEnabled( true );
        ADN_People_Data::PeopleInfos* pInfos = static_cast< ADN_People_Data::PeopleInfos* >( pCurData_ );
        if( pInfos == 0 )
            return;
        for( ADN_People_Data::PeopleInfos::IT_Events it = pInfos->schedule_.begin(); it != pInfos->schedule_.end(); ++it )
            AddRow( it->first, it->second->day_.GetData(),  it->second->from_.GetData(), it->second->to_.GetData(),
                    it->second->motivation_.GetData() );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table::OnValueChanged
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
void ADN_Schedule_Table::OnValueChanged( int row, int /*col*/ )
{
    if( pCurData_ )
    {
        QTime toTime = static_cast< QTimeEdit* >( cellWidget( row, 2 ) )->time();
        QTime fromTime = static_cast< QTimeEdit* >( cellWidget( row, 1 ) )->time();
        AddEvent( row, item( row, 0 )->text().ascii(), fromTime.toString( "hh:mm" ).ascii(),
                  toTime.toString( "hh:mm" ).ascii(), item( row, 3 )->text().ascii() );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table::AddEvent
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
void ADN_Schedule_Table::AddEvent( int index, const std::string& day, const std::string& from,
                                   const std::string& to, const std::string& motivation )
{
    ADN_People_Data::PeopleInfos* pInfos = static_cast< ADN_People_Data::PeopleInfos* >( pCurData_ );
    if( pInfos == 0 )
        return;
    if( pInfos->schedule_.find( index ) == pInfos->schedule_.end() )
         pInfos->schedule_[ index ].reset( new ADN_People_Data::EventInfos() );
    pInfos->schedule_[ index ]->day_ = day;
    pInfos->schedule_[ index ]->motivation_ = motivation;
    pInfos->schedule_[ index ]->from_ = from;
    pInfos->schedule_[ index ]->to_ = to;
}

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table::AddRow
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
void ADN_Schedule_Table::AddRow( int rows, const std::string& day, const std::string& from, const std::string& to,
                                 const std::string& accommodation )
{
    insertRows( rows );

    QComboTableItem* pCombo = new QComboTableItem( this, CreateWeek() );
    pCombo->setCurrentItem( day.c_str() );
    setItem( rows, 0, pCombo );

    QTimeEdit* pFrom = CreateItem( this, from );
    connect( pFrom, SIGNAL( valueChanged( const QTime& ) ), this, SLOT( OnTimeChanged( const QTime& ) ) );
    setCellWidget( rows, 1, pFrom );

    QTimeEdit* pTo = CreateItem( this, to );
    connect( pTo, SIGNAL( valueChanged( const QTime& ) ), this, SLOT( OnTimeChanged( const QTime& ) ) );
    setCellWidget( rows, 2, pTo );

    QComboTableItem* pAccommodation = new QComboTableItem( this, CreateAccommodations() );
    pAccommodation->setCurrentItem( accommodation.c_str() );
    setItem( rows, 3, pAccommodation );
}

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table::OnTimeChanged
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
void ADN_Schedule_Table::OnTimeChanged( const QTime& /*time*/ )
{
    for( int i = numRows() - 1; i >= 0; --i )
        OnValueChanged( i, 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Schedule_Table::Remove
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
void ADN_Schedule_Table::Remove( int rows )
{
    removeRow( rows );
    if( pCurData_ )
    {
        ADN_People_Data::PeopleInfos* pInfos = static_cast< ADN_People_Data::PeopleInfos* >( pCurData_ );
        if( pInfos == 0 )
            return;
        pInfos->schedule_.clear();
        for( int i = numRows() - 1; i >= 0; --i )
            OnValueChanged( i, 0 );
    }
}
