// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamLimaList_h_
#define __ParamLimaList_h_

#include "Param_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class TacticalLine_ABC;
    class OrderParameter;
}

class Lima;

// =============================================================================
/** @class  ParamLimaList
    @brief  Lima Parameter
*/
// Created: SBO 2006-11-14
// =============================================================================
class ParamLimaList : public QObject
                    , public Param_ABC
                    , public kernel::ElementObserver_ABC< Lima >
                    , public kernel::ContextMenuObserver_ABC< kernel::TacticalLine_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamLimaList( QObject* parent, const kernel::OrderParameter& parameter );
    virtual ~ParamLimaList();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity();
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void BuildInterface( QWidget* parent );
    virtual void CommitTo( ASN1T_OrderContext& asn ) const;
    virtual void Clean( ASN1T_OrderContext& asn ) const;
    virtual void CommitTo( Action_ABC& action ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void OnRequestPopup( QListViewItem* pItem, const QPoint& pos );
    void OnDeleteSelectedItem();
    void OnClearList();
    void TurnHeaderBlack();
    void MenuItemValidated( int function );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamLimaList( const ParamLimaList& );            //!< Copy constructor
    ParamLimaList& operator=( const ParamLimaList& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::TacticalLine_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyUpdated( const Lima& ) {};
    virtual void NotifyDeleted( const Lima& entity );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter& parameter_;
    QListView* list_;
    QPopupMenu* pPopupMenu_;
    const Lima* potential_;
    //@}
};

#endif // __ParamLimaList_h_
