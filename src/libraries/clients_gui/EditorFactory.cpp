// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EditorFactory.h"
#include "DurationEditor.h"
#include "RichLineEdit.h"
#include "RichSpinBox.h"
#include "SubObjectName.h"
#include "ValuedComboBox.h"
#include "ValueEditor.h"

#include "clients_kernel/Tools.h"
#include "clients_kernel/Units.h"

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
    class QStringEditor : public RichLineEdit
                        , public ValueEditor< QString >
    {
    public:
        explicit QStringEditor( const QString& objectName, QWidget* parent ) : RichLineEdit( objectName, parent )
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
    QStringEditor* editor = new QStringEditor( "stringEditor", parent_ );
    editor->setText( *value );
    result_ = editor;
}

// =============================================================================
// QTime
// =============================================================================
namespace
{
    class LocalDurationEditor : public DurationEditor
                              , public ValueEditor< kernel::Duration >
    {
    public:
        explicit LocalDurationEditor( QWidget* parent )
            : DurationEditor( parent )
        {
            // NOTHING
        }

        virtual ~LocalDurationEditor()
        {
            // NOTHING
        }

        virtual kernel::Duration GetValue()
        {
            return value();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: LGY 2011-12-06
// -----------------------------------------------------------------------------
void EditorFactory::Call( kernel::Duration* const& value )
{
    LocalDurationEditor* editor = new LocalDurationEditor( parent_ );
    editor->setValue( *value );
    result_ = editor;
}

// =============================================================================
// QDateTime
// =============================================================================
namespace
{
    class QDateTimeEditor : public QDateTimeEdit
                          , public ValueEditor< QDateTime >
    {
    public:
        explicit QDateTimeEditor( QWidget* parent )
            : QDateTimeEdit( parent )
        {
            // NOTHING
        }

        virtual ~QDateTimeEditor()
        {
            // NOTHING
        }

        virtual QDateTime GetValue()
        {
            return dateTime();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: LGY 2012-11-20
// -----------------------------------------------------------------------------
void EditorFactory::Call( QDateTime* const& value )
{
    QDateTimeEditor* editor = new QDateTimeEditor( parent_ );
    editor->setDisplayFormat( "dd/MM/yy HH:mm" );
    editor->setDateTime( *value );
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
        NumberEditor( const QString& objectName, QWidget* parent, const T& value )
             : BaseSpinBox( objectName, parent )
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
        UnitEditor( const QString& objectName, QWidget* parent, const UnitedValue< T >& value )
             : BaseSpinBox( objectName, parent, static_cast< EditedType >( value.unit_.GetMinValue() ), static_cast< EditedType >( value.unit_.GetMaxValue() ) )
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
            return new UnitEditor< RichSpinBox, int, T >( "numberEditorSpinBox", parent, UnitedValue< T >( value, *unit ) );
        return new NumberEditor< RichSpinBox, int, T >( "numberEditorSpinBox", parent, value );
    }

    template< typename T >
    QWidget* CreateFloatingNumberEditor( QWidget* parent, const T& value, const Unit* unit, unsigned short decimals = 0 )
    {
        RichDoubleSpinBox* spinBox = 0;
        if( unit )
            spinBox = new UnitEditor< RichDoubleSpinBox, double, T >( "floatingSpinBox", parent, UnitedValue< T >( value, *unit ));
        else
            spinBox = new NumberEditor< RichDoubleSpinBox, double, T >( "FloatingSpinBox", parent, value );
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
        BooleanEditor( const QString& objectName, QWidget* parent )
            : ValuedComboBox< bool >( objectName, parent )
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
    BooleanEditor* editor = new BooleanEditor( "editor", parent_ );
    editor->SetCurrentItem( *value );
    result_ = editor;
}
