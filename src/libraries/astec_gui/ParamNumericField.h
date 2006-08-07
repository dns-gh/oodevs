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

#include "astec_gaming/ASN_Types.h"
#include "Param_ABC.h"

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
             ParamNumericField( QWidget* parent, ASN1INT& asn, const std::string& label );
             ParamNumericField( QWidget* parent, ASN1REAL& asn, const std::string& label );
    virtual ~ParamNumericField();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
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
    QLabel* pLabel_;
    QLineEdit* pEdit_;
    //@}
};

#endif // __ParamNumericField_h_
