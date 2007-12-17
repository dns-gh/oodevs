// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Speeds_h_
#define __Speeds_h_

#include "game_asn/asn.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  Speeds
    @brief  Speeds
*/
// Created: AGE 2007-12-17
// =============================================================================
class Speeds : public kernel::Extension_ABC
             , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
             , public kernel::Updatable_ABC< ASN1T_MsgUnitKnowledgeUpdate >
             , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Speeds( const kernel::CoordinateConverter_ABC& converter );
    virtual ~Speeds();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Speeds( const Speeds& );            //!< Copy constructor
    Speeds& operator=( const Speeds& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& attributes );
    virtual void DoUpdate( const ASN1T_MsgUnitKnowledgeUpdate& attributes );
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    geometry::Vector2f direction_;
    float speed_;
    //@}
};

#endif // __Speeds_h_
