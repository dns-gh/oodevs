// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Report.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Styles.h"
#include "Tools.h"
#include <QtCore/qdatetime.h>

Q_DECLARE_METATYPE( const Report* )

const int Report::ReportRole = Qt::UserRole + 1;
const int Report::TypeFilterRole = Qt::UserRole + 2 ;

// -----------------------------------------------------------------------------
// Name: Report constructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
Report::Report( const kernel::Entity_ABC& entity, Report::E_Type type, const QString& message, const QDateTime& time )
    : entity_ ( entity )
    , type_   ( type )
    , message_( message )
    , time_   ( time )
    , isNew_  ( true )
    , point_  ( entity.Get< kernel::Positions >().GetPosition() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Report destructor
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
Report::~Report()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Report::Read
// Created: AGE 2006-09-21
// -----------------------------------------------------------------------------
void Report::Read()
{
    isNew_ = false;
}

// -----------------------------------------------------------------------------
// Name: Report::GetType
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
Report::E_Type Report::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Report::GetDateTime
// Created: NPT 2012-10-08
// -----------------------------------------------------------------------------
const QDateTime& Report::GetDateTime() const
{
    return time_;
}

// -----------------------------------------------------------------------------
// Name: QList< QStandardItem > Report::GetReportData
// Created: NPT 2012-10-05
// -----------------------------------------------------------------------------
QList< QStandardItem* > Report::GetReportData() const
{
    QList< QStandardItem* > list;

    QStandardItem* dateItem = new QStandardItem( time_.toString( Qt::LocalDate ) );
    dateItem->setFont( QFont( "Calibri", 10, QFont::Bold ) );
    dateItem->setForeground( GetColor( type_ ) );

    QStandardItem* reportItem = new QStandardItem( message_ ) ;
    reportItem->setFont( QFont( "Calibri", 10, QFont::Bold ) );
    reportItem->setForeground( GetColor( type_ ) );
    reportItem->setData( QString::number( type_ ), TypeFilterRole );
    reportItem->setData( QVariant::fromValue( this ), ReportRole );
    list.append( dateItem );
    list.append( reportItem );
    return list;
}

// -----------------------------------------------------------------------------
// Name: Report::GetOwner
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& Report::GetOwner() const
{
    return entity_;
}

// -----------------------------------------------------------------------------
// Name: Report::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Report::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Item( "" )
             .Start( GetColor( type_ ) )
             .Add( time_.toString( "hh:mm" ) )
                .Add( " " )
                .Add( message_.section( " - ", 0 ) )
             .End();
}

// -----------------------------------------------------------------------------
// Name: Report::GetColor
// Created: AGE 2006-07-03
// -----------------------------------------------------------------------------
QColor Report::GetColor( E_Type type )
{
    static QColor colors[] = { QColor( 0, 0, 0 ),      // eRC      = 1000,
                               QColor( 118,  83, 38 ), // eTrace   = 1001,
                               QColor(  32, 200, 64 ),  // eEvent   = 1002,
                               QColor(  64, 128, 200 ), // eMessage = 1003,
                               QColor( 255, 128, 64 )}; // eWarning = 1004
    return colors[ type - eRC ];
}
