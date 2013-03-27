// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SubObjectName.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectLevelScope constructor
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
SubObjectName::SubObjectName( const QString& objectName )
{
    ObjectNameManager::getInstance()->AddSubLevel( objectName );
}

// -----------------------------------------------------------------------------
// Name: ObjectLevelScope destructor
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
SubObjectName::~SubObjectName()
{
    ObjectNameManager::getInstance()->RemoveSubLevel();
}
