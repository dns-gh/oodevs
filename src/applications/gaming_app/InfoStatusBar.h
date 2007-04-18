// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoStatusBar_h_
#define __InfoStatusBar_h_

#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class HumanFactors_ABC;
}

class HumanFactors;
class Reinforcements;

// =============================================================================
/** @class  InfoStatusBar
    @brief  InfoStatusBar
*/
// Created: SBO 2007-02-15
// =============================================================================
class InfoStatusBar : public QWidget
                    , public kernel::Observer_ABC
                    , public kernel::SelectionObserver< kernel::Entity_ABC >
                    , public kernel::ElementObserver_ABC< kernel::HumanFactors_ABC >
                    , public kernel::ElementObserver_ABC< Reinforcements >
{

public:
    //! @name Constructors/Destructor
    //@{
             InfoStatusBar( QWidget* parent, kernel::Controllers& controllers );
    virtual ~InfoStatusBar();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoStatusBar( const InfoStatusBar& );            //!< Copy constructor
    InfoStatusBar& operator=( const InfoStatusBar& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void InitializeIcons();
    void AddButton( QGridLayout* grid, QLabel*& label, int index );

    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyUpdated( const kernel::HumanFactors_ABC& element );
    virtual void NotifyUpdated( const Reinforcements& element );

    void SetHumanFactors( const HumanFactors& humans );
    void SetReinforcements( const Reinforcements& reinforcements );
    //@}

    //! @name Types
    //@{
    typedef std::vector< QPixmap > T_Pixmaps;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;

    T_Pixmaps moodIcons_;
    T_Pixmaps experienceIcons_;
    T_Pixmaps tirednessIcons_;

    QLabel* reinforced_;
    QLabel* reinforcing_;
    QLabel* mood_;
    QLabel* experience_;
    QLabel* tiredness_;

    QPixmap reinforcingPixmap_;
    QPixmap reinforcedPixmap_;
    //@}
};

#endif // __InfoStatusBar_h_
