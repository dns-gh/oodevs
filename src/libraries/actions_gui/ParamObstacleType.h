// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ParamObstacleType_h_
#define __ParamObstacleType_h_

#include "Param_ABC.h"
#include "ENT/ENT_Enums.h"

namespace kernel
{
    class OrderParameter;
}

namespace actions
{
namespace gui
{
    class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ParamObstacleType
    @brief  ParamObstacleType
*/
// Created: LDC 2014-05-22
// =============================================================================
class ParamObstacleType : public Param_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ParamObstacleType( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamObstacleType();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual void Visit( const actions::parameters::ObstacleType& param );
    void SetValue( const QString& value );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCurrentIndexChanged( int index );
    //@}

private:
    //! @name Member data
    //@{
    QComboBox* comboBox_;
    E_ObstacleActivation activation_;
    //@}
};

}
}

#endif // __ParamObstacleType_h_
