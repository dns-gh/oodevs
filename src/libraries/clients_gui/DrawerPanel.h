// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerPanel_h_
#define __DrawerPanel_h_

class QToolBox;

namespace xml
{
    class xistream;
}

namespace kernel
{
    class GlTools_ABC;
}

namespace gui
{
    class DrawerLayer;
    class DrawerStyle;

// =============================================================================
/** @class  DrawerPanel
    @brief  Drawer panel
*/
// Created: AGE 2006-09-01
// =============================================================================
class DrawerPanel : public QVBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             DrawerPanel( QWidget* parent, DrawerLayer& layer, kernel::GlTools_ABC& tools );
    virtual ~DrawerPanel();
    //@}

    //! @name Operations
    //@{
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelect( DrawerStyle& style );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DrawerPanel( const DrawerPanel& );            //!< Copy constructor
    DrawerPanel& operator=( const DrawerPanel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadTemplates( kernel::GlTools_ABC& tools );
    void ReadCategory( xml::xistream& input, kernel::GlTools_ABC& tools );
    //@}

private:
    //! @name Member data
    //@{
    DrawerLayer& layer_;
    QToolBox* toolBox_;
    //@}
};

}

#endif // __DrawerPanel_h_
