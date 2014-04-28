// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ParamEquipmentType_h_
#define __ParamEquipmentType_h_

#include "ParamComboBox.h"

namespace kernel
{
    class EquipmentType;
    class OrderParameter;
}

namespace actions
{
namespace gui
{

// =============================================================================
/** @class  ParamEquipmentType
    @brief  Equipment type only
*/
// Created: SBO 2006-08-09
// =============================================================================
class ParamEquipmentType : public ParamComboBox< int >
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamEquipmentType( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamEquipmentType();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual void SetEntity( const kernel::Entity_ABC* entity );
    virtual void Visit( const actions::parameters::EquipmentType& param );
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool InternalCheckValidity() const;
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::EquipmentType >& resolver_;
    //@}
};

}
}

#endif // __ParamEquipmentType_h_
