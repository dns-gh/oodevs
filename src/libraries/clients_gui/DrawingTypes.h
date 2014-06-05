// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawingTypes_h_
#define __DrawingTypes_h_

#include <tools/Resolver.h>

namespace kernel
{
    class Controller;
}

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

namespace svg
{
    class TextRenderer;
}

namespace gui
{
    class DrawingCategory;

// =============================================================================
/** @class  DrawingTypes
    @brief  DrawingTypes
*/
// Created: SBO 2008-06-04
// =============================================================================
class DrawingTypes : public tools::Resolver< DrawingCategory, QString >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DrawingTypes( kernel::Controller& controller );
    virtual ~DrawingTypes();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config );
    void Purge();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DrawingTypes( const DrawingTypes& );            //!< Copy constructor
    DrawingTypes& operator=( const DrawingTypes& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadCategory( xml::xistream& xis );
    void Read( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    std::unique_ptr< svg::TextRenderer > renderer_;
    //@}
};

}

#endif // __DrawingTypes_h_
