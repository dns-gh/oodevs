// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: RichWidget constructor
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
template< typename T >
RichWidget< T >::RichWidget( const QString& objectName, QWidget* parent /* = 0 */ )
    : T( parent )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}

// -----------------------------------------------------------------------------
// Name: RichWidget destructor
// Created: ABR 2013-06-07
// -----------------------------------------------------------------------------
template< typename T >
RichWidget< T >::~RichWidget()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}
