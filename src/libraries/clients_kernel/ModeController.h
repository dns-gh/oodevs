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
    //! @name Types
    //@{
    typedef const std::string& (*T_Converter)( EnumType, ENT_Tr_ABC::E_Conversion );
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ModeController( QMainWindow* parent, EnumType initialMode, const QString& registeryEntry, T_Converter converter );
    virtual ~ModeController();
    //@}

    //! @name Operations
    //@{
    virtual void Initialize();
    virtual void ChangeMode( int newMode );
    virtual int GetCurrentMode() const;
    virtual void SaveGeometry();
    virtual const QString& GetRegisteryEntry() const { return registeryEntry_; }
    //@}

private:
    //! @name Helpers
    //@{
    bool LoadGeometry();
    //@}

private:
    //! @name Member data
    //@{
    QMainWindow*  parent_;
    const QString registeryEntry_;
    T_Converter   converter_;
    EnumType      currentMode_;
    //@}
};

#include "ModeController.inl"

} //! namespace kernel

#endif // __kernel_ModeController_h_
