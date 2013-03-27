// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ObjectNameManager.h"

using namespace gui;

ObjectNameManager* ObjectNameManager::instance_ = NULL;

// -----------------------------------------------------------------------------
// Name: ObjectNameManager constructor
// Created: NPT 2013-03-11
// -----------------------------------------------------------------------------
ObjectNameManager::ObjectNameManager()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectNameManager destructor
// Created: NPT 2013-03-11
// -----------------------------------------------------------------------------
ObjectNameManager::~ObjectNameManager()
{
    instance_ = 0;
}

// -----------------------------------------------------------------------------
// Name: NamingManager::getInstance
// Created: NPT 2013-03-11
// -----------------------------------------------------------------------------
ObjectNameManager* ObjectNameManager::getInstance()
{
    if ( instance_ == NULL )
        instance_ = new ObjectNameManager;
    return instance_;
}

namespace
{
    void DisplayErrorMessage( const QString& message )
    {
        QMessageBox messageBox;
        messageBox.setWindowTitle( "Naming Errors" ); 
        messageBox.setText( message );
        messageBox.exec();
    }
}

// -----------------------------------------------------------------------------
// Name: NamingManager::AddSubLevel
// Created: NPT 2013-03-11
// -----------------------------------------------------------------------------
void ObjectNameManager::AddSubLevel( const QString& name )
{
    if( name.isEmpty() )
    {
        DisplayErrorMessage( "this parent doesn't have a name" );
        return;
    }
    objectNamePath_.append( name );
}

// -----------------------------------------------------------------------------
// Name: NamingManager::GetSubName
// Created: NPT 2013-03-11
// -----------------------------------------------------------------------------
QString ObjectNameManager::GetSubName()
{
    return objectNamePath_.join( "_" );
}

// -----------------------------------------------------------------------------
// Name: NamingManager::RemoveSubLevel
// Created: NPT 2013-03-11
// -----------------------------------------------------------------------------
void ObjectNameManager::RemoveSubLevel()
{
    if( !objectNamePath_.isEmpty() )
        objectNamePath_.removeLast();
}

// -----------------------------------------------------------------------------
// Name: ObjectNameManager::SetObjectName
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
void ObjectNameManager::SetObjectName( QObject* obj, const QString& name )
{
    if( name.isEmpty() )
    {
        DisplayErrorMessage( "this object doesn't have a name" );
        return;
    }
    if( paths_.contains( GetSubName() + "_" + name ) )
    {
        DisplayErrorMessage( QString( GetSubName() + "_" + name + " : this object name already exist" ) );
        return;
    }
    obj->setObjectName( GetSubName() + "_" + name );
    paths_.append( GetSubName() + "_" + name );
}

// -----------------------------------------------------------------------------
// Name: ObjectNameManager::RemoveRegisteredName
// Created: NPT 2013-03-15
// -----------------------------------------------------------------------------
void ObjectNameManager::RemoveRegisteredName( const QString& name )
{
    if( paths_.contains( name ) )
        paths_.remove( name );
}
