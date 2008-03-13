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

// =============================================================================
/** @class  ParamBool
    @brief  ParamBool
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamBool : public QObject, public Param_ABC
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
    virtual void BuildInterface( QWidget* parent );
    virtual void CommitTo( ActionParameterContainer_ABC& action ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClicked();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamBool( const ParamBool& );            //!< Copy constructor
    ParamBool& operator=( const ParamBool& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual bool IsOptional() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter& parameter_;
    bool value_;
    //@}
};

#endif // __ParamBool_h_
