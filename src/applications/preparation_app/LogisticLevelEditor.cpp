// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LogisticLevelEditor.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticLevelEditor constructor
// Created: NLD 2010-11-25
// -----------------------------------------------------------------------------
LogisticLevelEditor::LogisticLevelEditor( QWidget* parent, const kernel::Entity_ABC& selected )
    : gui::ValuedComboBox< const LogisticLevel* >( parent )
{
    AddItem( QString( LogisticLevel::none_.GetName().c_str() ), &LogisticLevel::none_ );
    if( dynamic_cast< kernel::Automat_ABC* >( const_cast< kernel::Entity_ABC* >( &selected ) ) )
        AddItem( QString( LogisticLevel::tc2_.GetName().c_str() ), &LogisticLevel::tc2_ );
    if( dynamic_cast< kernel::Formation_ABC* >( const_cast< kernel::Entity_ABC* >( &selected ) ) )
        AddItem( QString( LogisticLevel::logistic_base_.GetName().c_str() ), &LogisticLevel::logistic_base_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLevelEditor::GetValue
// Created: NLD 2010-11-25
// -----------------------------------------------------------------------------
LogisticLevel* LogisticLevelEditor::GetValue()
{
    return const_cast< LogisticLevel* >( gui::ValuedComboBox< const LogisticLevel* >::GetValue() );
}
