// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlRenderPass_ABC_h_
#define __GlRenderPass_ABC_h_

namespace gui
{
    class GlWidget;

// =============================================================================
/** @class  GlRenderPass_ABC
    @brief  GlRenderPass_ABC
*/
// Created: SBO 2008-04-11
// =============================================================================
class GlRenderPass_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             GlRenderPass_ABC() {}
    virtual ~GlRenderPass_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const = 0;
    virtual void Render( GlWidget& widget ) = 0;
    virtual unsigned int Width() const = 0;
    virtual unsigned int Height() const = 0;
    virtual geometry::Rectangle2f Viewport() const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GlRenderPass_ABC( const GlRenderPass_ABC& );            //!< Copy constructor
    GlRenderPass_ABC& operator=( const GlRenderPass_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

}

#endif // __GlRenderPass_ABC_h_
