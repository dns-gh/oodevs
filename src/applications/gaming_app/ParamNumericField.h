// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamNumericField_h_
#define __ParamNumericField_h_

#include "game_asn/Asn.h"
#include "Param_ABC.h"

namespace kernel
{
    class OrderParameter;
}

namespace gui
{
    class RichLabel;
}

// =============================================================================
/** @class  ParamNumericField
    @brief  ParamNumericField
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamNumericField : public Param_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamNumericField( const kernel::OrderParameter& parameter, bool isReal );
             ParamNumericField( const QString& name, bool isReal );
    virtual ~ParamNumericField();
    //@}

    //! @name Operations
    //@{
    virtual void Show();
    virtual void Hide();
    virtual void BuildInterface( QWidget* parent );

    virtual bool CheckValidity();
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    void CommitTo( ASN1REAL& asn ) const;
    void SetLimits( float min, float max );
    virtual void CommitTo( Action_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamNumericField( const ParamNumericField& );            //!< Copy constructor
    ParamNumericField& operator=( const ParamNumericField& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter* parameter_;
    bool isReal_;
    gui::RichLabel* pLabel_;
    QLineEdit* pEdit_;
    //@}
};

#endif // __ParamNumericField_h_
