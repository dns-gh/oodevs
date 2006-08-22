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

#include "gaming/ASN_Types.h"
#include "Param_ABC.h"

namespace kernel
{
    class ActionController;
    class ObjectTypes;
    class CoordinateConverter_ABC;
}

namespace gui
{
    class ParametersLayer;
    class ValuedListItem;
}

class ParamListView;

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
             ParamObstacleList( QWidget* parent, ASN1T_ListMissionGenObject& asnObjectList, const std::string& label, const kernel::ObjectTypes& objectTypes, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& controller );
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
    kernel::ActionController& controller_;
    const kernel::ObjectTypes& objectTypes_;
    gui::ParametersLayer& layer_;
    const kernel::CoordinateConverter_ABC& converter_;
    
    ASN1T_ListMissionGenObject& asn_;
    ASN1T_MissionGenObject* objects_;
    ParamListView* listView_;
    gui::ValuedListItem* selected_;
    QPopupMenu* popup_;
    //@}
};

#endif // __ParamObstacleList_h_
