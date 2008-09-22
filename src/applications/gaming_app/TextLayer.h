// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TextLayer_h_
#define __TextLayer_h_

#include "clients_gui/Layer_ABC.h"
#include "gaming/CommandHandler_ABC.h"

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class GlTooltip_ABC;
}

class CommandPublisher;
class CommandHandler;
class Publisher_ABC;
class GlButton;

// =============================================================================
/** @class  TextLayer
    @brief  TextLayer
*/
// Created: AGE 2008-06-24
// =============================================================================
class TextLayer : public gui::Layer_ABC
                , public CommandHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TextLayer( kernel::Controllers& controllers, Publisher_ABC& publisher, const kernel::GlTools_ABC& tools, CommandHandler& handler );
    virtual ~TextLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const ViewFrustum& frustum );
    virtual void Paint( const geometry::Rectangle2f& viewport );

    virtual void Receive( const Command& command );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TextLayer( const TextLayer& );            //!< Copy constructor
    TextLayer& operator=( const TextLayer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual bool HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMouseMove( QMouseEvent* mouse, const geometry::Point2f& point );
    void Display( const QString& text );
    void AddButton( const QString& label );
    void DrawFrame( QPainter& painter, const QRect& rect );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::GlTools_ABC& tools_;
    CommandHandler& handler_;
    std::auto_ptr< CommandPublisher > publisher_;
    std::auto_ptr< kernel::GlTooltip_ABC > tooltip_;
    std::auto_ptr< GlButton > button_;
    std::string currentPrompt_;
    //@}
};

#endif // __TextLayer_h_
