// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-29 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_ListView.h $
// $Author: Ape $
// $Modtime: 29/12/04 14:52 $
// $Revision: 1 $
// $Workfile: ADN_Connector_ListView.h $
//
// *****************************************************************************

#ifndef __ADN_Connector_ListView_h_
#define __ADN_Connector_ListView_h_

#include "ADN_Connector_ListView_ABC.h"
#include "ADN_StandardItem.h"
#include <boost/noncopyable.hpp>

class ADN_ListView;

// =============================================================================
/** @class  ADN_Connector_ListView
    @brief  ADN_Connector_ListView
    @par    Using example
    @code
    ADN_Connector_ListView;
    @endcode
*/
// Created: APE 2004-12-29
// =============================================================================
template< typename T >
class ADN_Connector_ListView : public ADN_Connector_ListView_ABC
                             , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Connector_ListView( ADN_ListView& list );
    virtual ~ADN_Connector_ListView();
    //@}

    //! @name Operations
    //@{
    ADN_StandardItem* CreateItem( void* pObj );
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_Connector_ListView constructor
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
template< typename T >
ADN_Connector_ListView<T>::ADN_Connector_ListView( ADN_ListView& list )
: ADN_Connector_ListView_ABC( list )
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_ListView destructor
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
template< typename T >
ADN_Connector_ListView<T>::~ADN_Connector_ListView()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_ListView::CreateItem
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
template< typename T >
ADN_StandardItem* ADN_Connector_ListView<T>::CreateItem( void* pObj )
{
    // Create the new list item.
    ADN_StandardItem* pItem = new ADN_StandardItem( pObj );

    // Connect list item with object's name
    pItem->Connect( & static_cast< T* >( pObj )->strName_ );

    return pItem;
}

#endif // __ADN_Connector_ListView_h_
