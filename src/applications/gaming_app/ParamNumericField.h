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

#include "gaming/ASN_Types.h"
#include "Param_ABC.h"

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
class ParamNumericField : public QHBox, public Param_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamNumericField( QWidget* parent, ASN1INT& asn, const QString& label, int min = 0, int max = 9999 );
             ParamNumericField( QWidget* parent, ASN1REAL& asn, const QString& label, float min = 0., float max = 9999., int precision = 2 );
    virtual ~ParamNumericField();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity();
    virtual void Commit();
    void CommitTo( ASN1INT& asn );
    void CommitTo( ASN1REAL& asn );
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
    ASN1INT*  int_;
    ASN1REAL* real_;
    gui::RichLabel* pLabel_;
    QLineEdit* pEdit_;
    //@}
};

#endif // __ParamNumericField_h_
