// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Validator_h_
#define __ADN_Validator_h_

#include <qvalidator.h>
#include "ADN_Types.h"

// =============================================================================
/** @class  ADN_PercentageValidator
    @brief
*/
// Created: APE 2005-01-03
// =============================================================================
class ADN_PercentageValidator : public QDoubleValidator
{
public:
    explicit ADN_PercentageValidator( QObject* pParent, const char* szName = 0 );
    virtual ~ADN_PercentageValidator();

    void AddLinkedValue( ADN_Type_Double& value );

    void fixup( QString& strInput ) const;

private:
    typedef std::vector<ADN_Type_Double*>    T_ValuesVector;
    typedef T_ValuesVector::const_iterator CIT_ValuesVector;

    T_ValuesVector linkedValues_;
};

#endif // __ADN_Validator_h_
