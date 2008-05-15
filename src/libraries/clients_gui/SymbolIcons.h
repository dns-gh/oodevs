// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SymbolIcons_h_
#define __SymbolIcons_h_

#include "IconHandler_ABC.h"
#include "SymbolIcon.h"

namespace kernel
{
    class GlTools_ABC;
}

namespace gui
{
    class GlWidget;
    class IconsRenderPass;

// =============================================================================
/** @class  SymbolIcons
    @brief  SymbolIcons
*/
// Created: AGE 2006-11-22
// =============================================================================
class SymbolIcons : public QObject, private IconHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SymbolIcons( QObject* parent, kernel::GlTools_ABC& tools );
    virtual ~SymbolIcons();
    //@}

    //! @name Operations
    //@{
    virtual QPixmap GetSymbol( SymbolIcon symbol );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnWidget2dChanged( gui::GlWidget* );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SymbolIcons( const SymbolIcons& );            //!< Copy constructor
    SymbolIcons& operator=( const SymbolIcons& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< SymbolIcon, QPixmap >       T_Icons;
    typedef std::set< SymbolIcon >                T_PendingIcons;
    typedef T_PendingIcons::const_iterator      CIT_PendingIcons;
    //@}

    //! @name Helpers
    //@{
    virtual void AddIcon( const SymbolIcon& task, const QPixmap& icon );
    //@}

private:
    //! @name Member data
    //@{
    GlWidget* widget_;
    T_PendingIcons pending_;
    T_Icons icons_;
    std::auto_ptr< IconsRenderPass > renderPass_;
    //@}
};

}

#endif // __SymbolIcons_h_
