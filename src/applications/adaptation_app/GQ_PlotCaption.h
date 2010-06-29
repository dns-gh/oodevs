// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: CBX 2003-11-25 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/GQ_PlotCaption.h $
// $Author: Ape $
// $Modtime: 13/01/05 12:16 $
// $Revision: 2 $
// $Workfile: GQ_PlotCaption.h $
//
// *****************************************************************************

#ifndef __GQ_PlotCaption_h_
#define __GQ_PlotCaption_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include <qhbox.h>

class GQ_Plot;

class QListBox;

// =============================================================================
/** @class  GQ_PlotCaption
    @brief  GQ_PlotCaption
    @par    Using example
    @code
    GQ_PlotCaption;
    @endcode
*/
// Created: CBX 2003-11-25
// =============================================================================
class GQ_PlotCaption : public QHBox
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    GQ_PlotCaption( QWidget* pParent, GQ_Plot& );

    virtual ~GQ_PlotCaption();
    //@}

public slots:
    void UpdateCaption();

protected:
    //! @name Qt overloaded methods
    //@{
    virtual void paintEvent( QPaintEvent* );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GQ_PlotCaption( const GQ_PlotCaption& );            //!< Copy constructor
    GQ_PlotCaption& operator=( const GQ_PlotCaption& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Update();
    //@}

private:
    //! @name Member data
    //@{
    GQ_Plot*  pPlot_;
    QListBox* pEntryList_;
    //@}

    //! @name Runtime members
    //@{
    bool bUpdateNeeded_;
    //@}
};

#endif // __GQ_PlotCaption_h_
