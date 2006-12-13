// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ListView_FragOrderTypes_h_
#define __ADN_ListView_FragOrderTypes_h_

#include "ADN_ListView.h"
#include "ADN_Missions_Data.h"

// =============================================================================
/** @class  ADN_ListView_FragOrderTypes
    @brief  ADN_ListView_FragOrderTypes
*/
// Created: SBO 2006-12-06
// =============================================================================
class ADN_ListView_FragOrderTypes : public ADN_ListView
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ListView_FragOrderTypes( ADN_Missions_Data::T_FragOrder_Vector& orders, QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_ListView_FragOrderTypes();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ADN_ListView_FragOrderTypes( const ADN_ListView_FragOrderTypes& );            //!< Copy constructor
    ADN_ListView_FragOrderTypes& operator=( const ADN_ListView_FragOrderTypes& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ADN_Missions_Data::T_FragOrder_Vector& orders_;
    //@}
};

#endif // __ADN_ListView_FragOrderTypes_h_
