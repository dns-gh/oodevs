// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_StandardItem_h_
#define __ADN_StandardItem_h_

#include <QtGui/qstandarditem>
#include "ADN_Connector_Vector_ABC.h"

class ADN_Connector_ABC;
class ADN_Connector_StandardItem;

// =============================================================================
/** @class  ADN_StandardItem
    @brief  ADN_StandardItem
*/
// Created: ABR 2012-10-25
// =============================================================================
class ADN_StandardItem : public QStandardItem
{
public:
    //! @name Types
    //@{
    enum E_Type
    {
        eBool            = 0,
        eInt             = 1,
        eDouble          = 2,
        eEnum            = 3,
        eString          = 4,
        eLocalizedString = 5,
        ePtrInVector     = 6,
        eDelay           = 7,
        eTime            = 8,
        eColor           = 9
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_StandardItem( void* data, E_Type type = eString );
    virtual ~ADN_StandardItem();
    //@}

    //! @name Operations
    //@{
    void* GetData() const;
    ADN_Connector_ABC* GetConnector() const;
    E_Type GetType() const;
    void Connect( ADN_Connector_ABC* data, const QStringList* enumContent =  0  );
    //@}

private:
    //! @name Member data
    //@{
    void* pData_;
    ADN_Connector_ABC* connector_;
    E_Type type_;
    //@}
};

typedef ADN_StandardItem ADN_ListViewItem;

#endif // __ADN_StandardItem_h_
