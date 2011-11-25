// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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

// =============================================================================
/** @class  ParamBool
    @brief  ParamBool
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamBool : public QObject
                , public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamBool( QObject* parent, const kernel::OrderParameter& parameter, bool defaultValue = false );
    virtual ~ParamBool();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClicked();
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    bool value_;
    //@}
};

    }
}

#endif // __ParamBool_h_
