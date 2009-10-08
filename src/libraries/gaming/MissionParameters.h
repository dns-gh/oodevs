// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameters_h_
#define __MissionParameters_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "tools/Resolver.h"
#include "game_asn/Simulation.h"

namespace kernel
{
    class Controller;
}

namespace actions
{
    class Action_ABC;
}

class ActionFactory_ABC;

// =============================================================================
/** @class  MissionParameters
    @brief  Mission parameters
*/
// Created: SBO 2006-11-13
// =============================================================================
class MissionParameters : public kernel::Extension_ABC
                        , public kernel::Updatable_ABC< ASN1T_MsgUnitOrder >
                        , public kernel::Updatable_ABC< ASN1T_MsgAutomatOrder >
                        , public kernel::Updatable_ABC< ASN1T_MsgPopulationOrder >
                        , public tools::Resolver< actions::Action_ABC >
                        , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameters( kernel::Controller& controller, const ActionFactory_ABC& factory );
    virtual ~MissionParameters();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    void Display( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MissionParameters( const MissionParameters& );            //!< Copy constructor
    MissionParameters& operator=( const MissionParameters& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitOrder& message );
    virtual void DoUpdate( const ASN1T_MsgAutomatOrder& message );
    virtual void DoUpdate( const ASN1T_MsgPopulationOrder& message );
    template< typename T >
    void UpdateMessage( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const ActionFactory_ABC& factory_;
    //@}
};

#endif // __MissionParameters_h_
