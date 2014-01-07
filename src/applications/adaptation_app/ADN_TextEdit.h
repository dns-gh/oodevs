// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_TextEdit_h_
#define __ADN_TextEdit_h_

#include "ADN_Connector_String.h"
#include "ADN_TextEdit_ABC.h"

class ADN_Connector_TextEdit_String : public ADN_Connector_String< ADN_TextEdit_ABC >
{
public:
    ADN_Connector_TextEdit_String( ADN_TextEdit_ABC* gfx )
        : ADN_Connector_String< ADN_TextEdit_ABC >( gfx )
    {
        // NOTHING
    }

    virtual void SetDataPrivate( void *data )
    {
        assert( data );
        std::string* pTxt= static_cast< std::string* >( data );
        if( pGfx_->toPlainText() != pTxt->c_str() )
            pGfx_->setPlainText( pTxt->c_str() );
        pGfx_->setEnabled( ShouldEnableGfx() );
    }
};

class ADN_Connector_TextEdit_LocalizedString : public ADN_Connector_LocalizedString< ADN_TextEdit_ABC >
{
public:
    ADN_Connector_TextEdit_LocalizedString( ADN_TextEdit_ABC* gfx )
        : ADN_Connector_LocalizedString< ADN_TextEdit_ABC >( gfx )
    {
        // NOTHING
    }

    virtual void SetDataPrivate( void *data )
    {
        assert( data );
        std::string* pTxt= static_cast< std::string* >( data );
        if( pGfx_->toPlainText() != pTxt->c_str() )
            pGfx_->setPlainText( pTxt->c_str() );
        pGfx_->setEnabled( ShouldEnableGfx() );
    }
};

//*****************************************************************************
// Created: JSR 21-11-11
//*****************************************************************************
template < class Connector >
class ADN_TextEdit : public ADN_TextEdit_ABC
{
public:
    explicit ADN_TextEdit( QWidget* parent = 0, const char * name = 0 )
        : ADN_TextEdit_ABC( parent,name )
    {
        pConnector_.reset( new Connector( this ) );
    }
    virtual ~ADN_TextEdit()
    {
        // NOTHING
    }
};

typedef ADN_TextEdit< ADN_Connector_TextEdit_String >           ADN_TextEdit_String;
typedef ADN_TextEdit< ADN_Connector_TextEdit_LocalizedString >   ADN_TextEdit_LocalizedString;

#endif // __ADN_TextEdit_h_