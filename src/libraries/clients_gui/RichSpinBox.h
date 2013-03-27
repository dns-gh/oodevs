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

#include <QtGui/QSpinBox>
#include <QtGui/QDoubleSpinBox>
#include "QtGui/QValidator.h"
#include <QtCore/QLocale>

#include "ObjectNameManager.h"

namespace gui
{

// =============================================================================
/** @class  RichSpinBox
    @brief  RichSpinBox
*/
// Created: ABR 2012-04-05
// =============================================================================
class RichSpinBox : public QSpinBox
{
public:
    explicit RichSpinBox( const QString& objectName, QWidget* parent = 0, int minValue = 0, int maxValue = std::numeric_limits< int >::max(), int step = 1 )
        : QSpinBox( parent )
    {
        if( !parent )
            setLocale( QLocale() ); // Get the default locale
        setRange( minValue, maxValue );
        setSingleStep( step );
        ObjectNameManager::getInstance()->SetObjectName( this, objectName );
    }

    ~RichSpinBox()
    {
        ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
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

    virtual QValidator::State validate( QString& input, int& nPos ) const
    {
        input = input.remove( suffix() );
        int sizeBefore = input.size();
        input = input.remove( locale().groupSeparator() );
        QValidator::State result = QValidator::Acceptable;

        if( input.contains( ',' ) || input.contains( '.' ) )
            result = QValidator::Invalid;
        else
        {
            bool ok = true;
            long long entered = input.toLongLong( &ok );
            if( !ok )
                result = QValidator::Invalid;
            else if( entered > maximum() )
                input = QString::number( maximum() );
            else if( entered < minimum() )
                input = QString::number( minimum() );
        }

        if( result == QValidator::Acceptable )
        {
            int value = input.toInt();
            input = locale().toString( value );
            int sizeAfter = input.size();
            nPos += ( sizeAfter - sizeBefore );
            nPos = ( nPos > input.length() ) ? input.length() : ( nPos < 0 ) ? 0 : nPos;
            result = QSpinBox::validate( input, nPos );
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
{
public:
    RichDoubleSpinBox( const QString& objectName, QWidget* parent = 0, double minValue = 0, double maxValue = std::numeric_limits< double >::max(), double step = 1, int decimals = 2 )
        : QDoubleSpinBox( parent )
        , tolleredSeparator_( "." )
    {
        if( !parent )
            setLocale( QLocale() ); // Get the default locale
        setRange( minValue, maxValue );
        setSingleStep( step );
        setDecimals( decimals );
        setObjectName( objectName );
    }

    ~RichDoubleSpinBox()
    {
        ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
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

    virtual QValidator::State validate( QString& input, int& nPos ) const
    {
        input = input.remove( suffix() );
        int sizeBefore = input.size();
        input = input.remove( locale().groupSeparator() );
        input = input.replace( tolleredSeparator_, locale().decimalPoint() );
        input = input.replace( ',', locale().decimalPoint() );

        QValidator::State result = QValidator::Acceptable;
        int d = decimals();
        int decimalPointIndex = input.find( locale().decimalPoint() );
        int trailingChars = ( decimalPointIndex == -1 ) ? d + 1 : d;

        QRegExp empty( QString( "\\s*-?\\" ) + locale().decimalPoint() + "?\\s*" );
        if( minimum() >= 0 && input.stripWhiteSpace().startsWith( '-' ) )
            result = QValidator::Invalid;
        else if( input.stripWhiteSpace() == locale().decimalPoint() )
            result = QValidator::Invalid;
        else if( empty.exactMatch( input ) )
            result = QValidator::Intermediate;
        else
        {
            bool ok = false;
            double entered = locale().toDouble( input, &ok );
            if( !ok )
                result = QValidator::Invalid;
            else if( decimalPointIndex >= 0 )
            {
                if( d == 0 )
                    result = QValidator::Invalid;
                else
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
            if( result != QValidator::Invalid )
            {
                if( entered > maximum() )
                {
                    input = QString::number( maximum(), 'f', d );
                    result = QValidator::Intermediate;
                }
                else
                    result = ( entered < minimum() ) ? QValidator::Intermediate : QValidator::Acceptable;
            }
        }

        if( result == QValidator::Acceptable )
        {
            double value = input.toDouble();
            input = locale().toString( value, 'f', d );
            input.truncate( input.size() - trailingChars );
            int sizeAfter = input.size();
            nPos += ( sizeAfter - sizeBefore );
            nPos = ( nPos > input.length() ) ? input.length() : ( nPos < 0 ) ? 0 : nPos;
            result = QDoubleSpinBox::validate( input, nPos );
        }
        return result;
    }

protected:
    const QString tolleredSeparator_;
};

} // !namespace gui

#endif // __RichSpinBox_h_
