// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: ADN_TextEdit constructor
// Created: JSR 21-11-11
//-----------------------------------------------------------------------------
template< class Connector >
ADN_TextEdit< Connector >::ADN_TextEdit( QWidget* parent /* = 0 */, const char * name)
    : ADN_TextEdit_ABC( parent,name )
{
    pConnector_ = new Connector( this );
    assert( pConnector_ != 0 );
}

//-----------------------------------------------------------------------------
// Name: ADN_TextEdit destructor
// Created: JSR 21-11-11
//-----------------------------------------------------------------------------
template< class Connector >
ADN_TextEdit< Connector >::~ADN_TextEdit()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_TextEdit::TextChanged
// Created: JSR 21-11-11
//-----------------------------------------------------------------------------
template< class Connector >
void ADN_TextEdit< Connector >::TextChanged()
{
    static_cast< Connector* >( pConnector_ )->SetDataChanged( text() );
    emit textChanged( text() );
}
