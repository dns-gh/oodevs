// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_AnchorType.h"
#include "ADN_Tr.h"

namespace
{
    class ADN_Private_EnumConnector : public ADN_Connector_TableItem_EnumComboBox
    {
    public:
                 ADN_Private_EnumConnector( ADN_AnchorType& combo )
                     : ADN_Connector_TableItem_EnumComboBox( &combo )
        {
            // NOTHING
        }

        virtual ~ADN_Private_EnumConnector()
        {
            // NOTHING
        }

        virtual std::string GetItem( void* obj )
        {
            return ADN_Tr::ConvertFromAnchorType( *(E_AnchorType*)obj, ENT_Tr_ABC::eToTr );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_AnchorType constructor
// Created: LGY 2011-11-28
// -----------------------------------------------------------------------------
ADN_AnchorType::ADN_AnchorType( ADN_Table* parent, void* data, T_ConnectorVector& itemConnectors )
    : ADN_TableItem_ComboBox( parent, data )
    , itemConnectors_( itemConnectors )
{
    pConnector_ = new ADN_Private_EnumConnector( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_AnchorType destructor
// Created: LGY 2011-11-28
// -----------------------------------------------------------------------------
ADN_AnchorType::~ADN_AnchorType()
{
    delete pConnector_;
}
