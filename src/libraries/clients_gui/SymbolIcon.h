// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 MASA Group
//
// *****************************************************************************

#ifndef __SymbolIcon_h_
#define __SymbolIcon_h_

namespace gui
{
// =============================================================================
/** @class  SymbolIcon
    @brief  SymbolIcon
*/
// Created: AGE 2007-10-31
// =============================================================================
class SymbolIcon
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SymbolIcon( const std::string& symbol, const std::string& level = std::string() );
    virtual ~SymbolIcon();
    //@}

    //! @name Construction
    //@{
    void AddLevel( const std::string& level );
    void SetColor( const QColor& color );
    void SetSize( unsigned size );
    //@}

    //! @name Operators
    //@{
    bool operator<( const SymbolIcon& rhs ) const;
    //@}

public:
    //! @name Member data
    //@{
    std::string symbol_;
    std::string level_;
    QColor color_;
    QSize size_;
    //@}
};

}

#endif // __SymbolIcon_h_
