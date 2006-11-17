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

namespace kernel {
    class GlTools_ABC;
}

namespace gui
{

// =============================================================================
/** @class  GlTooltip
    @brief  GlTooltip
*/
// Created: AGE 2006-06-29
// =============================================================================
class GlTooltip : public NoLinkDisplayer
                , public kernel::Caller< QColor >
                , public kernel::Caller< E_EtatOperationnel >
                , public kernel::Caller< E_EtatRapFor >
                , public kernel::Caller< E_EtatCombatRencontre >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit GlTooltip( const kernel::GlTools_ABC& tools );
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
    virtual void Call( const QColor& value );
    virtual void Call( const E_EtatOperationnel& value );
    virtual void Call( const E_EtatRapFor& value );
    virtual void Call( const E_EtatCombatRencontre& value );

    virtual Displayer_ABC& SubItem( const QString& name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();

    void GenerateImage();
    QPixmap CreatePixmap( QPainter& p );
    void RestoreAlpha();
    //@}

    //! @name Types
    //@{
    typedef std::pair< QString, QColor > T_Message;
    typedef std::vector< T_Message >     T_Messages;
    typedef T_Messages::const_iterator CIT_Messages;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::GlTools_ABC& tools_;
    QString currentItem_;
    QColor color_;
    QString message_;
    T_Messages messages_;
    QImage image_;
    //@}
};

}

#endif // __GlTooltip_h_
