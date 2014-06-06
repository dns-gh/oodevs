// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SVGFactory_h_
#define __SVGFactory_h_

#include "Factory_ABC.h"
#include "RenderingContext_ABC.h"

namespace svg
{
    class Node_ABC;
    class TextRenderer;
    class PropertyFactory;
    class PropertyFactory_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  SVGFactory
    @brief  SVGFactory
*/
// Created: AGE 2006-08-10
// =============================================================================
class SVGFactory : public svg::Factory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit SVGFactory( svg::TextRenderer& renderer );
    virtual ~SVGFactory();
    //@}

    //! @name Operations
    //@{
    void ChangePropertyFactory( svg::RenderingContext_ABC::E_Properties property, std::unique_ptr< svg::PropertyFactory_ABC > factory );
    void ChangePropertyFactory( svg::RenderingContext_ABC::E_Properties property, const svg::PropertyFactory_ABC& factory );

    svg::Node_ABC* Compile( const std::string& filename, svg::References_ABC& references, float expectedPrecision ) const;
    svg::Node_ABC* Compile( xml::xistream& input, svg::References_ABC& references, float expectedPrecision ) const;

    svg::Node_ABC* Parse( const std::string& filename, svg::References_ABC& references ) const;
    svg::Node_ABC* Parse( xml::xistream& input, svg::References_ABC& references ) const;

    virtual svg::Node_ABC* Create( const std::string& name, xml::xistream& input, svg::References_ABC& references ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SVGFactory( const SVGFactory& );            //!< Copy constructor
    SVGFactory& operator=( const SVGFactory& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    svg::TextRenderer& renderer_;
    std::unique_ptr< svg::PropertyFactory > factory_;
    //@}
};

#endif // __SVGFactory_h_
