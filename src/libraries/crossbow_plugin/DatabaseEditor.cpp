// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "DatabaseEditor.h"

using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: DatabaseEditor constructor
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
DatabaseEditor::DatabaseEditor( IWorkspacePtr spWorkspace )
    : inTransaction_ ( false )
    , editing_ ( false) 
{
    if( FAILED( spWorkspace->QueryInterface( IID_IWorkspaceEdit, (LPVOID*)&workspaceEdit_ ) ) || workspaceEdit_ == NULL )
        throw std::runtime_error( "Cannot retrieve IWorkspaceEdit interface." );
}
    
// -----------------------------------------------------------------------------
// Name: DatabaseEditor destructor
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
DatabaseEditor::~DatabaseEditor()
{
    if( IsEditing() )
    {
        workspaceEdit_->StopEditOperation();
        workspaceEdit_->StopEditing( VARIANT_TRUE );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseEditor::IsEditing
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
bool DatabaseEditor::IsEditing() const
{
    return editing_;
    try
    {
        VARIANT_BOOL editing;
        return SUCCEEDED( workspaceEdit_->IsBeingEdited( &editing ) ) && editing == VARIANT_TRUE;
    }
    catch( std::exception& e )
    {
        MT_LOG_INFO_MSG( e.what() );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DatabaseEditor::Lock
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
void DatabaseEditor::Lock()
{
    try
    {
        if( ! IsEditing() )
            editing_ = SUCCEEDED( workspaceEdit_->StartEditing( VARIANT_FALSE ) );

    }
    catch( std::exception& e )
    {
        MT_LOG_INFO_MSG( e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseEditor::Lock
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
void DatabaseEditor::UnLock()
{
    try
    {
        StopEdit();
        if( IsEditing() )
            workspaceEdit_->StopEditing( VARIANT_TRUE );
        editing_ = true;
    }
    catch( std::exception& e )
    {
        MT_LOG_INFO_MSG( e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseEditor::Lock
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
void DatabaseEditor::StartEdit()
{    
    if( !InTransaction() )
    {
        Lock();
        workspaceEdit_->StartEditOperation();
        BeginTransaction();
    }
}
    
// -----------------------------------------------------------------------------
// Name: DatabaseEditor::Lock
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
void DatabaseEditor::StopEdit()
{
    if( InTransaction() )
    {
        workspaceEdit_->StopEditOperation();
        EndTransaction();        
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseEditor::Lock
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
void DatabaseEditor::BeginTransaction()
{
    inTransaction_ = true;
}

// -----------------------------------------------------------------------------
// Name: DatabaseEditor::Lock
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
void DatabaseEditor::EndTransaction()
{
    inTransaction_ = false;
}

// -----------------------------------------------------------------------------
// Name: DatabaseEditor::Lock
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
bool DatabaseEditor::InTransaction() const
{
    return inTransaction_;
}
