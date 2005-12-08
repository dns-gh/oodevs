//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LineManager.cpp $
// $Author: Ape $
// $Modtime: 30/07/04 11:23 $
// $Revision: 7 $
// $Workfile: MOS_LineManager.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_LineManager.h"

#include "MOS_Lima.h"
#include "MOS_Limit.h"

//-----------------------------------------------------------------------------
// Name: MOS_LineManager constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_LineManager::MOS_LineManager()
: lineSet_              ()
, bUseSimTacticalLines_ ( false )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_LineManager::~MOS_LineManager()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: MOS_LineManager::UseSimTacticalLines
// Created: SBO 2005-12-08
// -----------------------------------------------------------------------------
void MOS_LineManager::UseSimTacticalLines()
{
    DeleteAll();
    bUseSimTacticalLines_ = true;
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::RegisterLine
// Created: APE 2004-04-14
//-----------------------------------------------------------------------------
void MOS_LineManager::RegisterLine( MOS_TacticalLine_ABC& line )
{
    bool bOut = lineSet_.insert( &line ).second;

    if( !bOut )
    {
        MT_LOG_WARNING_MSG( MT_FormatString( "Line %d already exists", line.GetID() ).c_str() );
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_LineManager::DeleteLine
// Created: APE 2004-04-14
//-----------------------------------------------------------------------------
void MOS_LineManager::DeleteLine( MIL_LineID nLineID )
{
    for( IT_TacticalLineSet it = lineSet_.begin(); it != lineSet_.end() ; ++it )
    {
        if( (*it)->GetID() == nLineID )
        {
            delete *it;
            lineSet_.erase( it );
            return;
        }
    }

    MT_LOG_WARNING_MSG( MT_FormatString( "Line %d already removed", nLineID ).c_str() );
}


// -----------------------------------------------------------------------------
// Name: MOS_LineManager::OnDeconnexion
// Created: APE 2004-07-30
// -----------------------------------------------------------------------------
void MOS_LineManager::OnDeconnexion()
{
    for( IT_TacticalLineSet it = lineSet_.begin(); it != lineSet_.end(); )
    {
        if( ! (*it)->IsCreatedByMos() )
        {
            delete *it;
            it = lineSet_.erase( it );
            continue;
        }

        (*it)->SetState       ( MOS_TacticalLine_ABC::eStateCreated );
        (*it)->SetNetworkState( MOS_TacticalLine_ABC::eNetworkStateNotRegistered );
        ++it;
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::FindLine
// Created: APE 2004-04-14
//-----------------------------------------------------------------------------
MOS_TacticalLine_ABC* MOS_LineManager::FindLine( MIL_LineID nLineID )
{
    for( IT_TacticalLineSet it = lineSet_.begin(); it != lineSet_.end() ; ++it )
    {
        if( (*it)->GetID() == nLineID )
            return *it;
    }

    return 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_LineManager::UpdateToSim
// Created: APE 2004-04-14
//-----------------------------------------------------------------------------
bool MOS_LineManager::UpdateToSim()
{
    for ( CIT_TacticalLineSet it = lineSet_.begin() ; it != lineSet_.end() ; ++it )
    {
        if( ! (*it)->UpdateToSim() )
            return false;
    }
    return true;
}


// -----------------------------------------------------------------------------
// Name: MOS_LineManager::Read
// Created: APE 2004-07-26
// -----------------------------------------------------------------------------
void MOS_LineManager::Read( MT_InputArchive_ABC& archive )
{
    archive.Section( "TacticalLines" );

    archive.BeginList( "Lines" );
    while( archive.NextListElement() )
    {
        std::string strSection = archive.GetCurrentElementName();
        if( strSection == "Lima" )
        {
            std::auto_ptr<MOS_Lima> spLima( new MOS_Lima() );
            spLima->Read( archive );
            lineSet_.insert( spLima.release() );
        }
        else if( strSection == "Limit" )
        {
            std::auto_ptr<MOS_Limit> spLimit( new MOS_Limit() );
            spLimit->Read( archive );
            lineSet_.insert( spLimit.release() );
        }
        else
            throw MT_Exception( "MOS_LineManager::Read", 1, "Parsing error" );
    }
    archive.EndList();
    archive.EndSection();
}


// -----------------------------------------------------------------------------
// Name: MOS_LineManager::Write
// Created: APE 2004-07-26
// -----------------------------------------------------------------------------
void MOS_LineManager::Write( MT_OutputArchive_ABC& archive ) const
{
    archive.Section( "TacticalLines" );
    archive.BeginList( "Lines", lineSet_.size() );
    for( CIT_TacticalLineSet it = lineSet_.begin(); it != lineSet_.end(); ++it )
    {
        // Only save the limits/limas we created, not those sent by the simulation.
        if( (*it)->IsCreatedByMos() )
            (*it)->Write( archive );
    }
        
    archive.EndList();
    archive.EndSection();
}
