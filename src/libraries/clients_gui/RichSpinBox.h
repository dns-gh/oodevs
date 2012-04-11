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
        input = input.replace( tolleredSeparator_, locale().decimalPoint() );
        QValidator::State result = QDoubleSpinBox::validate( input, pos );
        return result;
    }

protected:
    const QString tolleredSeparator_;
};

} // !namespace gui

#endif // __RichSpinBox_h_
