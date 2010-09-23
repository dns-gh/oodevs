// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamDirection_h_
#define __ParamDirection_h_

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
/** @class  ParamDirection
    @brief  ParamDirection
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamDirection : public QObject, public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamDirection( QObject* parent, const kernel::OrderParameter& parameter );
    virtual ~ParamDirection();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged( int value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamDirection( const ParamDirection& );            //!< Copy constructor
    ParamDirection& operator=( const ParamDirection& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual bool IsOptional() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    int value_;
    //@}
};

    }
}

#endif // __ParamDirection_h_
