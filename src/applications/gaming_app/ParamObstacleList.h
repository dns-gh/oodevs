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

#include "game_asn/Asn.h"
#include "Param_ABC.h"

namespace kernel
{
    class ActionController;
    class ObjectTypes;
    class CoordinateConverter_ABC;
    class OrderParameter;
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
class ParamObstacleList : public QObject, public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamObstacleList( QObject* parent, const kernel::OrderParameter& parameter, const kernel::ObjectTypes& objectTypes, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& controller );
    virtual ~ParamObstacleList();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity();
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual void CommitTo( Action_ABC& action ) const;
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void BuildInterface( QWidget* parent );
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
    const kernel::OrderParameter& parameter_;
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::ActionController& controller_;
    const kernel::ObjectTypes& objectTypes_;
    gui::ParametersLayer& layer_;
    QVBox* box_;
    ParamListView* list_;
    gui::ValuedListItem* selected_;
    QPopupMenu* popup_;
    //@}
};

#endif // __ParamObstacleList_h_
