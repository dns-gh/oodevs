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

namespace xml {
    class xistream;
}

namespace kernel {
    class GlTools_ABC;
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
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    DrawerCategory( QWidget* parent, kernel::GlTools_ABC& tools, xml::xistream& input, svg::TextRenderer& renderer );
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

    //! @name Types
    //@{
    typedef std::map< int, DrawerStyle* > T_Styles;
    typedef T_Styles::const_iterator    CIT_Styles;
    //@}

private:
    //! @name Member data
    //@{
    svg::TextRenderer& renderer_;
    QString name_;
    QString description_;
    T_Styles styles_;
    //@}
};

}

#endif // __DrawerCategory_h_
