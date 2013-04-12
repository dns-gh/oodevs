// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticLevelEditor_h__
#define __LogisticLevelEditor_h__

#include "clients_gui/ValuedComboBox.h"
#include "clients_gui/ValueEditor.h"

namespace kernel {
    class Entity_ABC;
    class LogisticLevel;
}

// =============================================================================
/** @class  LogisticLevelEditor
    @brief  LogisticLevelEditor
*/
// Created: NLD 2010-11-25
// =============================================================================
class LogisticLevelEditor : public gui::ValuedComboBox< const kernel::LogisticLevel* >
                          , public gui::ValueEditor< kernel::LogisticLevel* >
{
public:
    LogisticLevelEditor( QWidget* parent, const kernel::Entity_ABC& selected );
    virtual ~LogisticLevelEditor() {}

    virtual kernel::LogisticLevel* GetValue();
};

#endif // __LogisticLevelEditor_h__