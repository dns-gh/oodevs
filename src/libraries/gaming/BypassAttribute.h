// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BypassAttribute_h_
#define __BypassAttribute_h_

#include "game_asn/Simulation.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Drawable_ABC.h"
#include "Types.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
}

// =============================================================================
/** @class  BypassAttribute
    @brief  BypassAttribute
*/
// Created: SBO 2007-02-08
// =============================================================================
class BypassAttribute : public kernel::BypassAttribute_ABC
                     , public kernel::Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
                     , public kernel::Updatable_ABC< ASN1T_MsgObjectUpdate >
                     , public kernel::Updatable_ABC< ASN1T_MsgObjectCreation >
                     , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             BypassAttribute( kernel::Controller& controller );
    virtual ~BypassAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BypassAttribute( const BypassAttribute& );            //!< Copy constructor
    BypassAttribute& operator=( const BypassAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgObjectUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgObjectCreation& message );
    template< typename T >
    void UpdateData( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    float rBypassConstructionPercentage_;    
    //@}
};

#endif // __BypassAttribute_h_
