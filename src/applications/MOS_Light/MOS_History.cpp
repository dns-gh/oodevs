//*****************************************************************************
//
// $Created: FBD 03-08-25 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_History.cpp $
// $Author: Nld $
// $Modtime: 30/11/04 11:42 $
// $Revision: 4 $
// $Workfile: MOS_History.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_History.h"

#ifndef MOS_USE_INLINE
#	include "MOS_History.inl"
#endif


#include <QImage.h>
#include <QPainter.h>

#include "MOS_HistoryItem.h"
#include "MOS_HistoryMission.h"
#include "MOS_Agent.h"
#include "MOS_App.h"

#include <QTable.h>

DynamicTip::DynamicTip( QWidget * parent )
    : QToolTip( parent )
{
    // no explicit initialization needed
}


void DynamicTip::maybeTip( const QPoint &pos )
{
    if( (MOS_History*)parentWidget() )
    {
        std::string sParam;
        QRect r( ((MOS_History*)parentWidget())->tip( pos, sParam ) );
        if ( !r.isValid() )
            return;

        ((MOS_History*)parentWidget())->StartPopup( sParam );
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_History constructor
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
MOS_History::MOS_History( QWidget* pParent )
:   QTable( pParent )
,   pTip_( 0 )
{
    setSelectionMode( SelectionMode::NoSelection );
//    setLeftMargin( 100 );

    setNumCols( 1 );
    setColumnStretchable( 0, false );
    setColumnReadOnly( 0, true );
    setColumnWidth( 0, width() - 122 );
    setColumnReadOnly( 1, true );
    horizontalHeader()->setLabel( 0, tr( "Time" ) );
    horizontalHeader()->setStretchEnabled( false );
    setColumnMovingEnabled ( false );

    pTip_ = new DynamicTip( this );

/*
    uint nRawWidth = columnWidth(0);
    uint nRawHeight = rowHeight(0);   
    uchar* pBuffer = (uchar*)malloc( nRawWidth * nRawHeight * 3 );
    QImage image( (uchar*)pBuffer, nRawWidth, nRawHeight, 24, (QRgb*)NULL, 0, QImage::LittleEndian );
    QPixmap pix( image );
    pix.fill();
    QPainter p( &pix );
    p.setPen( Qt::red );
    p.setWindow( 0, 0, nRawWidth, nRawHeight );             // defines coordinate system

    for ( uint i = 0 ; i < 255 ; ++i )
    {
        QColor c;
        c.setHsv( i, 255, 255 );             // rainbow effect
        p.setBrush( c );                       // solid fill with color c
        p.setPen( c );
        p.drawRect( i, 0, 1, nRawHeight );         // draw the rectangle
    }

    QString n;
    n.sprintf( "Toto = %d", 1*20 );
    p.drawText( 0, 15, n );           // draw the hue number
    p.end();
    setPixmap( 0, 0, pix );
*/
}



//-----------------------------------------------------------------------------
// Name: MOS_History destructor
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
MOS_History::~MOS_History()
{
    if( pTip_ )
        delete pTip_;
}

//-----------------------------------------------------------------------------
// Name: MOS_History::AddAgent
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
void MOS_History::AddAgent( MOS_Agent& agent )
{
   for( IT_HistoryItemVector itItem = itemPtrVector_.begin() ; itItem != itemPtrVector_.end() ; ++itItem )
   {
        MOS_HistoryItem& item = **itItem;
        if ( &item.GetAgent() == &agent )
            return;
   }
    uint nRow = numRows();

    setNumRows( nRow + 1 );
    if( nRow == 0 ) 
    {
        setRowMovingEnabled( false );
        setRowStretchable( nRow, false );
        setRowReadOnly( nRow, true );
    }

    MOS_HistoryItem* pItem = new MOS_HistoryItem( this, agent );
    verticalHeader()->setLabel( nRow, tr( agent.GetName().c_str() ) );
    verticalHeader()->setStretchEnabled( false );

    itemPtrVector_.push_back( pItem );
    setItem( nRow, 0, pItem );
}

//-----------------------------------------------------------------------------
// Name: MOS_History::DelAgent
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
void MOS_History::DelAgent( MOS_Agent& agent )
{
   for( IT_HistoryItemVector itItem = itemPtrVector_.begin() ; itItem != itemPtrVector_.end() ; ++itItem )
   {
        MOS_HistoryItem& item = **itItem;
        if ( &item.GetAgent() == &agent )
        {
            itemPtrVector_.erase( itItem );
            delete &item;
            return;
        }
   }
// assert( false );
}

    

//-----------------------------------------------------------------------------
// Name: MOS_History::AddMission
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
void MOS_History::AddMission( MOS_Agent& agent, std::string& sName, uint nTimeStart, uint nTimeEnd, std::string& sParam, E_MissionID nMission )
{
   for( IT_HistoryItemVector itItem = itemPtrVector_.begin() ; itItem != itemPtrVector_.end() ; ++itItem )
   {
        MOS_HistoryItem& item = **itItem;
        if ( &item.GetAgent() == &agent )
        {
            item.AddMission( nTimeStart, nTimeEnd, sName, sParam, nMission );
            return;
        }
   }
}


//-----------------------------------------------------------------------------
// Name: MOS_History::SetLastOrderState
// Created: FBD 03-09-02
//-----------------------------------------------------------------------------
void MOS_History::SetLastOrderState( MOS_Agent& agent, E_StateMissionHistory nState, uint nTime )
{
   for( IT_HistoryItemVector itItem = itemPtrVector_.begin() ; itItem != itemPtrVector_.end() ; ++itItem )
   {
        MOS_HistoryItem& item = **itItem;
        if ( &item.GetAgent() == &agent )
        {
            item.SetLastOrderState( nState, nTime );
        }
   }
}





//-----------------------------------------------------------------------------
// Name: MOS_History::Update
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
void MOS_History::Update( )
{
    if ( itemPtrVector_.size() == 0 )
        return;

    for ( int i = 0; i < numRows() ; ++i )
       updateCell( i, 0 );

    int nWidth = MOS_App::GetApp().GetTime();
    if ( nWidth < width() )
    {
        nWidth = width();
    }

    setColumnWidth( 0, nWidth );

    if ( horizontalScrollBar() )
    {
        if ( horizontalScrollBar()->value() == horizontalScrollBar()->maxValue() )
        {
            uint nValue = 0;
            if ( verticalScrollBar()  )
            {
                nValue = verticalScrollBar()->value();
            }
            ensureVisible( nWidth - 10, contentsY()  );
            if ( verticalScrollBar()  )
            {
                verticalScrollBar()->setValue( nValue );
            }
        }
    }
    
    
}


//-----------------------------------------------------------------------------
// Name: MOS_History::Reset
// Created: FBD 03-08-26
//-----------------------------------------------------------------------------
void MOS_History::Reset()
{
   for ( uint i = 0 ; i < itemPtrVector_.size() ; ++i )
       delete itemPtrVector_[i];
   itemPtrVector_.clear();
    setNumRows( 0 );
    setColumnWidth( 0, width() - 122 );
}





//-----------------------------------------------------------------------------
// Name: MOS_History::tip
// Created: FBD 03-08-27
//-----------------------------------------------------------------------------
QRect MOS_History::tip( const QPoint& p, std::string& sParam )
{
    if ( numRows() ) 
    {
        QPoint newPoint( p );
        uint nWidth = verticalHeader()->width();
        uint nHeight = horizontalHeader()->height();
        newPoint.setX( newPoint.x() - nWidth );
        newPoint.setY( newPoint.y() - nHeight );
        
        uint nContentX = contentsX();
        uint nContentY = contentsY();
        
        newPoint.setX( newPoint.x() + nContentX );
        newPoint.setY( newPoint.y() + nContentY );

        for( IT_HistoryItemVector itItem = itemPtrVector_.begin() ; itItem != itemPtrVector_.end() ; ++itItem )
        {
            MOS_HistoryItem& item = **itItem;
            QRect* pRect = item.tip( newPoint, sParam );
            if ( pRect )
                return *pRect;
        }
    }
    return QRect( 0,0, -1,-1 );
}



//-----------------------------------------------------------------------------
// Name: MOS_History::StartPopup
// Created: FBD 03-08-27
//-----------------------------------------------------------------------------
void MOS_History::StartPopup( std::string& sParam )
{
    QPopupMenu* pPopup_ = new QPopupMenu( this );
    std::string sText( sParam );
    size_t nVal = sText.find( '|' );
    while ( nVal != std::string::npos )
    {
        std::string sCur( sText.substr( 0, nVal ) );
        pPopup_->insertItem( QString( sCur.c_str() ), 0 );
        sText = sText.substr( nVal + 1, sText.size() );
        if ( sText.size() <= 1  )
            break;
        nVal = sText.find( '|' );
    }
    pPopup_->move( QCursor::pos() );
    pPopup_->exec( QCursor::pos() );
    delete pPopup_;
}


