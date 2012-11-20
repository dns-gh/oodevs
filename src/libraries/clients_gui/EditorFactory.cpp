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
#include "RichSpinBox.h"
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
// QTime
// =============================================================================
namespace
{
    class QTimeEditor : public QTimeEdit
                      , public ValueEditor< QTime >
    {
    public:
        explicit QTimeEditor( QWidget* parent )
            : QTimeEdit( parent )
        {
            // NOTHING
        }

        virtual ~QTimeEditor()
        {
            // NOTHING
        }

        virtual QTime GetValue()
        {
            return time();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: LGY 2011-12-06
// -----------------------------------------------------------------------------
void EditorFactory::Call( QTime* const& value )
{
    QTimeEditor* editor = new QTimeEditor( parent_ );
    editor->setDisplayFormat( "HH:mm:ss" );
    editor->setTime( *value );
    result_ = editor;
}

// =============================================================================
// Numbers
// =============================================================================
namespace
{
    template< typename BaseSpinBox, typename EditedType, typename T >
    class NumberEditor : public BaseSpinBox
                       , public ValueEditor< T >
    {
    public:
        NumberEditor( QWidget* parent, const T& value )
             : BaseSpinBox( parent )
        {
            setValue( static_cast< EditedType >( value ) );
        }

        virtual ~NumberEditor() {}

        virtual T GetValue()
        {
            return static_cast< T >( value() );
        }

    };

    template< typename BaseSpinBox, typename EditedType, typename T >
    class UnitEditor : public BaseSpinBox
                     , public ValueEditor< UnitedValue< T > >
    {
    public:
         UnitEditor( QWidget* parent, const UnitedValue< T >& value )
             : BaseSpinBox( parent, static_cast< EditedType >( value.unit_.GetMinValue() ), static_cast< EditedType >( value.unit_.GetMaxValue() ) )
             , unit_( value.unit_ )
         {
             setSuffix( value.unit_.AsString() );
             setValue( value.value_ );
         }
        virtual ~UnitEditor() {}

        virtual UnitedValue< T > GetValue()
        {
            return UnitedValue< T >( static_cast< T >( value() ), unit_ );
        }

    private:
        Unit unit_;
    };

    template< typename T >
    QWidget* CreateNumberEditor( QWidget* parent, const T& value, const Unit* unit )
    {
        if( unit )
            return new UnitEditor< RichSpinBox, int, T >( parent, UnitedValue< T >( value, *unit ) );
        return new NumberEditor< RichSpinBox, int, T >( parent, value );
    }

    template< typename T >
    QWidget* CreateFloatingNumberEditor( QWidget* parent, const T& value, const Unit* unit, unsigned short decimals = 0 )
    {
        RichDoubleSpinBox* spinBox = 0;
        if( unit )
            spinBox = new UnitEditor< RichDoubleSpinBox, double, T >( parent, UnitedValue< T >( value, *unit ));
        else
            spinBox = new NumberEditor< RichDoubleSpinBox, double, T >( parent, value );
        spinBox->setDecimals( ( unit ) ? unit->GetDecimal() : decimals );
        return spinBox;
    }
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void EditorFactory::Call( double* const& value )
{
    result_ = CreateFloatingNumberEditor( parent_, *value, unit_, 2 );
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void EditorFactory::Call( float* const& value )
{
    result_ = CreateFloatingNumberEditor( parent_, *value, unit_, 2 );
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
