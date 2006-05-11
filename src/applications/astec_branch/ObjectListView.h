// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-08-05 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectListView.h $
// $Author: Ape $
// $Modtime: 26/08/04 11:03 $
// $Revision: 3 $
// $Workfile: ObjectListView.h $
//
// *****************************************************************************

#ifndef __ObjectListView_h_
#define __ObjectListView_h_

#include "ElementObserver_ABC.h"
#include "SelectionObserver_ABC.h"
#include "OptionsObserver_ABC.h"

class Object;
class Controllers;
class Team;
class ItemFactory_ABC;

// =============================================================================
/** @class  ObjectListView
    @brief  ObjectListView
*/
// Created: APE 2004-08-05
// =============================================================================
class ObjectListView : public QListView
                     , public Observer_ABC
                     , public ElementObserver_ABC< Object >
                     , public SelectionObserver_Base< Object >
                     , public OptionsObserver_ABC
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             ObjectListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~ObjectListView();
    //@}

    //! @name Operations
    //@{
     QSize sizeHint() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange( QListViewItem* item );
    void OnRequestCenter();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectListView( const ObjectListView& );
    ObjectListView& operator=( const ObjectListView );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Object& object );
    virtual void NotifyDeleted( const Object& object );
    virtual void Select( const Object& object );
    virtual void OptionChanged( const std::string& name, const OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    ItemFactory_ABC& factory_;
    QPopupMenu* pPopupMenu_;
    const Team* currentTeam_;
    //@}
};

#endif // __ObjectListView_h_
