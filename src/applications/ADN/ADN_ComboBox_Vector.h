// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-05 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ComboBox_Vector.h $
// $Author: Ape $
// $Modtime: 10/02/05 17:45 $
// $Revision: 3 $
// $Workfile: ADN_ComboBox_Vector.h $
//
// *****************************************************************************

#ifndef __ADN_ComboBox_Vector_h_
#define __ADN_ComboBox_Vector_h_

#include "ADN_ComboBox.h"
#include "ADN_ComboBoxItem.h"


// =============================================================================
/** @class  ADN_ComboBox_Vector
    @brief  ADN_ComboBox_Vector
    @par    Using example
    @code
    ADN_ComboBox_Vector;
    @endcode
*/
// Created: APE 2005-01-05
// =============================================================================
template< typename VectorType >
class ADN_ComboBox_Vector
: public ADN_ComboBox
{
    MT_COPYNOTALLOWED( ADN_ComboBox_Vector )

//public:
//typedef ADN_ComboBoxItem T_ItemType;

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ComboBox_Vector( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_ComboBox_Vector();
    //@}
};

template< typename AType >
class ADN_CBV_Connector
    : public ADN_Connector_ComboBox
{
public:
    ADN_CBV_Connector( ADN_ComboBox_Vector<AType>& combo )
        : ADN_Connector_ComboBox( &combo )
    {}

    virtual ~ADN_CBV_Connector()
    {}

    ADN_ComboBoxItem* CreateItem( void * obj )
    {
        // Create the new combo item.
        ADN_ComboBoxItem* pItem = new ADN_ComboBoxItem(*pCombo_,obj);

        // Connect it with the object's name.
        pItem->GetConnector().Connect( &(static_cast<AType*>(obj))->strName_ );

        return pItem; 
    };
};

#include "ADN_ComboBox_Vector.inl"

#endif // __ADN_ComboBox_Vector_h_
