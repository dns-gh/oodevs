// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 MASA Group
//
// *****************************************************************************

#ifndef __SymbolIcons_h_
#define __SymbolIcons_h_

#include "SymbolIcon.h"

namespace gui
{
    class GlWidget;

// =============================================================================
/** @class  SymbolIcons
    @brief  Symbol icons
*/
// Created: AGE 2006-11-22
// =============================================================================
class SymbolIcons : QObject
{
    Q_OBJECT

public:
             SymbolIcons();
    virtual ~SymbolIcons();

    void Initialize( gui::GlWidget* widget );

    const QPixmap& GetSymbol( const SymbolIcon& symbol );
    const QPixmap& GetDefaultSymbol() const;

private:
    QPixmap GenerateSymbol( const SymbolIcon& symbol );

    void Draw( std::string symbol, const geometry::Point2f& center, float factor ) const;

private slots:
    void Destroyed( QObject* );

private:
    QPixmap defaultSymbol_;
    std::map< SymbolIcon, QPixmap > icons_;
    QGLWidget* context_;
    gui::GlWidget* widget_;
};

}

#endif // __SymbolIcons_h_
