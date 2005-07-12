//*****************************************************************************
//
// $Created: FBD 03-08-25 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_HistoryItem.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_HistoryItem.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_HistoryItem.h"

#ifndef MOS_USE_INLINE
#	include "MOS_HistoryItem.inl"
#endif

#include "MOS_Agent.h"

#include "MOS_History.h"
#include "MOS_HistoryMission.h"
#include "MOS_App.h"

#include <QPainter.h>
#include <QPixmap.h>
#include <QImage.h>




//-----------------------------------------------------------------------------
// Name: MOS_HistoryItem constructor
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
MOS_HistoryItem::MOS_HistoryItem( MOS_History* pTable, MOS_Agent& agent )
:   QTableItem( pTable, Never, "" )
,   pAgent_( &agent )
,   nTotalTime_( 0 )
{
    uint nRawWidth = sizeHint().width();
    uint nRawHeight = sizeHint().height();
    uchar* pBuffer = (uchar*)malloc( nRawWidth * nRawHeight * 3 );
    QImage image( (uchar*)pBuffer, nRawWidth, nRawHeight, 24, (QRgb*)NULL, 0, QImage::LittleEndian );
    QPixmap pix( image );
    pix.fill();
    setPixmap( pix );

}


//-----------------------------------------------------------------------------
// Name: MOS_HistoryItem destructor
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
MOS_HistoryItem::~MOS_HistoryItem()
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_HistoryItem::AddMission
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
MOS_HistoryMission& MOS_HistoryItem::AddMission( uint nTimeStart, uint nTimeEnd, std::string& sName, std::string& sParam, E_MissionID nMission )
{
    if ( missionVector_.size() )
    {
        MOS_HistoryMission& last = **missionVector_.rbegin();
        last.SetTimeEnd( nTimeStart );
    }
        
    MOS_HistoryMission& mission = *new MOS_HistoryMission( nTimeStart, nTimeEnd, sName, sParam, nMission );
    missionVector_.push_back( &mission );
    return mission;
}


//-----------------------------------------------------------------------------
// Name: MOS_HistoryItem::SetLastOrderState
// Created: FBD 03-09-02
//-----------------------------------------------------------------------------
void MOS_HistoryItem::SetLastOrderState( E_StateMissionHistory nState, uint nTime )
{
    if ( missionVector_.size() )
    {
        MOS_HistoryMission& last = **missionVector_.rbegin();
        last.SetOrderState( nState, nTime );
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_HistoryItem::DelMission
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
void MOS_HistoryItem::DelMission( MOS_HistoryMission& mission )
{
   IT_MOS_HistoryMissionVector itMission = std::find( missionVector_.begin(), missionVector_.end(), &mission );
   if ( itMission != missionVector_.end() )
   {
        missionVector_.erase( itMission );
        delete &mission;
   }

}



//-----------------------------------------------------------------------------
// Name: MOS_HistoryItem::paint
// Created: FBD 03-08-25
//-----------------------------------------------------------------------------
void MOS_HistoryItem::paint( QPainter* p, const QColorGroup & cg, const QRect & cr, bool selected )
{    
    QTableItem::paint( p, cg, cr, selected );

    
    if ( nTotalTime_ < MOS_App::GetApp().GetTime() )
        nTotalTime_ = MOS_App::GetApp().GetTime() + 200;
   
    for ( CIT_MOS_HistoryMissionVector itMission = missionVector_.begin() ; itMission != missionVector_.end() ; ++itMission )
    {
        MOS_HistoryMission& mission = **itMission;
        const std::string& sName = mission.GetName();
        uint nTimeStart = mission.GetTimeStart();
        uint nTimeEnd = mission.GetTimeEnd();
        uint nColMission  = mission.GetColor();
        uint nLight       = mission.GetColorState();
        if ( nTimeEnd == -1 )
        {
            nTimeEnd = MOS_App::GetApp().GetTime();
        }

        



        uint nWidthTotal = cr.width();
        MT_Float rRatio = (MT_Float)nTotalTime_ / (MT_Float)nWidthTotal;
        uint nX1 = nTimeStart / rRatio;        
        uint nX2 = nTimeEnd / rRatio;
        nX1 += 1;
        nX2 -= 1;
        uint nY1 = 1;
        uint nY2 = cr.height();
        if ( nX1 < nX2 )
        {
            uint nWidth = nX2 - nX1;
            uint nHeight = nY2 - nY1;
            mission.SetRect( QRect( nX1+cr.left(), nY1 + cr.top(), nWidth, nHeight ) );

            MT_Float rFactor = 32. / (MT_Float)nWidth;
            MT_Float rCol = nColMission;
            for ( uint i = nX1 ; i < nX2 ; ++i )
            {
                rCol += rFactor;
                QColor c;
                c.setHsv( (int)rCol, nLight, 255 );  
                p->setBrush( c );                 
                p->setPen( c );
                p->drawRect( i, nY1, 1, nHeight );
            }

            uint nSizeXLine = 2;
            uint nSizeYLine = 2;

            p->setPen( lightGray );
            p->fillRect( nX1             , nY1                   , nWidth,       nSizeYLine, black    );
            p->fillRect( nX2 - nSizeXLine, nY1                   , nSizeXLine,   nHeight   , black    );

            p->setPen( darkGray );
            p->fillRect( nX1             , nHeight - nSizeYLine  , nWidth,       nSizeYLine, gray    );
            p->fillRect( nX1             , nY1                 , nSizeXLine,   nHeight   , gray    );
 
            p->setPen( black );
            QRect recta( nX1 + nSizeXLine, nY1 + nSizeYLine, nWidth - nSizeXLine*2, nHeight - nSizeYLine *2 );
            p->drawText( recta, Qt::AlignHCenter | Qt::AlignVCenter, sName.c_str() );
        
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_HistoryItem::tip
// Created: FBD 03-08-27
//-----------------------------------------------------------------------------
QRect* MOS_HistoryItem::tip( const QPoint& p, std::string& sParam )
{
    for ( CIT_MOS_HistoryMissionVector itMission = missionVector_.begin() ; itMission != missionVector_.end() ; ++itMission )
    {
        MOS_HistoryMission& mission = **itMission;
        if ( mission.GetRect().contains( p ) )
        {
            const std::string& sName = mission.GetParam();
            sParam = sName;
            return &mission.GetRect();
        }
    }
    return 0;
}




