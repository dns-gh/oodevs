// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ComboBox_Drawings_h_
#define __ADN_ComboBox_Drawings_h_

#include "ADN_ComboBox.h"
#include "ADN_ComboBoxItem.h"
#include "clients_gui/StandardIconProxyStyle.h"

// =============================================================================
/** @class  ADN_ComboBox_Drawings
    @brief  ADN_ComboBox_Drawings
*/
// Created: ABR 2011-04-19
// =============================================================================
template< typename T >
class ADN_ComboBox_Drawings : public ADN_ComboBox
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ComboBox_Drawings( QWidget* pParent = 0, int iconSize = 50 );
    virtual ~ADN_ComboBox_Drawings();
    //@}

    //! @name Operations
    //@{
    virtual void insertItem ( ADN_ComboBoxItem* item, int index = -1 );
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< QStyle > style_;
    //@}
};

// =============================================================================
/** @class  ADN_ComboBox_Drawings_Connector
    @brief  ADN_ComboBox_Drawings_Connector
*/
// Created: ABR 2011-04-19
// =============================================================================
template< typename T >
class ADN_ComboBox_Drawings_Connector : public ADN_Connector_ComboBox
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_ComboBox_Drawings_Connector( ADN_ComboBox_Drawings< T >& combo );
    virtual ~ADN_ComboBox_Drawings_Connector();
    //@}

    //! @name Operations
    //@{
    virtual ADN_ComboBoxItem* CreateItem( void * obj );
    //@}
};

// =============================================================================
/** @class  ADN_ComboBoxDrawingItem
    @brief  ADN_ComboBoxDrawingItem
*/
// Created: ABR 2011-04-19
// =============================================================================
template< typename T >
class ADN_ComboBoxDrawingItem : public ADN_ComboBoxItem
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ComboBoxDrawingItem( ADN_ComboBox& combo, void *data );
    virtual ~ADN_ComboBoxDrawingItem();
    //@}

    //! @name Operations
    //@{
    virtual void setText( const QString& txt );
    //@}
};

#include "ADN_ComboBox_Drawings.inl"

#endif // __ADN_ComboBox_Drawings_h_
