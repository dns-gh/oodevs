//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_DynaObjectManager.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 5 $
// $Workfile: MOS_DynaObjectManager.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_DynaObjectManager.h"

#ifndef MOS_USE_INLINE
#   include "MOS_DynaObjectManager.inl"
#endif

#include "MOS_DynaObject.h"
#include "MOS_DynaObject_ListView.h"
#include "MOS_DynaObject_ListViewItem.h"

//-----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager constructor
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
MOS_DynaObjectManager::MOS_DynaObjectManager()
: pListView_( 0 )
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager destructor
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
MOS_DynaObjectManager::~MOS_DynaObjectManager()
{
    
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager::Initialize
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void MOS_DynaObjectManager::Initialize()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager::Terminate
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void MOS_DynaObjectManager::Terminate()
{
    
}
//=============================================================================
// MAIN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager::RegisterDynaObject
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_DynaObjectManager::RegisterDynaObject( MOS_DynaObject& object )
{
    bool bOut = dynaObjectMap_.insert( std::make_pair( object.GetID(), &object ) ).second;
    if( pListView_ != 0 )
        pListView_->AddObject( object );
    assert( bOut );
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager::UnregisterDynaObject
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_DynaObjectManager::UnregisterDynaObject( MOS_DynaObject& object )
{
    uint nOut = dynaObjectMap_.erase( object.GetID() );
    if( pListView_ != 0 )
        pListView_->RemoveObject( object );
//    if( pListView_ != 0 && object.GetViewItem() != 0 )
//    {
//        pListView_->takeItem( object.GetViewItem() );
//        object.SetViewItem( 0 );
//        
//    }

    assert( nOut == 1 );
}

//-----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager::RemoveAllDynaObjects
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
void MOS_DynaObjectManager::RemoveAllDynaObjects()
{
    while( dynaObjectMap_.begin() != dynaObjectMap_.end() )
    {
        MOS_DynaObject& object = *dynaObjectMap_.begin()->second;
        UnregisterDynaObject( object );
        object.Terminate();
        delete &object;
    }

    assert( dynaObjectMap_.empty() );

//    pListView_->clear();

    /*
            MOS_App::GetApp().GetMainWindow().NotifyDeleteAgent( agent );

        for ( CIT_AgentListViewSet itListView = agentListViewSet_.begin() ; itListView != agentListViewSet_.end() ; ++itListView )
            agent.DeleteListView( **itListView );
            */

//    for( IT_DynaObjectMap itObject = dynaObjectMap_.begin(); itObject != dynaObjectMap_.end(); ++itObject )
//        delete itObject->second;

    dynaObjectMap_.clear();
}

//=============================================================================
// DRAW
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager::Draw
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void MOS_DynaObjectManager::Draw()
{
    for( IT_DynaObjectMap itDynaObject = dynaObjectMap_.begin(); itDynaObject != dynaObjectMap_.end(); ++itDynaObject )
        itDynaObject->second->Draw();
}

//=============================================================================
// GUI
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager::CreateListView
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
QListView* MOS_DynaObjectManager::CreateListView( QWidget* pParent )
{
    // Creates the list view
    pListView_ = new MOS_DynaObject_ListView( pParent );

    // Fills the list view with the existing items
    for( CIT_DynaObjectMap itDynaObject = dynaObjectMap_.begin(); itDynaObject != dynaObjectMap_.end(); ++itDynaObject )
        pListView_->AddObject( *(itDynaObject->second) );

    return pListView_;
}

