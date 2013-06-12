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
#include "clients_kernel/OrderParameter.h"
#include "tools/Resolver.h"

#pragma warning( push, 0 )
#include <QtGui/qlistwidget.h>
#pragma warning( pop )

namespace kernel
{
    class EquipmentType;
}

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ParamEquipmentList
    @brief  ParamEquipmentList
*/
// Created: SBO 2005-09-27
// =============================================================================
class ParamEquipmentList : public Param_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ParamEquipmentList( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamEquipmentList();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual bool InternalCheckValidity() const;
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual void SetEntity( const kernel::Entity_ABC* entity );
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
    void Move( QListWidget* from, QListWidget* to );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::EquipmentType, unsigned long >& resolver_;
    const InterfaceBuilder_ABC& builder_;
    QListWidget* baseList_;
    QListWidget* list_;
    //@}
};
}
}

#endif // __ParamEquipmentList_h_
