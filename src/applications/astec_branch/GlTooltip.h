// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlTooltip_h_
#define __GlTooltip_h_

#include "NoLinkDisplayer.h"

// =============================================================================
/** @class  GlTooltip
    @brief  GlTooltip
    // $$$$ AGE 2006-06-29: Ajouter des couleurs, ...
*/
// Created: AGE 2006-06-29
// =============================================================================
class GlTooltip : public NoLinkDisplayer
{

public:
    //! @name Constructors/Destructor
    //@{
             GlTooltip();
    virtual ~GlTooltip();
    //@}

    //! @name Operations
    //@{
    void Draw( const geometry::Point2f& position );
    virtual void Hide();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GlTooltip( const GlTooltip& );            //!< Copy constructor
    GlTooltip& operator=( const GlTooltip& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual Displayer_ABC& SubItem( const char* name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();

    void GenerateImage();
    QPixmap CreatePixmap( QPainter& p );
    void RestoreAlpha();
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::string >   T_Messages;
    typedef T_Messages::const_iterator CIT_Messages;
    //@}

private:
    //! @name Member data
    //@{
    std::string currentItem_;
    QString message_;
    T_Messages messages_;
    QImage image_;
    //@}
};

#endif // __GlTooltip_h_
