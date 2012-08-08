// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __RichSpinBox_h_
#define __RichSpinBox_h_

#include <boost/noncopyable.hpp>
#include <QtGui/QSpinBox>
#include <QtGui/QDoubleSpinBox>
#include "QtGui/QValidator.h"
#include <QtCore/QLocale>

namespace gui
{

// =============================================================================
/** @class  RichSpinBox
    @brief  RichSpinBox
*/
// Created: ABR 2012-04-05
// =============================================================================
class RichSpinBox : public QSpinBox
                  , private boost::noncopyable
{
public:
    explicit RichSpinBox( QWidget* parent = 0, int minValue = 0, int maxValue = std::numeric_limits< int >::max(), int step = 1 )
        : QSpinBox( parent )
    {
        if( !parent )
            setLocale( QLocale() ); // Get the default locale
        setRange( minValue, maxValue );
        setSingleStep( step );
    }

    void focusOutEvent( QFocusEvent* event )
    {
        QSpinBox::focusOutEvent( event );
        setValue( value() );
    }

    QString textFromValue( int value ) const
    {
        return locale().toString( value );
    }

    virtual QValidator::State validate( QString& input, int& pos ) const
    {
        QLocale locale;
        input = input.remove( suffix() );
        int sizeBefore = input.size();
        input = input.remove( locale.groupSeparator() );
        QValidator::State result = QValidator::Acceptable;

        if( input.contains( ',' ) || input.contains( '.' ) )
            result = QValidator::Invalid;
        else
        {
            bool ok = true;
            input.toLongLong( &ok );
            if( !ok )
                result = QValidator::Invalid;
        }
        if( result == QValidator::Acceptable )
        {
            int value = input.toInt();
            input = locale.toString( value );
            int sizeAfter = input.size();
            pos += ( sizeAfter - sizeBefore );
            pos = ( pos > input.length() ) ? input.length() : ( pos < 0 ) ? 0 : pos;
            result = QSpinBox::validate( input, pos );
        }
        return result;
    }
};

// =============================================================================
/** @class  RichDoubleSpinBox
    @brief  RichDoubleSpinBox
*/
// Created: ABR 2012-04-05
// =============================================================================
class RichDoubleSpinBox : public QDoubleSpinBox
                        , private boost::noncopyable
{
public:
    RichDoubleSpinBox( QWidget* parent = 0, double minValue = 0, double maxValue = std::numeric_limits< double >::max(), double step = 1, int decimals = 2, const QString separator = "." )
        : QDoubleSpinBox( parent )
        , tolleredSeparator_( separator )
    {
        if( !parent )
            setLocale( QLocale() ); // Get the default locale
        setRange( minValue, maxValue );
        setSingleStep( step );
        setDecimals( decimals );
    }

    virtual void focusOutEvent( QFocusEvent* event )
    {
        QDoubleSpinBox::focusOutEvent( event );
        setValue( value() );
    }

    virtual QString textFromValue( double value ) const
    {
        return locale().toString( value, 'f', decimals() );
    }

    virtual double valueFromText( const QString& text ) const
    {
        QString copy = text;
        return QDoubleSpinBox::valueFromText( copy.replace( tolleredSeparator_, locale().decimalPoint() ) );
    }

    virtual void fixup( QString& input ) const
    {
        input = input.replace( tolleredSeparator_, locale().decimalPoint() );
        QDoubleSpinBox::fixup( input );
    }

    virtual QValidator::State validate( QString& input, int& pos ) const
    {
        QLocale locale;
        input = input.replace( tolleredSeparator_, locale.decimalPoint() );
        input = input.remove( suffix() );
        int sizeBefore = input.size();
        input = input.remove( locale.groupSeparator() );
        input = input.replace( ',', locale.decimalPoint() );
        input = input.replace( '.', locale.decimalPoint() );
        QValidator::State result = QValidator::Acceptable;
        int d = decimals();
        int decimalPointIndex = input.find( locale.decimalPoint() );
        int trailingChars = ( decimalPointIndex == -1 ) ? d + 1 : d;

        QRegExp empty( QString( " *-?" ) + locale.decimalPoint() + "? *" );
        if( input.stripWhiteSpace() == locale.decimalPoint() )
            result = QValidator::Invalid;
        else if( empty.exactMatch( input ) )
            result = QValidator::Intermediate;
        else
        {
            bool ok = false;
            locale.toDouble( input, &ok );
            if( !ok )
                result = QValidator::Invalid;
            else if( decimalPointIndex >= 0 )
            {
                if( d == 0 )
                    result = QValidator::Invalid;
                else    // has decimal point, now count digits after that
                {
                    int j = decimalPointIndex + 1;
                    while( input[ j ].isDigit() )
                    {
                        ++j;
                        --trailingChars;
                    }
                    if( j - decimalPointIndex - 1 > d )
                        result = QValidator::Invalid;
                }
            }
        }
        if( result == QValidator::Acceptable )
        {
            double value = input.toDouble();
            input = locale.toString( value, 'f', d );
            input.truncate( input.size() - trailingChars );
            int sizeAfter = input.size();
            pos += ( sizeAfter - sizeBefore );
            pos = ( pos > input.length() ) ? input.length() : ( pos < 0 ) ? 0 : pos;
            result = QDoubleSpinBox::validate( input, pos );
        }
        return result;
    }

protected:
    const QString tolleredSeparator_;
};

} // !namespace gui

#endif // __RichSpinBox_h_
