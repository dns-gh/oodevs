// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Equipments_Resources_ListView_h_
#define __ADN_Equipments_Resources_ListView_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_Equipments_Resources_ListView
    @brief  ADN_Equipments_Resources_ListView
*/
// Created: ABR 2012-12-03
// =============================================================================
class ADN_Equipments_Resources_ListView : public ADN_ListView
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Equipments_Resources_ListView( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* parent = 0 );
    virtual ~ADN_Equipments_Resources_ListView();
    //@}

private:
    //! @name Helpers
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    bool Contains( void* category );
    std::string GetToolTipFor( const QModelIndex& index );
    //@}
};

#endif // __ADN_Equipments_Resources_ListView_h_
