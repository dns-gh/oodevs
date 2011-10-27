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
class ADN_Template_Usages : public ADN_Table2
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Template_Usages( QWidget* parent = 0 );
    virtual ~ADN_Template_Usages();
    //@}

private:
    //! @name Slots
    //@{
    virtual void OnContextMenu( int row, int col, const QPoint& point );
    virtual bool Contains( ADN_Urban_Data::AccommodationInfos& accommodation );
    virtual void doValueChanged( int row, int col );
    void UpdateValidator();
    //@}
};

#endif // __ADN_Template_Usages_h_
