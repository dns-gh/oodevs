// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ScenarioEditPage_h_
#define __ScenarioEditPage_h_

#include "ContentPage.h" 

namespace tools
{
    class GeneralConfig;
}

namespace kernel
{
    class Controllers; 
}

class Session; 

// =============================================================================
/** @class  ScenarioEditPage
    @brief  ScenarioEditPage
*/
// Created: RDS 2008-09-09
// =============================================================================
class ScenarioEditPage : public ContentPage
{
    
    Q_OBJECT; 

public:
    //! @name Constructors/Destructor
    //@{
             ScenarioEditPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, kernel::Controllers& controllers, boost::shared_ptr< Session > sessionStatus );
    virtual ~ScenarioEditPage();
    //@}

private slots:
    //! @name Operations
    //@{
    void Update(); 
    void CreateExercise(); 
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScenarioEditPage( const ScenarioEditPage& );            //!< Copy constructor
    ScenarioEditPage& operator=( const ScenarioEditPage& ); //!< Assignment operator
    //@}


private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_ ; 
    QListBox*  editTerrainList_ ; 
    QLineEdit* editName_ ; 
    boost::shared_ptr< Session > sessionStatus_ ; 
    kernel::Controllers& controllers_ ; 
    //@}
};

#endif // __ScenarioEditPage_h_
