// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Drawer_h_
#define __Drawer_h_

namespace kernel
{
    class Drawable_ABC;
    class Extension_ABC;
    class GlTools_ABC;

// =============================================================================
/** @class  Drawer
    @brief  Drawer implementation
*/
// Created: AGE 2006-08-10
// =============================================================================
class Drawer
{

public:
    //! @name Constructors/Destructor
    //@{
             Drawer();
    virtual ~Drawer();
    //@}

    //! @name Operations
    //@{
    void Register( const Extension_ABC& extension );
    void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
    //@}

    // $$$$ AGE 2006-08-10: mettre static ?
    //! @name Operations
    //@{
    void SetPosition( const std::string& name, unsigned pos );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Drawer( const Drawer& );            //!< Copy constructor
    Drawer& operator=( const Drawer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Add( const Drawable_ABC& extension );
    std::string Strip( const std::string& name );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, unsigned >   T_Positions;
    typedef T_Positions::const_iterator       CIT_Positions;
    typedef std::vector< const Drawable_ABC* >  T_Drawables;
    typedef T_Drawables::const_iterator       CIT_Drawables;
    //@}

private:
    //! @name Member data
    //@{
    T_Positions positions_;
    T_Drawables extensions_; 
    //@}
};

}

#endif // __Drawer_h_
