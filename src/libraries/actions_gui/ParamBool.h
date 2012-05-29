// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ParamBool_h_
#define __ParamBool_h_

#include "Param_ABC.h"

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
/** @class  ParamBool
    @brief  ParamBool
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamBool : public Param_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ParamBool( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamBool();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    void SetValue( bool value );
    virtual void Show() { if( group_ ) group_->show(); }
    virtual void Hide() { if( group_ ) group_->hide(); }
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
    bool       value_;
    //@}
};

    }
}

#endif // __ParamBool_h_
