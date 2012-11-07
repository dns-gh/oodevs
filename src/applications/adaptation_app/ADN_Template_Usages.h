// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Template_Usages_h_
#define __ADN_Template_Usages_h_

#include "ADN_CommonGfx.h"
#include "ADN_Urban_Data.h"

// =============================================================================
/** @class  ADN_Template_Usages
    @brief  ADN template usages
*/
// Created: LGY 2011-09-20
// =============================================================================
class ADN_Template_Usages : public ADN_Table
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Template_Usages( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Template_Usages();
    //@}

private:
    //! @name Slots
    //@{
    virtual void OnContextMenu( const QPoint& point );
    virtual bool Contains( ADN_Urban_Data::AccommodationInfos& accommodation );
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // __ADN_Template_Usages_h_
