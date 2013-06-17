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

#include "QtGui/qtreeview.h"
#include "QtGui/qstandarditemmodel.h"

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ParamHumanWoundList
    @brief  ParamHumanWoundList
*/
// Created: SBO 2005-09-27
// =============================================================================
class ParamHumanWoundList : public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamHumanWoundList( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamHumanWoundList();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual bool InternalCheckValidity() const;
    virtual void Visit( const actions::parameters::MedicalPriorities& param );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnUp();
    void OnDown();
    void OnAdd( int index );
    void OnRemove();
    void OnContextMenu( const QPoint& point );
    //@}

private:
    //! @name Member data
    //@{
    QTreeView* list_;
    QStandardItemModel model_;
    //@}
};

    }
}

#endif // __ParamHumanWoundList_h_
