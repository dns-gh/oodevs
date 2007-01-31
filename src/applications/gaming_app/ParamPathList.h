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

#include "game_asn/Asn.h"
#include "Param_ABC.h"
#include "ParamListView.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Entity_ABC;
    class ActionController;    
}

namespace gui
{
    class ParametersLayer;
    class ValuedListItem;
}

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
             ParamPathList( QWidget* parent, ASN1T_ListItineraire*& asnPathList, const QString& label, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, const kernel::Entity_ABC& agent, kernel::ActionController& controller );
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
    gui::ParametersLayer& layer_;
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::ActionController& controller_;
    const kernel::Entity_ABC& agent_;
    ASN1T_ListItineraire* asn_;
    ASN1T_Itineraire* paths_;

    ParamListView* listView_;
    gui::ValuedListItem* selected_;
    QPopupMenu* popup_;
    //@}
};

#endif // __ParamPathList_h_
