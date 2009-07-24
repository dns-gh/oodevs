// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __XmliaPanel_h_
#define __XmliaPanel_h_

namespace kernel
{
    class Controllers;
}

class Publisher_ABC;
class CommandHandler;

// =============================================================================
/** @class  XmliaPanel
    @brief  XmliaPanel
*/
// Created: RPD 2009-06-11
// =============================================================================
class XmliaPanel : public QDockWindow
{

public:
    //! @name Constructors/Destructor
    //@{
             XmliaPanel( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, CommandHandler& handler );
    virtual ~XmliaPanel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    XmliaPanel( const XmliaPanel& );            //!< Copy constructor
    XmliaPanel& operator=( const XmliaPanel& ); //!< Assignment operator
    //@}
};

#endif // __XmliaPanel_h_
