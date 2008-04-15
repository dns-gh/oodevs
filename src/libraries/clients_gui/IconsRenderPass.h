// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IconsRenderPass_h_
#define __IconsRenderPass_h_

#include "GlRenderPass_ABC.h"
#include "SymbolIcon.h"

namespace gui
{
    class IconHandler_ABC;
    class GlWidget;

// =============================================================================
/** @class  IconsRenderPass
    @brief  IconsRenderPass
*/
// Created: SBO 2008-04-11
// =============================================================================
class IconsRenderPass : public GlRenderPass_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             IconsRenderPass();
    virtual ~IconsRenderPass();
    //@}

    //! @name Operations
    //@{
    void CreateIcon( const SymbolIcon& symbol, IconHandler_ABC& handler );

    virtual std::string GetName() const;
    virtual void Render( GlWidget& widget );
    virtual unsigned int Width() const;
    virtual unsigned int Height() const;
    virtual geometry::Rectangle2f Viewport() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IconsRenderPass( const IconsRenderPass& );            //!< Copy constructor
    IconsRenderPass& operator=( const IconsRenderPass& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::pair< SymbolIcon, IconHandler_ABC* > T_IconTask;
    typedef std::vector< T_IconTask >                 T_IconTasks;
    typedef T_IconTasks::const_iterator             CIT_IconTasks;
    //@}

    //! @name Helpers
    //@{
    void RenderIcon( const T_IconTask& task, GlWidget& widget );
    //@}

private:
    //! @name Statics
    //@{
    static const unsigned int iconSide_ = 128;
    //@}

    //! @name Member data
    //@{
    const geometry::Rectangle2f viewport_;
    T_IconTasks tasks_;
    //@}
};

}

#endif // __IconsRenderPass_h_
