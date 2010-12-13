 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __VisionCones_h_
#define __VisionCones_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "protocol/Protocol.h"

namespace kernel
{
    class SensorType;
    class Agent_ABC;
    class Workers;
}

namespace sword
{
    class UnitVisionCones;
    class UnitAttributes;
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
                  , public kernel::Updatable_ABC< sword::UnitVisionCones >
                  , public kernel::Updatable_ABC< sword::UnitAttributes >
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
    //! @name Copy/Assignment
    //@{
    VisionCones( const VisionCones& );            //!< Copy constructor
    VisionCones& operator=( const VisionCones& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< Surface* >      T_Surfaces;
    typedef T_Surfaces::const_iterator CIT_Surfaces;
    struct Updater;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitVisionCones& message );
    virtual void DoUpdate( const sword::UnitAttributes& message );
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
    float elongationFactor_;
    //@}
};

#endif // __VisionCones_h_
