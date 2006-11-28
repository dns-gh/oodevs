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

#include "ASN_Types.h"
#include "DIN_Types.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/ThreadShared.h"

namespace kernel
{
    class CoordinateConverter_ABC;
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
                  , public kernel::Updatable_ABC< VisionConesMessage >
                  , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
                  , public kernel::Drawable_ABC
                  , public kernel::ThreadShared
{

public:
    //! @name Constructors/Destructor
    //@{
             VisionCones( const kernel::Agent_ABC& agent, SurfaceFactory& factory, kernel::Workers& workers );
    virtual ~VisionCones();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
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
    struct Updater
    {
        Updater( VisionCones& cones );
        void operator()();
        VisionCones* cones_;
        kernel::ThreadShared::Locker locker_;
    };
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const VisionConesMessage& message );
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    void Update() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Agent_ABC& agent_;
    SurfaceFactory& factory_;
    kernel::Workers& workers_;
    VisionMap* map_;
    T_Surfaces surfaces_;
    double elongationFactor_;
    mutable volatile bool needUpdating_;
    mutable volatile bool updating_;
    mutable volatile bool commiting_; // $$$$ AGE 2006-11-28: not really enough on dual procs...
    
    //@}
};

#endif // __VisionCones_h_
