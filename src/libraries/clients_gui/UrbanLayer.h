// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanLayer_h_
#define __UrbanLayer_h_

#include "Layer_ABC.h"
#include "EntityLayer.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/OptionVariant.h"

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class Viewport_ABC;
}

namespace
{
    class InfrastructureHandler : public kernel::OptionsObserver_ABC, public tools::Observer_ABC
    {
    public:
        InfrastructureHandler( kernel::Controllers& controllers ): infraDisplayed_( false )
        {
            controllers.Register( *this );
        }
        ~InfrastructureHandler(){}
        virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value )
        {
            if ( name == "Infra" )
                infraDisplayed_ = value.To< bool >();
        }
        bool ShouldBeDisplayed(){ return infraDisplayed_; }
    private:
        bool infraDisplayed_;

    };
}

namespace gui
{
    class View_ABC;

// =============================================================================
/** @class  UrbanLayer
@brief  UrbanLayer
*/
// Created: SLG 2006-03-23
// =============================================================================
class UrbanLayer : public EntityLayer< TerrainObjectProxy >
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~UrbanLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( kernel::Viewport_ABC& viewport );
    virtual void Reset2d();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const TerrainObjectProxy& object );
    virtual void NotifyDeleted( const TerrainObjectProxy& object );
    virtual void NotifySelected( const TerrainObjectProxy* object );
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );
    virtual void ContextMenu( const kernel::Entity_ABC&, const geometry::Point2f&, const QPoint& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanLayer( const UrbanLayer& );            //!< Copy constructor
    UrbanLayer& operator=( const UrbanLayer& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const TerrainObjectProxy*   selectedObject_;
    InfrastructureHandler& infraHandler_;
    //@}
};

}
#endif // __UrbanLayer_h_