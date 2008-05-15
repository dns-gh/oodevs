// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MiniViewsRenderPass_h_
#define __MiniViewsRenderPass_h_

#include "GlRenderPass_ABC.h"

namespace gui
{
    class MiniView;

// =============================================================================
/** @class  MiniViewsRenderPass
    @brief  MiniViewsRenderPass
*/
// Created: SBO 2008-04-14
// =============================================================================
class MiniViewsRenderPass : public GlRenderPass_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MiniViewsRenderPass( MiniView& view );
    virtual ~MiniViewsRenderPass();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const;
    virtual void Render( MapWidget_ABC& widget );
    virtual unsigned int Width() const;
    virtual unsigned int Height() const;
    virtual geometry::Rectangle2f Viewport() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MiniViewsRenderPass( const MiniViewsRenderPass& );            //!< Copy constructor
    MiniViewsRenderPass& operator=( const MiniViewsRenderPass& ); //!< Assignment operator
    //@}

private:
    //! @name Statics
    //@{
    static const unsigned int miniViewSide_ = 128;
    //@}

    //! @name Member data
    //@{
    MiniView& view_;
    //@}
};

}

#endif // __MiniViewsRenderPass_h_
