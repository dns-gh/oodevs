// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ListView_MissionTypes_h_
#define __ADN_ListView_MissionTypes_h_

#include "ADN_ListView.h"
#include "ADN_Missions_Data.h"

// =============================================================================
/** @class  ADN_ListView_MissionTypes
    @brief  ADN_ListView_MissionTypes
*/
// Created: SBO 2006-12-04
// =============================================================================
class ADN_ListView_MissionTypes : public ADN_ListView
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ListView_MissionTypes( ADN_Missions_Data::T_Mission_Vector& missions, QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_ListView_MissionTypes();
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
    ADN_ListView_MissionTypes( const ADN_ListView_MissionTypes& );            //!< Copy constructor
    ADN_ListView_MissionTypes& operator=( const ADN_ListView_MissionTypes& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ADN_Missions_Data::T_Mission_Vector& missions_;
    //@}
};

#endif // __ADN_ListView_MissionTypes_h_
