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
/** @class  ADN_IntValidator
    @brief  A better int validator than Qt's.
*/
// Created: APE 2005-01-03
// =============================================================================
class ADN_IntValidator : public QIntValidator
{
public:
    explicit ADN_IntValidator( QObject* pParent, const char* szName = 0 );
             ADN_IntValidator( int nBottom, int nTop, QObject* pParent, const char* szName = 0 );
    virtual ~ADN_IntValidator();

    QValidator::State validate( QString& strInput, int& nPos ) const;
    void fixup( QString& strInput ) const;
};

// =============================================================================
/** @class  ADN_DoubleValidator
    @brief  A better double validator than Qt's.
*/
// Created: APE 2005-01-03
// =============================================================================
class ADN_DoubleValidator : public QDoubleValidator
{
public:
    explicit ADN_DoubleValidator( QObject* pParent, const char* szName = 0 );
             ADN_DoubleValidator( double rBottom, double rTop, int nDecimals, QObject* pParent, const char* szName = 0 );
    virtual ~ADN_DoubleValidator();

    QValidator::State validate( QString& strInput, int& nPos ) const;
    void fixup( QString& strInput ) const;
};

// =============================================================================
/** @class  ADN_PercentageValidator
    @brief
*/
// Created: APE 2005-01-03
// =============================================================================
class ADN_PercentageValidator : public ADN_DoubleValidator
{
public:
    explicit ADN_PercentageValidator( QObject* pParent, const char* szName = 0 );
    virtual ~ADN_PercentageValidator();

    void AddLinkedValue( ADN_Type_Double& value );

    QValidator::State validate( QString& strInput, int& nPos ) const;
    void fixup( QString& strInput ) const;

private:
    typedef std::vector<ADN_Type_Double*>    T_ValuesVector;
    typedef T_ValuesVector::const_iterator CIT_ValuesVector;

    ADN_DoubleValidator doubleValidator_;
    T_ValuesVector linkedValues_;
};

#endif // __ADN_Validator_h_
