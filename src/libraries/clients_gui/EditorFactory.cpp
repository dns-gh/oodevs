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
    unit_   = 0;
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
                 QStringEditor( QWidget* parent ) : QLineEdit( parent ) {}
        virtual ~QStringEditor() {}

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
    template< typename T >
    class NumberEditor : public QSpinBox
                       , public ValueEditor< T >
    {
    public:
                 NumberEditor( QWidget* parent, const T& value ) : QSpinBox( parent )
                 {
                     setValue( value );
                 }
        virtual ~NumberEditor() {}

        virtual T GetValue()
        {
            return value();
        }

    };

    template< typename T >
    class UnitEditor : public QSpinBox
                     , public ValueEditor< UnitedValue< T > >
    {
    public:
                 UnitEditor( QWidget* parent, const UnitedValue< T >& value ) : QSpinBox( parent ), unit_( value.unit_ )
                 {
                     setValue( value.value_ );
                     setSuffix( value.unit_ );
                 }
        virtual ~UnitEditor() {}

        virtual UnitedValue< T > GetValue()
        {
            return UnitedValue< T >( value(), unit_ );
        }
    private:
        Unit unit_;
    };

    template< typename T >
    QWidget* CreateNumberEditor( QWidget* parent, const T& value, const Unit* unit )
    {
        if( unit )
            return new UnitEditor< T >( parent, UnitedValue< T >( value, *unit ) );
        return new NumberEditor< T >( parent, value );
    }
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void EditorFactory::Call( double* const& value )
{
    result_ = CreateNumberEditor( parent_, *value, unit_ );
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void EditorFactory::Call( float* const& value )
{
    result_ = CreateNumberEditor( parent_, *value, unit_ );
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
void EditorFactory::Call( kernel::Unit* const& value )
{
    unit_ = value;
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
            AddItem( tools::translate( "EditorFactory", "True" ), true );
            AddItem( tools::translate( "EditorFactory", "False" ), false );
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
