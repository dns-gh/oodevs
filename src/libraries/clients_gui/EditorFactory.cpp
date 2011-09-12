// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::EditorFactory */

#include "clients_gui_pch.h"
#include "EditorFactory.h"
#include "clients_kernel/ValueEditor.h"
#include "clients_kernel/Units.h"
#include "ValuedComboBox.h"
#include "Tools.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EditorFactory constructor
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
EditorFactory::EditorFactory()
    : parent_( 0 )
    , result_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EditorFactory destructor
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
EditorFactory::~EditorFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::BeginEditor
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void EditorFactory::BeginEditor( QWidget* parent )
{
    parent_ = parent;
    result_ = 0;
    unit_ = 0;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::EndEditor
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
QWidget* EditorFactory::EndEditor()
{
    return result_;
}

// =============================================================================
// QString
// =============================================================================
namespace
{
    class QStringEditor : public QLineEdit
                        , public ValueEditor< QString >
    {
    public:
        explicit QStringEditor( QWidget* parent ) : QLineEdit( parent )
        {
            // NOTHING
        }

        virtual ~QStringEditor()
        {
            // NOTHING
        }

        virtual QString GetValue()
        {
            return text();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void EditorFactory::Call( QString* const& value )
{
    QStringEditor* editor = new QStringEditor( parent_ );
    editor->setText( *value );
    result_ = editor;
}

// =============================================================================
// Numbers
// =============================================================================
namespace
{
    class DecimalSpinBox : public QSpinBox
    {
    public:
        explicit DecimalSpinBox( QWidget* parent, unsigned short decimals = 0 )
             : QSpinBox( parent )
             , decimals_  ( decimals )
             , multiplier_( static_cast< unsigned int >( std::pow( 10.f, decimals ) ) )
        {
            setMinValue( 0 );
            setMaxValue( std::numeric_limits< int >::max() );
        }

        virtual ~DecimalSpinBox()
        {
            // NOTHING
        }

        virtual QValidator::State validate( QString& input, int& pos ) const
        {
            return QDoubleValidator( 0, std::numeric_limits< int >::max(), decimals_, 0 ).validate( input, pos );
        }

        virtual QString textFromValue( int value ) const
        {
            if( multiplier_ > 1 )
                return QString( "%1.%2" ).arg( value / multiplier_ ).arg( value % multiplier_ );
            return QString::number( value );
        }

        virtual int valueFromText( const QString& text ) const
        {
            QString textValue = text.mid( prefix().length(), text.length() - prefix().length() - suffix().length() );
            const char* ascii = textValue.ascii();
            if( !ascii )
                return 0;
            bool ok = true;
            if( multiplier_ > 1 )
                return static_cast< int >( textValue.toDouble( &ok ) * multiplier_ );
            return textValue.toInt( &ok );
        }

    protected:
        template< typename T >
        T DecimalValue()
        {
            return static_cast< T >( value() ) / static_cast< T >( multiplier_ );
        }

        template< typename T >
        void SetValue( const T& value )
        {
            setValue( static_cast< int >( value * static_cast< T >( multiplier_ ) + 0.5 ) );
        }

    private:
        int decimals_;
        unsigned int multiplier_;
    };

    template< typename T >
    class NumberEditor : public DecimalSpinBox
                       , public ValueEditor< T >
    {
    public:
        NumberEditor( QWidget* parent, const T& value, unsigned short decimals = 0 )
             : DecimalSpinBox( parent, decimals )
        {
             SetValue( value );
        }

        virtual ~NumberEditor() {}

        virtual T GetValue()
        {
            return DecimalValue< T >();
        }

    };

    template< typename T >
    class UnitEditor : public DecimalSpinBox
                     , public ValueEditor< UnitedValue< T > >
    {
    public:
         UnitEditor( QWidget* parent, const UnitedValue< T >& value, unsigned short decimals = 0 )
             : DecimalSpinBox( parent, decimals )
             , unit_( value.unit_ )
         {
             setSuffix( value.unit_.AsString() );
             SetValue( value.value_ );
         }
        virtual ~UnitEditor() {}

        virtual UnitedValue< T > GetValue()
        {
            return UnitedValue< T >( DecimalValue< T >(), unit_ );
        }

    private:
        Unit unit_;
    };

    template< typename T >
    QWidget* CreateNumberEditor( QWidget* parent, const T& value, const Unit* unit, unsigned short decimals = 0 )
    {
        if( unit )
            return new UnitEditor< T >( parent, UnitedValue< T >( value, *unit ), decimals );
        return new NumberEditor< T >( parent, value, decimals );
    }
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void EditorFactory::Call( double* const& value )
{
    result_ = CreateNumberEditor( parent_, *value, unit_, 2 );
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void EditorFactory::Call( float* const& value )
{
    result_ = CreateNumberEditor( parent_, *value, unit_, 2 );
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void EditorFactory::Call( int* const& value )
{
    result_ = CreateNumberEditor( parent_, *value, unit_ );
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
void EditorFactory::Call( unsigned int* const& value )
{
    result_ = CreateNumberEditor( parent_, *value, unit_ );
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void EditorFactory::Call( Unit* const& value )
{
    unit_ = value;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: MGD 2011-02-23
// -----------------------------------------------------------------------------
void EditorFactory::Call( unsigned long* const& value )
{
    result_ = CreateNumberEditor( parent_, *value, unit_ );
}

// =============================================================================
// Boolean
// =============================================================================
namespace
{
    class BooleanEditor : public ValuedComboBox< bool >
                        , public ValueEditor< bool >
    {
    public:
        BooleanEditor( QWidget* parent )
            : ValuedComboBox< bool >( parent )
        {
            AddItem( tools::translate( "gui::EditorFactory", "True" ), true );
            AddItem( tools::translate( "gui::EditorFactory", "False" ), false );
        }
        virtual ~BooleanEditor() {}

        virtual bool GetValue()
        {
            return ValuedComboBox< bool >::GetValue();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void EditorFactory::Call( bool* const& value )
{
    BooleanEditor* editor = new BooleanEditor( parent_ );
    editor->SetCurrentItem( *value );
    result_ = editor;
}
