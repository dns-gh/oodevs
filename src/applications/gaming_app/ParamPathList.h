// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPathList_h_
#define __ParamPathList_h_

#include "gaming/ASN_Types.h"
#include "Param_ABC.h"
#include "ParamListView.h"

class ParametersLayer;
class CoordinateConverter_ABC;
class Entity_ABC;
class ValuedListItem;
class ActionController;

// =============================================================================
/** @class  ParamPathList
    @brief  ParamPathList
*/
// Created: SBO 2006-06-28
// =============================================================================
class ParamPathList : public QVBox, public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamPathList( QWidget* parent, ASN1T_ListItineraire& asnPathList, const std::string& label, ParametersLayer& layer, const CoordinateConverter_ABC& converter, const Entity_ABC& agent, ActionController& controller );
    virtual ~ParamPathList();
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
    void NewPath();
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
    ParametersLayer& layer_;
    const CoordinateConverter_ABC& converter_;
    ActionController& controller_;
    const Entity_ABC& agent_;
    ASN1T_ListItineraire& asn_;
    ASN1T_Itineraire* paths_;

    ParamListView* listView_;
    ValuedListItem* selected_;
    QPopupMenu* popup_;
    //@}
};

#endif // __ParamPathList_h_
