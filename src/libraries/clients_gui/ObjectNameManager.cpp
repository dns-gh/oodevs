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

ObjectNameManager *ObjectNameManager::instance_ = NULL;

// -----------------------------------------------------------------------------
// Name: ObjectNameManager constructor
// Created: NPT 2013-03-11
// -----------------------------------------------------------------------------
ObjectNameManager::ObjectNameManager()
    : objectNamePath_ ( new QStringList() )
    , paths_          ( new QStringList() )
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

// -----------------------------------------------------------------------------
// Name: NamingManager::AddSubLevel
// Created: NPT 2013-03-11
// -----------------------------------------------------------------------------
void ObjectNameManager::AddSubLevel( const QString& name )
{
    if( name.isEmpty() )
        throw MASA_EXCEPTION( "this parent doesn't have a name" );

    if( !objectNamePath_->contains( name ) )
        objectNamePath_->append( name );
    else
        throw MASA_EXCEPTION( name.toStdString() + " : Name already given" );
}

// -----------------------------------------------------------------------------
// Name: NamingManager::GetSubName
// Created: NPT 2013-03-11
// -----------------------------------------------------------------------------
QString ObjectNameManager::GetSubName()
{
    return objectNamePath_->join( "_" );
}

// -----------------------------------------------------------------------------
// Name: NamingManager::RemoveSubLevel
// Created: NPT 2013-03-11
// -----------------------------------------------------------------------------
void ObjectNameManager::RemoveSubLevel()
{
    objectNamePath_->removeLast();
}

// -----------------------------------------------------------------------------
// Name: ObjectNameManager::SetObjectName
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
void ObjectNameManager::SetObjectName( QObject* obj, const QString& name )
{
    if( name.isEmpty() )
        throw MASA_EXCEPTION( "this object doesn't have a name" );

    obj->setObjectName( GetSubName() + "_" + name );
    std::cout << obj->objectName().toStdString() << std::endl;

    if( paths_->contains( GetSubName() + "_" + name ) )
        throw MASA_EXCEPTION( QString( GetSubName() + "_" + name + " : this object name already exist" ).toStdString() );

    paths_->append( GetSubName() + "_" + name );
}

// -----------------------------------------------------------------------------
// Name: ObjectNameManager::RemoveRegisteredName
// Created: NPT 2013-03-15
// -----------------------------------------------------------------------------
void ObjectNameManager::RemoveRegisteredName( const QString& name )
{
    if( paths_->contains( name ) )
        paths_->remove( name );
}

