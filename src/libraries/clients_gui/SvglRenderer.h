// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SvglRenderer_h_
#define __SvglRenderer_h_

namespace svg
{
    class Node_ABC;
    class TextRenderer;
    class References;
    class RenderingContext;
    class ListLengthFactory;
    class Style;
}

namespace xml
{
    class xistream;
}

namespace gui
{
// =============================================================================
/** @class  SvglRenderer
    @brief  SvglRenderer
*/
// Created: AGE 2007-05-31
// =============================================================================
class SvglRenderer
{
public:
    //! @name Constructors/Destructor
    //@{
             SvglRenderer();
    virtual ~SvglRenderer();
    //@}

    //! @name Operations
    //@{
    static std::string DefaultStyle();
    void SetCurrentColor( float r, float g, float b, float a );
    svg::Node_ABC* Compile( xml::xistream& input, float lod);
    void Render( const std::shared_ptr< svg::Node_ABC >& node, const std::string& style, const geometry::Rectangle2f& viewport,
                 unsigned vWidth, unsigned vHeight, bool pickingMode );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SvglRenderer( const SvglRenderer& );            //!< Copy constructor
    SvglRenderer& operator=( const SvglRenderer& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::shared_ptr< svg::Node_ABC >, unsigned int > T_Lists;
    typedef T_Lists::const_iterator                CIT_Lists;
    //@}

    //! @name Helpers
    //@{
    void         Draw( const std::shared_ptr< svg::Node_ABC >& node, const std::string& style, const geometry::Rectangle2f& viewport, unsigned vWidth, unsigned vHeight, bool pickingMode );
    void         ConfigureColorList();
    void         ConfigureWidthList( const geometry::Rectangle2f& viewport, unsigned vWidth, unsigned vHeight );
    void         CreateStaticLists();
    unsigned int RetrieveListId( const std::shared_ptr< svg::Node_ABC >& node, const std::string& style, const geometry::Rectangle2f& viewport,
                                 unsigned vWidth, unsigned vHeight, bool pickingMode, T_Lists& lists );
    std::unique_ptr< svg::Style > CreateStyle( const std::string& style );
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< svg::References >        references_;
    std::unique_ptr< svg::RenderingContext >  renderingContext_;
    std::unique_ptr< svg::ListLengthFactory > listLenghts_;

    T_Lists lists_;
    geometry::Rectangle2f previousViewport_;
    unsigned int previousWidth_;
    unsigned int previousHeight_;
    float r_, g_, b_, a_;
    bool colorDirty_;
    //@}

    //! @name Static data
    //@{
    static std::unique_ptr< svg::TextRenderer > renderer_;
    static unsigned int colorList_;
    //@}
};

}

#endif // __SvglRenderer_h_
