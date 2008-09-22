// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamEquipmentList_h_
#define __ParamEquipmentList_h_

#include "Param_ABC.h"
#include "clients_kernel/Resolver.h"
#include "clients_kernel/OrderParameter.h"

namespace kernel
{
    class EquipmentType;
}

// =============================================================================
/** @class  ParamEquipmentList
    @brief  ParamEquipmentList
*/
// Created: SBO 2005-09-27
// =============================================================================
class ParamEquipmentList : public QObject, public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamEquipmentList( QObject* parent, const kernel::OrderParameter& parameter, const kernel::Resolver< kernel::EquipmentType >& resolver );
    virtual ~ParamEquipmentList();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnUp();
    void OnDown();
    void OnAdd();
    void OnRemove();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool IsOptional() const;
    void Move( QListView* from, QListView* to );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    const kernel::Resolver< kernel::EquipmentType >& resolver_;
    QListView* baseList_;
    QListView* list_;
    //@}
};

#endif // __ParamEquipmentList_h_
