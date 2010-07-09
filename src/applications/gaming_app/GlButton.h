// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __GlButton_h_
#define __GlButton_h_

namespace kernel
{
    class GlTools_ABC;
}

namespace gui
{
    class GlTooltip;
}

// =============================================================================
/** @class  GlButton
    @brief  GlButton
*/
// Created: SBO 2008-07-04
// =============================================================================
class GlButton
{
public:
    //! @name Constructors/Destructor
    //@{
             GlButton( const QString& label, const kernel::GlTools_ABC& tools );
    virtual ~GlButton();
    //@}

    //! @name Operations
    //@{
    void SetPosition( int alignment );
    void SetMargin( int x, int y );
    void Draw( const geometry::Rectangle2f& boundingBox );
    bool HandleMousePress( const geometry::Point2f& point );
    bool HandleMouseMove( const geometry::Point2f& point );
    //@}

    //! @name Accessors
    //@{
    QString Label() const;
    geometry::Rectangle2f BoundingBox() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GlButton( const GlButton& );            //!< Copy constructor
    GlButton& operator=( const GlButton& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    std::auto_ptr< gui::GlTooltip > CreateTooltip( const QFont& font, bool hover );
    void DrawFrame( QPainter& painter, const QRect& rect, bool hover );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::GlTools_ABC& tools_;
    QString label_;
    std::auto_ptr< gui::GlTooltip > default_;
    std::auto_ptr< gui::GlTooltip > hover_;
    gui::GlTooltip* current_;
    geometry::Point2f position_;
    int alignment_;
    unsigned int width_;
    unsigned int height_;
    int xMargin_;
    int yMargin_;
    //@}
};

#endif // __GlButton_h_
