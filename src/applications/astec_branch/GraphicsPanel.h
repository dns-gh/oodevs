// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GraphicsPanel_h_
#define __GraphicsPanel_h_

class GraphicPreferences;

// =============================================================================
/** @class  GraphicsPanel
    @brief  Graphics panel
*/
// Created: SBO 2006-04-04
// =============================================================================
class GraphicsPanel : public QWidget
{

public:
    //! @name Constructors/Destructor
    //@{
             GraphicsPanel( QWidget* parent );
    virtual ~GraphicsPanel();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GraphicsPanel( const GraphicsPanel& );            //!< Copy constructor
    GraphicsPanel& operator=( const GraphicsPanel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    GraphicPreferences& preferences_;
    //@}
};

#endif // __GraphicsPanel_h_
