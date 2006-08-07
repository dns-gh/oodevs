// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamObstacleList_h_
#define __ParamObstacleList_h_

#include "astec_gaming/ASN_Types.h"
#include "Param_ABC.h"

class ActionController;
class ParamListView;
class ObjectTypes;
class ParametersLayer;
class CoordinateConverter_ABC;
class ValuedListItem;

// =============================================================================
/** @class  ParamObstacleList
    @brief  ParamObstacleList
*/
// Created: APE 2004-05-18
// =============================================================================
class ParamObstacleList : public QVBox, public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamObstacleList( QWidget* parent, ASN1T_ListMissionGenObject& asnObjectList, const std::string& label, const ObjectTypes& objectTypes, ParametersLayer& layer, const CoordinateConverter_ABC& converter, ActionController& controller );
    virtual ~ParamObstacleList();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity();
    virtual void Commit();
    //@}
    
private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( QListViewItem* pItem );
    void OnRequestPopup( QListViewItem* pItem, const QPoint& pos, int col );
    void NewObstacle();
    void DeleteSelected();
    void ClearList();
    //@}

private:
    //! @name Helpers
    //@{
    void DeleteItem( QListViewItem& item );
    bool ChangeSelection();
    //@}

private:
    //! @name Member data
    //@{
    ActionController& controller_;
    const ObjectTypes& objectTypes_;
    ParametersLayer& layer_;
    const CoordinateConverter_ABC& converter_;
    
    ASN1T_ListMissionGenObject& asn_;
    ASN1T_MissionGenObject* objects_;
    ParamListView* listView_;
    ValuedListItem* selected_;
    QPopupMenu* popup_;
    //@}
};

#endif // __ParamObstacleList_h_
