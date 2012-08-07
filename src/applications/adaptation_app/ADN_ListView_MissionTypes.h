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

enum E_EntityType;

class ADN_ListView_Models;

// =============================================================================
/** @class  ADN_ListView_MissionTypes
    @brief  ADN_ListView_MissionTypes
*/
// Created: SBO 2006-12-04
// =============================================================================
class ADN_ListView_MissionTypes : public ADN_ListView
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ListView_MissionTypes( E_EntityType eEntityType, ADN_Missions_Data::T_Mission_Vector& missions, QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_ListView_MissionTypes();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    bool ContextMenuDelete();
    std::string GetToolTipFor( Q3ListViewItem& item );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_ListView_MissionTypes( const ADN_ListView_MissionTypes& );            //!< Copy constructor
    ADN_ListView_MissionTypes& operator=( const ADN_ListView_MissionTypes& ); //!< Assignment operator
    //@}

signals:
    //! @name Copy/Assignment
    //@{
    void NotifyMissionDeleted( std::string name, E_EntityType type );
    //@}

private:
    //! @name Member data
    //@{
    ADN_Missions_Data::T_Mission_Vector& missions_;
    E_EntityType eEntityType_;
    //@}
};

#endif // __ADN_ListView_MissionTypes_h_
