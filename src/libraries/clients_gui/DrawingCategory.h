// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawingCategory_h_
#define __DrawingCategory_h_

#include "clients_kernel/Resolver2.h"

namespace kernel
{
    class Controller;
}

namespace svg
{
    class TextRenderer;
}

namespace xml
{
    class xistream;
}

namespace gui
{
    class DrawingTemplate;

// =============================================================================
/** @class  DrawingCategory
    @brief  DrawingCategory
*/
// Created: SBO 2008-06-04
// =============================================================================
class DrawingCategory : public kernel::Resolver2< DrawingTemplate, QString, std::string >
{
public:
    //! @name Constructors/Destructor
    //@{
             DrawingCategory( xml::xistream& xis, svg::TextRenderer& renderer, kernel::Controller& controller,
                              bool internalCategory );
    virtual ~DrawingCategory();
    //@}

    //! @name Accessors
    //@{
    QString GetName() const;
    QString GetDescription() const;
    bool IsInternal() const;
    const DrawingTemplate& GetTemplate( const std::string& templateName ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DrawingCategory( const DrawingCategory& );            //!< Copy constructor
    DrawingCategory& operator=( const DrawingCategory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadTemplate( xml::xistream& xis, svg::TextRenderer& renderer );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const QString name_;
    const QString id_;
    const QString description_;
    const bool internalCategory_;
    //@}
};

}

#endif // __DrawingCategory_h_
