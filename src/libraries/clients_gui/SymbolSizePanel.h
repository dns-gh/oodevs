// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef SymbolSizePanel_h
#define SymbolSizePanel_h

#include "PreferencePanel_ABC.h"

namespace kernel
{
    class OptionsController;
}

namespace gui
{
// =============================================================================
/** @class  SymbolSizePanel
    @brief  SymbolSizePanel
*/
// Created: SLI 2014-06-18
// =============================================================================
class SymbolSizePanel : public PreferencePanel_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SymbolSizePanel( QWidget* parent, kernel::OptionsController& controllers );
    virtual ~SymbolSizePanel();
    //@}
};

} //! namespace gui

#endif // SymbolSizePanel_h
