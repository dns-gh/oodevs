// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SvglProxy_h_
#define __SvglProxy_h_

namespace svg
{
    class Node_ABC;
}

namespace gui
{
    class SvglRenderer;

// =============================================================================
/** @class  SvglProxy
    @brief  SvglProxy
*/
// Created: AGE 2007-05-31
// =============================================================================
class SvglProxy
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SvglProxy( SvglRenderer& renderer );
    virtual ~SvglProxy();
    //@}

    //! @name Operations
    //@{
    void Draw( const std::string& name, const geometry::Rectangle2f& viewport, unsigned vWidth = 640,
               unsigned vHeight = 480, bool pickingMode = false );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SvglProxy( const SvglProxy& );            //!< Copy constructor
    SvglProxy& operator=( const SvglProxy& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, std::shared_ptr< svg::Node_ABC > > T_Symbols;
    typedef T_Symbols::const_iterator             CIT_Symbols;
    //@}

private:
    //! @name Member data
    //@{
    SvglRenderer& renderer_;
    T_Symbols symbols_;
    //@}
};

}

#endif // __SvglProxy_h_
