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
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Drawable_ABC.h"
#include "Resolver_ABC.h"

class Surface;
class CoordinateConverter;
class SensorType;
class Agent;

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
             VisionCones( const Agent& agent, const Resolver_ABC< SensorType, std::string >& resolver );
    virtual ~VisionCones();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const;
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
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const VisionConesMessage& message );
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    //@}

private:
    //! @name Member data
    //@{
    const Agent& agent_;
    const Resolver_ABC< SensorType, std::string >& resolver_;
    T_Surfaces surfaces_;
    double elongationFactor_;
    bool needsUpdating_;
    //@}
};

#endif // __VisionCones_h_
