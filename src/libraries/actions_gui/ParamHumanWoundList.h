// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamHumanWoundList_h_
#define __ParamHumanWoundList_h_

#include "Param_ABC.h"
#include "clients_kernel/OrderParameter.h"

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamHumanWoundList
    @brief  ParamHumanWoundList
*/
// Created: SBO 2005-09-27
// =============================================================================
class ParamHumanWoundList : public QObject
                          , public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamHumanWoundList( QObject* parent, const kernel::OrderParameter& parameter );
    virtual ~ParamHumanWoundList();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual bool InternalCheckValidity() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnUp();
    void OnDown();
    void OnAdd( int index );
    void OnRemove();
    void OnContextMenu( Q3ListViewItem* item, const QPoint& point, int col );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    Q3ListView* list_;
    //@}
};

    }
}

#endif // __ParamHumanWoundList_h_
