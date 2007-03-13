// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionsPanel_h_
#define __ActionsPanel_h_

// =============================================================================
/** @class  ActionsPanel
    @brief  ActionsPanel
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionsPanel : public QDockWindow
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionsPanel( QMainWindow* parent );
    virtual ~ActionsPanel();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionsPanel( const ActionsPanel& );            //!< Copy constructor
    ActionsPanel& operator=( const ActionsPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __ActionsPanel_h_
