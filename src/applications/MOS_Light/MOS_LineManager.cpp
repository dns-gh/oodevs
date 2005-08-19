//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LineManager.cpp $
// $Author: Nld $
// $Modtime: 1/03/03 2:28 $
// $Revision: 12 $
// $Workfile: MOS_LineManager.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_pch.h"

#include "MOS_LineManager.h"

#ifndef MOS_USE_INLINE
#   include "MOS_LineManager.inl"
#endif

#include "MOS_Lima.h"
#include "MOS_Limit.h"
#include "MOS_App.h"
#include "MOS_MOSServer.h"
#include "MOS_AgentServerController.h"
#include "MOS_AgentServerMsgMgr.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_LineManager constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_LineManager::MOS_LineManager()
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_LineManager destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_LineManager::~MOS_LineManager()
{
    RemoveAllLimas();
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_LineManager::Initialize
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void MOS_LineManager::Initialize()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::Terminate
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void MOS_LineManager::Terminate()
{
    
}

//=============================================================================
// LIMAS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_LineManager::RegisterLima
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_LineManager::RegisterLima( MOS_Lima& lima )
{
    bool bOut = limaSet_.insert( &lima ).second;
    if( !bOut )
    {
        MT_LOG_WARNING_MSG( MT_FormatString( "Lima %d already exists", lima.GetID() ).c_str() );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::UnregisterLima
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_LineManager::UnregisterLima( MOS_Lima& lima )
{
    uint nOut = limaSet_.erase( &lima );
    assert( nOut == 1 );
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::RemoveLima
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
void MOS_LineManager::RemoveLima( MIL_LimaID nLimaID )
{
    MOS_Lima* pLine = FindLima( nLimaID );
    if( !pLine )
    {
        MT_LOG_WARNING_MSG( MT_FormatString( "Lima %d already removed", nLimaID ).c_str() );
        return;
    }
    for ( CIT_LimaListViewSet itListView = limaListViewSet_.begin() ; itListView != limaListViewSet_.end() ; ++itListView )
        pLine->DeleteListView( **itListView );
    delete pLine; // NB ; erase dans le set fait dans ~
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::RemoveAllLimas
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
void MOS_LineManager::RemoveAllLimas()
{
    while( limaSet_.size() )
    {
        MOS_Lima& lima = **limaSet_.begin();
        for ( CIT_LimaListViewSet itListView = limaListViewSet_.begin() ; itListView != limaListViewSet_.end() ; ++itListView )
            lima.DeleteListView( **itListView );
        
        delete &lima;
    }
    limaSet_.clear();
}

//-----------------------------------------------------------------------------
// Name: MOS_LineManager::FindLima
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_Lima* MOS_LineManager::FindLima( MIL_LimaID nLimaID )
{
    for ( CIT_LimaSet itLima = limaSet_.begin() ; itLima != limaSet_.end() ; ++itLima )
    {
        if ( (*itLima)->GetID() == nLimaID )
           break;
    }
    if( itLima == limaSet_.end() )
        return 0;

    return *itLima;
}

//=============================================================================
// LIMITS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_LineManager::AddLimit
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_LineManager::RegisterLimit( MOS_Limit& limit )
{
    bool bOut = limitSet_.insert( &limit ).second;

    if( !bOut )
    {
        MT_LOG_WARNING_MSG( MT_FormatString( "Limit %d already exists", limit.GetID() ).c_str() );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::RemoveLimit
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_LineManager::UnregisterLimit( MOS_Limit& limit )
{
    uint nOut = limitSet_.erase( &limit );
    assert( nOut == 1 );
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::RemoveLimit
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
void MOS_LineManager::RemoveLimit( MIL_LimitID nLimitID )
{
    MOS_Limit* pLine = FindLimit( nLimitID );
    if( !pLine )
    {
        MT_LOG_WARNING_MSG( MT_FormatString( "Limit %d already removed", nLimitID ).c_str() );
        return;
    }
    for ( CIT_LimitListViewSet itListView = limitListViewSet_.begin() ; itListView != limitListViewSet_.end() ; ++itListView )
        pLine->DeleteListView( **itListView );
    delete pLine; // NB ; erase dans le set fait dans ~
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::RemoveAllLimits
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
void MOS_LineManager::RemoveAllLimits()
{
    while( limitSet_.size() )
    {
        MOS_Limit& limit = **limitSet_.begin();
        for ( CIT_LimitListViewSet itListView = limitListViewSet_.begin() ; itListView != limitListViewSet_.end() ; ++itListView )
            limit.DeleteListView( **itListView );
        
        delete &limit;
    }
    limitSet_.clear();
}

//-----------------------------------------------------------------------------
// Name: MOS_LineManager::FindLimit
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_Limit* MOS_LineManager::FindLimit( MIL_LimitID nLimitID )
{
    IT_LimitSet itLimit = limitSet_.begin();
    for (  ; itLimit != limitSet_.end() ; ++itLimit )
    {
        if ( (*itLimit)->GetID() == nLimitID )
            break;
    }
    if( itLimit == limitSet_.end() )
        return 0;

    return *itLimit;
}



//=============================================================================
// LIMIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_LineManager::UpdateLimitToSim
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
bool MOS_LineManager::UpdateLimitToSim()
{
    for ( CIT_LimitSet itLimit = limitSet_.begin() ; itLimit != limitSet_.end() ; ++itLimit )
    {
        if ( (**itLimit).UpdateLimitToSim() )
            return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: MOS_LineManager::RegisterLimitListView
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
void MOS_LineManager::RegisterLimitListView( MOS_LimitListView& limitListView )
{
    limitListViewSet_.insert( &limitListView );

    for ( CIT_LimitSet itLimit = limitSet_.begin() ; itLimit != limitSet_.end() ; ++itLimit )
        (**itLimit).RegisterListView( limitListView );
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::UnregisterLimitListView
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
void MOS_LineManager::UnregisterLimitListView( MOS_LimitListView& limitListView )
{
    limitListViewSet_.erase( &limitListView );

    for ( CIT_LimitSet itLimit = limitSet_.begin() ; itLimit != limitSet_.end() ; ++itLimit )
        (**itLimit).UnregisterListView( limitListView );
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::RegisterListViewsForLimit
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
void MOS_LineManager::RegisterListViewsForLimit( MOS_Limit& limit )
{
    for ( CIT_LimitListViewSet itListView = limitListViewSet_.begin() ; itListView != limitListViewSet_.end() ; ++itListView )
        limit.RegisterListView( **itListView );
}


//=============================================================================
// LIMA
//=============================================================================


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::UpdateLimaToSim
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
bool MOS_LineManager::UpdateLimaToSim()
{
    for ( CIT_LimaSet itLima = limaSet_.begin() ; itLima != limaSet_.end() ; ++itLima )
    {
        if ( (**itLima).UpdateLimaToSim() )
            return true;
    }
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::RegisterLimaListView
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
void MOS_LineManager::RegisterLimaListView( MOS_LimaListView& limaListView )
{
    limaListViewSet_.insert( &limaListView );

    for ( CIT_LimaSet itLima = limaSet_.begin() ; itLima != limaSet_.end() ; ++itLima )
        (**itLima).RegisterListView( limaListView );
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::UnregisterLimaListView
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
void MOS_LineManager::UnregisterLimaListView( MOS_LimaListView& limaListView )
{
    limaListViewSet_.erase( &limaListView );

    for ( CIT_LimaSet itLima = limaSet_.begin() ; itLima != limaSet_.end() ; ++itLima )
        (**itLima).UnregisterListView( limaListView );
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::RegisterListViewsForLima
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
void MOS_LineManager::RegisterListViewsForLima( MOS_Lima& lima )
{
    for ( CIT_LimaListViewSet itListView = limaListViewSet_.begin() ; itListView != limaListViewSet_.end() ; ++itListView )
        lima.RegisterListView( **itListView );
}


