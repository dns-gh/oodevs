// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawingFactory_ABC_h_
#define __DrawingFactory_ABC_h_

namespace xml
{
    class xistream;
}

namespace gui
{
    class Drawing_ABC;
    class DrawingTemplate;

// =============================================================================
/** @class  DrawingFactory_ABC
    @brief  DrawingFactory_ABC
*/
// Created: SBO 2008-06-04
// =============================================================================
class DrawingFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DrawingFactory_ABC() {}
    virtual ~DrawingFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual Drawing_ABC* CreateShape( const DrawingTemplate& style, const QColor& color ) const = 0;
    virtual Drawing_ABC* CreateShape( xml::xistream& xis ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DrawingFactory_ABC( const DrawingFactory_ABC& );            //!< Copy constructor
    DrawingFactory_ABC& operator=( const DrawingFactory_ABC& ); //!< Assignment operator
    //@}
};

}

#endif // __DrawingFactory_ABC_h_
