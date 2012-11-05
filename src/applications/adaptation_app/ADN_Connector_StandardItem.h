// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Connector_StandardItem_h_
#define __ADN_Connector_StandardItem_h_

#include "ADN_Connector_ABC.h"
#include "ADN_StandardItem.h"

// =============================================================================
/** @class  ADN_Connector_StandardItem
    @brief  ADN_Connector_StandardItem
*/
// Created: ABR 2012-10-30
// =============================================================================
class ADN_Connector_StandardItem : public ADN_Connector_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Connector_StandardItem( ADN_StandardItem& item, ADN_StandardItem::E_Type type, const QStringList* enumContent = 0 );
    virtual ~ADN_Connector_StandardItem();
    //@}

    //! @name Operations
    //@{
    void SetDataPrivate( void* data );
    void SetDataChanged( const QString& text );
    //@}

private:
    //! @name Helpers
    //@{
    ADN_Connector_StandardItem& operator=( const ADN_Connector_StandardItem& );
    //@}

private:
    //! @name StandardItem
    //@{
    ADN_StandardItem& item_;
    ADN_StandardItem::E_Type type_;
    const QStringList* enumContent_;
    //@}
};

#endif // __ADN_Connector_StandardItem_h_
