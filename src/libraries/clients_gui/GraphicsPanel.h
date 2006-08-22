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

namespace gui
{
    class GraphicPreferences;

// =============================================================================
/** @class  GraphicsPanel
    @brief  Graphics panel
*/
// Created: SBO 2006-04-04
// =============================================================================
class GraphicsPanel : public QVBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit GraphicsPanel( QWidget* parent );
    virtual ~GraphicsPanel();
    //@}

    GraphicPreferences& GetPreferences(); // $$$$ AGE 2006-04-05: 

private slots:
    //! @name Slots
    //@{
    void OnOk();
    void OnApply();
    void OnCancel();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GraphicsPanel( const GraphicsPanel& );            //!< Copy constructor
    GraphicsPanel& operator=( const GraphicsPanel& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    GraphicPreferences& preferences_;
    QWidget* parent_;
    //@}
};

}

#endif // __GraphicsPanel_h_
