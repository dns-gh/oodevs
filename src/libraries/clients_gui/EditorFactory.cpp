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
void EditorFactory::Call( const QString& value )
{
    QStringEditor* editor = new QStringEditor( parent_ );
    editor->setText( value );
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
                 NumberEditor( QWidget* parent ) : QSpinBox( parent ) {}
        virtual ~NumberEditor() {}

        virtual T GetValue()
        {
            return value();
        }

    };
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void EditorFactory::Call( const double& value )
{
    NumberEditor< double >* editor = new NumberEditor< double >( parent_ );
    editor->setValue( value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void EditorFactory::Call( const float& value )
{
    NumberEditor< float >* editor = new NumberEditor< float >( parent_ );
    editor->setValue( value );
    result_ = editor;
}

// -----------------------------------------------------------------------------
// Name: EditorFactory::Call
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void EditorFactory::Call( const int& value )
{
    NumberEditor< int >* editor = new NumberEditor< int >( parent_ );
    editor->setValue( value );
    result_ = editor;
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
void EditorFactory::Call( const bool& value )
{
    BooleanEditor* editor = new BooleanEditor( parent_ );
    editor->SetCurrentItem( value );
    result_ = editor;
}
