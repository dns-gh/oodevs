// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Panel_ABC_h_
#define __Panel_ABC_h_

class QAction;
class QWidgetStack;

// =============================================================================
/** @class  Panel_ABC
    @brief  Panel_ABC
*/
// Created: SBO 2007-10-05
// =============================================================================
class Panel_ABC : public QVBox
{

public:
    //! @name Constructors/Destructor
    //@{
             Panel_ABC( QWidgetStack* widget, QAction& action );
    virtual ~Panel_ABC();
    //@}

    //! @name Operations
    //@{
    void LinkTo( Panel_ABC& next );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Panel_ABC( const Panel_ABC& );            //!< Copy constructor
    Panel_ABC& operator=( const Panel_ABC& ); //!< Assignment operator
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void setShown( bool show );
    virtual void showEvent( QShowEvent* event );
    virtual void Update();
    void ShowNext();
    //@}

private:
    //! @name Member data
    //@{
    QWidgetStack* stack_;
    QAction& action_;
    QAction* next_;
    //@}
};

#endif // __Panel_ABC_h_
