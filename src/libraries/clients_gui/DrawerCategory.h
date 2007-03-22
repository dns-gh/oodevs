// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerCategory_h_
#define __DrawerCategory_h_

#include <qvbuttongroup.h>
#include "clients_kernel/Resolver2.h"

namespace xml {
    class xistream;
}

namespace kernel {
    class GlTools_ABC;
    class Controller;
}

namespace svg {
    class TextRenderer;
}

namespace gui
{
    class DrawerStyle;

// =============================================================================
/** @class  DrawerCategory
    @brief  Drawer category
*/
// Created: AGE 2006-09-01
// =============================================================================
class DrawerCategory : public QVButtonGroup
                     , public kernel::Resolver2< DrawerStyle, int, QString >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             DrawerCategory( QWidget* parent, kernel::GlTools_ABC& tools, xml::xistream& input, svg::TextRenderer& renderer, kernel::Controller& controller );
    virtual ~DrawerCategory();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    QString GetDescription() const;
    //@}

signals:
    //! @name Signals
    //@{
    void Selected( DrawerStyle& style );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClicked( int id );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DrawerCategory( const DrawerCategory& );            //!< Copy constructor
    DrawerCategory& operator=( const DrawerCategory& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadTemplates( xml::xistream& input, kernel::GlTools_ABC& tools );
    void ReadTemplate( xml::xistream& input, kernel::GlTools_ABC& tools );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    svg::TextRenderer& renderer_;
    QString name_;
    QString description_;
    //@}
};

}

#endif // __DrawerCategory_h_
