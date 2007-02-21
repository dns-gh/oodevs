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

namespace gui
{
    class GlWidget;

// =============================================================================
/** @class  SymbolIcons
    @brief  SymbolIcons
*/
// Created: AGE 2006-11-22
// =============================================================================
class SymbolIcons : public QObject, private IconHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SymbolIcons( QObject* parent, GlWidget*& widget );
    virtual ~SymbolIcons();
    //@}

    //! @name Operations
    //@{
    virtual QPixmap GetSymbol( const std::string& symbol, const QColor& color = Qt::white );
    virtual QPixmap GetSymbol( const std::string& symbol, const std::string& level, const QColor& color = Qt::white );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SymbolIcons( const SymbolIcons& );            //!< Copy constructor
    SymbolIcons& operator=( const SymbolIcons& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::pair< std::string, std::string > T_Key;
    typedef std::map< T_Key, QPixmap >            T_Icons;
    typedef std::set< T_Key >                     T_PendingIcons;
    typedef T_PendingIcons::const_iterator      CIT_PendingIcons;
    //@}

    //! @name Helpers
    //@{
    virtual void AddIcon( const std::string& symbol, const std::string& level, const QPixmap& icon );
    //@}

private:
    //! @name Member data
    //@{
    GlWidget*& widget_;
    T_PendingIcons pending_;
    T_Icons icons_;
    //@}
};

}

#endif // __SymbolIcons_h_
