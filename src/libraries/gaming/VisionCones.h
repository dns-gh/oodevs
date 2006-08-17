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

class Surface;
class CoordinateConverter_ABC;
class SensorType;
class Agent_ABC;
class SurfaceFactory;
class VisionMap;
class Workers;

// =============================================================================
/** @class  VisionCones
    @brief  VisionCones
*/
// Created: AGE 2006-02-14
// =============================================================================
class VisionCones : public Extension_ABC
                  , public Updatable_ABC< VisionConesMessage >
                  , public Updatable_ABC< ASN1T_MsgUnitAttributes >
                  , public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             VisionCones( const Agent_ABC& agent, SurfaceFactory& factory, Workers& workers );
    virtual ~VisionCones();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
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
        Updater( const VisionCones& cones );
        void operator()();
        const VisionCones* cones_;
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
    const Agent_ABC& agent_;
    SurfaceFactory& factory_;
    Workers& workers_;
    VisionMap* map_;
    T_Surfaces surfaces_;
    double elongationFactor_;
    mutable volatile bool updating_;
    mutable volatile bool needUpdating_;
    //@}
};

#endif // __VisionCones_h_
