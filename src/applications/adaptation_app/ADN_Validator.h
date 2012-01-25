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

#include "ADN_Types.h"

template< typename BaseType >
class ADN_Validator_ABC
{
public:
    ADN_Validator_ABC() {}
    ~ADN_Validator_ABC() {}

protected:
    virtual QValidator::State InternalValidate( BaseType top, BaseType bottom, QString& input, int& nPos ) const = 0;
};

// =============================================================================
/** @class ADN_IntValidator
    @brief
*/
// Created: APE 2005-01-03
// =============================================================================
class ADN_IntValidator : public QIntValidator
                       , public ADN_Validator_ABC< int >
{
public:
    explicit ADN_IntValidator( QObject* parent );
             ADN_IntValidator( int bottom, int top, QObject * parent );
    virtual ~ADN_IntValidator();

    void fixup( QString& strInput ) const;
    virtual QValidator::State validate( QString& input, int& nPos ) const;

protected:
    virtual QValidator::State InternalValidate( int top, int bottom, QString& input, int& nPos ) const;
};

// =============================================================================
/** @class ADN_DoubleValidator
    @brief
*/
// Created: APE 2005-01-03
// =============================================================================
class ADN_DoubleValidator : public QDoubleValidator
                          , public ADN_Validator_ABC< double >
{
public:
    explicit ADN_DoubleValidator( QObject* parent );
             ADN_DoubleValidator( double bottom, double top, int decimals, QObject * parent );
    virtual ~ADN_DoubleValidator();

    void fixup( QString& strInput ) const;
    virtual QValidator::State validate( QString& input, int& nPos ) const;

protected:
    virtual QValidator::State InternalValidate( double top, double bottom, QString& input, int& nPos ) const;
};


// =============================================================================
/** @class  ADN_PercentageValidator
    @brief
*/
// Created: APE 2005-01-03
// =============================================================================
template< typename BaseType, typename BaseValidator >
class ADN_PercentageValidator : public BaseValidator
{
public:
    explicit ADN_PercentageValidator( QObject* pParent );
    virtual ~ADN_PercentageValidator();

    void AddLinkedValue( ADN_Type_ABC< BaseType >& value );
    void ClearLinkedValues();

    void fixup( QString& strInput ) const;
    virtual QValidator::State validate( QString& input, int& nPos ) const;

private:
    typedef std::vector< ADN_Type_ABC< BaseType >* >    T_ValuesVector;
    typedef typename T_ValuesVector::const_iterator   CIT_ValuesVector;

private:
    T_ValuesVector linkedValues_;
};

// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::AddLinkedValue
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
template< typename BaseType, typename BaseValidator >
void ADN_PercentageValidator< BaseType, BaseValidator >::AddLinkedValue( ADN_Type_ABC< BaseType >& value )
{
    linkedValues_.push_back( &value );
}

// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::ClearLinkedValues
// Created: MMC 2011-03-29
// -----------------------------------------------------------------------------
template< typename BaseType, typename BaseValidator >
void ADN_PercentageValidator< BaseType, BaseValidator >::ClearLinkedValues()
{
    linkedValues_.clear();
}

typedef ADN_PercentageValidator< int, ADN_IntValidator >       ADN_IntPercentageValidator;
typedef ADN_PercentageValidator< double, ADN_DoubleValidator > ADN_DoublePercentageValidator;

#endif // __ADN_Validator_h_
