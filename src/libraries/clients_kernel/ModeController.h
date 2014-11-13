// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __kernel_ModeController_h_
#define __kernel_ModeController_h_

#include "ENT/ENT_Enums.h"
#include "Controllers.h"
#include <tools/SortedInterfaceContainer.h>
#include <tools/Observer_ABC.h>
#include <tools/Path.h>

class QMainWindow;

namespace kernel
{
    class OptionsController;

// =============================================================================
/** @class  ModeController
    @brief  ModeController
*/
// Created: ABR 2012-05-09
// =============================================================================
class ModeController : public tools::SortedInterfaceContainer< tools::Observer_ABC >
{
    friend void Controllers::ChangeMode( E_Modes newMode );

public:
    //! @name Constructors/Destructor
    //@{
             ModeController( OptionsController& options );
    virtual ~ModeController();
    //@}

    //! @name Accessors
    //@{
    void SetMainWindow( QMainWindow* parent );
    E_Modes GetCurrentMode() const;
    //@}

    //! @name Serialization
    //@{
    void LoadGeometry( E_Modes mode );
    void SaveGeometry( E_Modes mode );

    void LoadState( E_Modes mode );
    void SaveState( E_Modes mode );

    void LoadOptions( E_Modes mode );
    void SaveOptions( E_Modes mode );
    //@}

private:
    //! @name Helpers
    //@{
    void ChangeMode( E_Modes newMode );
    //@}

private:
    //! @name Member data
    //@{
    OptionsController& options_;
    QMainWindow* parent_;
    E_Modes currentMode_;
    bool firstChangeToSavedMode_;
    bool useDefault_;
    //@}
};

} //! namespace kernel

#endif // __kernel_ModeController_h_
