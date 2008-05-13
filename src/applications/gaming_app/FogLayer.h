// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FogLayer_h_
#define __FogLayer_h_

#include "clients_gui/EntityLayer.h"

// =============================================================================
/** @class  FogLayer
    @brief  FogLayer
*/
// Created: SBO 2008-04-14
// =============================================================================
class FogLayer : public gui::EntityLayerBase
               , public kernel::ElementObserver_ABC< kernel::Agent_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             FogLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~FogLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void Paint( const ViewFrustum& frustum );
    virtual void Draw( const kernel::Entity_ABC& entity, kernel::Viewport_ABC& viewport );
    virtual void Reset2d();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FogLayer( const FogLayer& );            //!< Copy constructor
    FogLayer& operator=( const FogLayer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Agent_ABC& entity );
    virtual void NotifyDeleted( const kernel::Agent_ABC& entity );
    void CreateTexture();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    unsigned int texture_;
    //@}
};

#endif // __FogLayer_h_
