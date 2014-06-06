// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GUI_DrawerShape_h_
#define __GUI_DrawerShape_h_

#include "Drawing.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class LocationProxy;
    class CoordinateConverter_ABC;
}

namespace gui
{
    class DrawingTemplate;
    class DrawingTypes;
    class GlTools_ABC;
    class SvgLocationDrawer;
}

namespace gui
{
// =============================================================================
/** @class  DrawerShape
    @brief  DrawerShape
*/
// Created: AGE 2006-08-31
// =============================================================================
class DrawerShape : public Drawing
                  , public kernel::Extension_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DrawerShape( kernel::Controllers& controllers, unsigned long id, const QString& name, const DrawingTemplate& style,
                          const QColor& color, const kernel::Entity_ABC* entity, kernel::LocationProxy& location,
                          const kernel::CoordinateConverter_ABC& coordinateConverter, E_Dash_style dashStyle );
             DrawerShape( kernel::Controllers& controllers, unsigned long id, xml::xistream& xis, const kernel::Entity_ABC* entity, const DrawingTypes& types,
                          kernel::LocationProxy& location, const kernel::CoordinateConverter_ABC& coordinateConverter );
    virtual ~DrawerShape();
    //@}

    //! @name Accessors
    //@{
    virtual QColor GetColor() const;
    virtual const kernel::Entity_ABC* GetDiffusionEntity() const;
    const DrawingTemplate& GetDrawingTemplate() const;
    //@}

    //! @name Operations
    //@{
    void Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision );
    //@}

    //! @name Operations
    //@{
    virtual void Handle( kernel::Location_ABC& location );
    virtual void Reset();

    virtual void Draw( const geometry::Rectangle2f& viewport, const GlTools_ABC& tools, bool overlined ) const;
    virtual void Draw( const kernel::Location_ABC& location, const geometry::Rectangle2f&, const GlTools_ABC& tools ) const;

    virtual void ChangeColor( const QColor& color );
    virtual void Edit( ParametersLayer& parameters );
    virtual void Serialize( xml::xostream& xos ) const;

    virtual void NotifyDestruction() const {}
    virtual const kernel::Location_ABC& GetLocation() const;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void Update();
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary();
    //@}

protected:
    //! @name Member data
    //@{
    const DrawingTemplate& style_;
    kernel::LocationProxy& location_;
    QColor color_;
    const QString template_;
    kernel::SafePointer< kernel::Entity_ABC > entity_;
    std::unique_ptr< SvgLocationDrawer > drawer_;
    bool isEditing_;
    //@}

private:
    //! @name Static Member
    //@{
    const kernel::CoordinateConverter_ABC& coordinateConverter_;
    //@}
};

}

#endif // __GUI_DrawerShape_h_
