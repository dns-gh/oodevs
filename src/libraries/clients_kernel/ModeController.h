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

#include "ENT/ENT_Tr_ABC.h"
#include "ModeController_ABC.h"
#include "ModesObserver_ABC.h"

class QMainWindow;

namespace kernel
{
    class Options;

// =============================================================================
/** @class  ModeController
    @brief  ModeController
*/
// Created: ABR 2012-05-09
// =============================================================================
template< typename EnumType >
class ModeController : public ModeController_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ModeController( QMainWindow* parent, EnumType savedMode, const QString& registeryEntry );
    virtual ~ModeController();
    //@}

    //! @name Operations
    //@{
    virtual int GetCurrentMode() const;
    virtual const QString& GetRegisteryEntry() const;
    //@}

private:
    //! @name Abstract operations
    //@{
    virtual void ChangeMode( int newMode );
    //@}

private:
    //! @name Helpers
    //@{
    void LoadGeometry();
    void SaveGeometry();
    //@}

private:
    //! @name Member data
    //@{
    QMainWindow*  parent_;
    const QString registeryEntry_;
    EnumType      currentMode_;
    EnumType      savedMode_;
    bool          useDefault_;
    bool          firstChangeToSavedMode_;
    //@}
};

#include "ModeController.inl"

} //! namespace kernel

#endif // __kernel_ModeController_h_
