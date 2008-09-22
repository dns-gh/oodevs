// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Panel_ABC_h_
#define __Panel_ABC_h_

#include <boost/ptr_container/ptr_vector.hpp>

namespace frontend
{
    class SpawnCommand; 
} 

class QAction;
class QWidgetStack;
class ActionsContext;

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
             Panel_ABC( QWidgetStack* widget, QAction& action, ActionsContext& context, const char* name );
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
    void Start ( frontend::SpawnCommand* command ) ; 
    //@}

protected:
    //! @name Member data
    //@{
    ActionsContext& context_;
    //@}

private:
    //! @name Member data
    //@{
    QWidgetStack* stack_;
    QAction& action_;
    QAction* next_;
    boost::ptr_vector< frontend::SpawnCommand > commands_ ; 
    //@}
};

#endif // __Panel_ABC_h_
