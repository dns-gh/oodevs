 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __VisionCones_h_
#define __VisionCones_h_

#include "game_asn/Simulation.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class SensorType;
    class Agent_ABC;
    class Workers;
}

class Surface;
class SurfaceFactory;
class VisionMap;

// =============================================================================
/** @class  VisionCones
    @brief  VisionCones
*/
// Created: AGE 2006-02-14
// =============================================================================
class VisionCones : public kernel::Extension_ABC
                  , public kernel::Updatable_ABC< ASN1T_MsgUnitVisionCones >
                  , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
                  , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             VisionCones( const kernel::Agent_ABC& agent, SurfaceFactory& factory, kernel::Workers& workers );
    virtual ~VisionCones();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DrawFill( const kernel::Viewport_ABC& viewport ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    VisionCones( const VisionCones& );            //!< Copy constructor
    VisionCones& operator=( const VisionCones& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< Surface* >      T_Surfaces;
    typedef T_Surfaces::const_iterator CIT_Surfaces;
    struct Updater;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitVisionCones& message );
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    void Invalidate();
    void Update() const;
    void CancelCurrent();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Agent_ABC& agent_;
    SurfaceFactory& factory_;
    kernel::Workers& workers_;

    bool needUpdating_;
    Updater* current_;
    VisionMap* map_;
    T_Surfaces surfaces_;
    double elongationFactor_;
    //@}
};

#endif // __VisionCones_h_
