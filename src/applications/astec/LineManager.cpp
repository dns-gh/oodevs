// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LineManager.h"

#include "Lima.h"
#include "Limit.h"

//-----------------------------------------------------------------------------
// Name: LineManager constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
LineManager::LineManager()
: lineSet_              ()
, bUseSimTacticalLines_ ( false )
{
}


//-----------------------------------------------------------------------------
// Name: LineManager destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
LineManager::~LineManager()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LineManager::UseSimTacticalLines
// Created: SBO 2005-12-08
// -----------------------------------------------------------------------------
void LineManager::UseSimTacticalLines()
{
    DeleteAll();
    bUseSimTacticalLines_ = true;
}


//-----------------------------------------------------------------------------
// Name: LineManager::RegisterLine
// Created: APE 2004-04-14
//-----------------------------------------------------------------------------
void LineManager::RegisterLine( TacticalLine_ABC& line )
{
    bool bOut = lineSet_.insert( &line ).second;

    if( !bOut )
    {
        MT_LOG_WARNING_MSG( MT_FormatString( "Line %d already exists", line.GetID() ).c_str() );
    }
}



//-----------------------------------------------------------------------------
// Name: LineManager::DeleteLine
// Created: APE 2004-04-14
//-----------------------------------------------------------------------------
void LineManager::DeleteLine( MIL_LineID nLineID )
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
// Name: LineManager::OnDeconnexion
// Created: APE 2004-07-30
// -----------------------------------------------------------------------------
void LineManager::OnDeconnexion()
{
    for( IT_TacticalLineSet it = lineSet_.begin(); it != lineSet_.end(); )
    {
        if( ! (*it)->IsCreatedByMos() )
        {
            delete *it;
            it = lineSet_.erase( it );
            continue;
        }

        (*it)->SetState       ( TacticalLine_ABC::eStateCreated );
        (*it)->SetNetworkState( TacticalLine_ABC::eNetworkStateNotRegistered );
        ++it;
    }
}


//-----------------------------------------------------------------------------
// Name: LineManager::FindLine
// Created: APE 2004-04-14
//-----------------------------------------------------------------------------
TacticalLine_ABC* LineManager::FindLine( MIL_LineID nLineID )
{
    for( IT_TacticalLineSet it = lineSet_.begin(); it != lineSet_.end() ; ++it )
    {
        if( (*it)->GetID() == nLineID )
            return *it;
    }

    return 0;
}


//-----------------------------------------------------------------------------
// Name: LineManager::UpdateToSim
// Created: APE 2004-04-14
//-----------------------------------------------------------------------------
bool LineManager::UpdateToSim()
{
    for ( CIT_TacticalLineSet it = lineSet_.begin() ; it != lineSet_.end() ; ++it )
    {
        if( ! (*it)->UpdateToSim() )
            return false;
    }
    return true;
}


// -----------------------------------------------------------------------------
// Name: LineManager::Read
// Created: APE 2004-07-26
// -----------------------------------------------------------------------------
void LineManager::Read( MT_InputArchive_ABC& archive )
{
    archive.Section( "TacticalLines" );

    archive.BeginList( "Lines" );
    while( archive.NextListElement() )
    {
        std::string strSection = archive.GetCurrentElementName();
        if( strSection == "Lima" )
        {
            std::auto_ptr<Lima> spLima( new Lima() );
            spLima->Read( archive );
            lineSet_.insert( spLima.release() );
        }
        else if( strSection == "Limit" )
        {
            std::auto_ptr<Limit> spLimit( new Limit() );
            spLimit->Read( archive );
            lineSet_.insert( spLimit.release() );
        }
        else
            throw MT_Exception( "LineManager::Read", 1, "Parsing error" );
    }
    archive.EndList();
    archive.EndSection();
}


// -----------------------------------------------------------------------------
// Name: LineManager::Write
// Created: APE 2004-07-26
// -----------------------------------------------------------------------------
void LineManager::Write( MT_OutputArchive_ABC& archive ) const
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
