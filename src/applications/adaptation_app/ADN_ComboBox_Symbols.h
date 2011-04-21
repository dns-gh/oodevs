// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ComboBox_Symbols_h_
#define __ADN_ComboBox_Symbols_h_

#include "ADN_ComboBox.h"
#include "ADN_ComboBoxItem.h"

// =============================================================================
/** @class  ADN_ComboBox_Symbols
    @brief  ADN_ComboBox_Symbols
*/
// Created: ABR 2011-04-19
// =============================================================================
template< typename T >
class ADN_ComboBox_Symbols : public ADN_ComboBox
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ComboBox_Symbols( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_ComboBox_Symbols();
    //@}

    //! @name Operations
    //@{
    virtual void insertItem ( ADN_ComboBoxItem* item, int index = -1 );
    //@}
};

// =============================================================================
/** @class  ADN_ComboBox_Symbols_Connector
    @brief  ADN_ComboBox_Symbols_Connector
*/
// Created: ABR 2011-04-19
// =============================================================================
template< typename T >
class ADN_ComboBox_Symbols_Connector : public ADN_Connector_ComboBox
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_ComboBox_Symbols_Connector( ADN_ComboBox_Symbols< T >& combo );
    virtual ~ADN_ComboBox_Symbols_Connector();
    //@}

    //! @name Operations
    //@{
    virtual ADN_ComboBoxItem* CreateItem( void * obj );
    //@}
};

// =============================================================================
/** @class  ADN_ComboBoxSymbolItem
    @brief  ADN_ComboBoxSymbolItem
*/
// Created: ABR 2011-04-19
// =============================================================================
template< typename T >
class ADN_ComboBoxSymbolItem : public ADN_ComboBoxItem
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ComboBoxSymbolItem( ADN_ComboBox& combo, void *data );
    virtual ~ADN_ComboBoxSymbolItem();
    //@}

    //! @name Operations
    //@{
    virtual void setText( const QString& txt );
    //@}
};

#include "ADN_ComboBox_Symbols.inl"

#endif // __ADN_ComboBox_Symbols_h_
