// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLine_ABC_h_
#define __TacticalLine_ABC_h_

#include "clients_gui/EntityImplementation.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace sword
{
    class Diffusion;
    class LimitUpdate;
    class Location;
    class PhaseLineUpdate;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

class Publisher_ABC;

// =============================================================================
/** @class  TacticalLine_ABC
    @brief  Base class of the limits and limas.
*/
// Created: APE 2004-04-14
// =============================================================================
class TacticalLine_ABC : public gui::EntityImplementation< kernel::TacticalLine_ABC >
                       , public kernel::Extension_ABC
                       , public kernel::Updatable_ABC< sword::PhaseLineUpdate >
                       , public kernel::Updatable_ABC< sword::LimitUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             TacticalLine_ABC( kernel::Controller& controller,
                               const QString& baseName,
                               unsigned long id,
                               Publisher_ABC& publisher,
                               const kernel::CoordinateConverter_ABC& converter,
                               const T_CanBeRenamedFunctor& canBeRenamedFunctor );
    virtual ~TacticalLine_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyDestruction();
    void Create();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId  () const;
    //@}

protected:
    //! @name Types
    //@{
    enum E_State
    {
        eStateOk       = 0x00,
        eStateCreated  = 0x01,
        eStateModified = 0x02,
        eStateDeleted  = 0x04
    };
    //@}

    //! @name Helpers
    //@{
    void WriteGeometry( sword::Location& loc ) const;
    void WriteDiffusion( sword::Diffusion& diffusion ) const;
    virtual void UpdateToSim( E_State state ) = 0;
    template< typename Message >
    void Send( Message& message )
    {
        message.Send( publisher_);
    }

    virtual void DoUpdate( const sword::PhaseLineUpdate& message );
    virtual void DoUpdate( const sword::LimitUpdate&  message );
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateName( const std::string& name );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalLine_ABC( const TacticalLine_ABC& );
    TacticalLine_ABC& operator=( const TacticalLine_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    Publisher_ABC& publisher_;
    unsigned long  id_;
    //@}
};

#endif // __TacticalLine_ABC_h_
