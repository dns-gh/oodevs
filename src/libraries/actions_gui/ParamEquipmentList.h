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
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual bool InternalCheckValidity() const;
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
    void Move( Q3ListView* from, Q3ListView* to );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver< kernel::EquipmentType >& resolver_;
    Q3ListView* baseList_;
    Q3ListView* list_;
    //@}
};
}
}

#endif // __ParamEquipmentList_h_
