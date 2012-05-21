// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanInfosDockWidget_h_
#define __UrbanInfosDockWidget_h_

#include "clients_gui/RichDockWidget.h"

namespace kernel
{
    class Controllers;
}

class StaticModel;
class UrbanModel;

// =============================================================================
/** @class  UrbanInfosDockWidget
    @brief  UrbanInfosDockWidget
*/
// Created: ABR 2012-05-16
// =============================================================================
class UrbanInfosDockWidget : public gui::RichDockWidget
                           //, public MultipleSelectionObserver_ABC
                           //, public tools::ElementObserver_ABC< urban::TerrainObject_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             UrbanInfosDockWidget( QWidget* parent, kernel::Controllers& controllers, StaticModel& staticModel, UrbanModel& model );
    virtual ~UrbanInfosDockWidget();
    //@}

    //! @name Observers implementation
    //@{
    //virtual void SelectionChanged();
    //virtual void NotifyCreated( const urban::TerrainObject_ABC& element );
    //virtual void NotifyDeleted( const urban::TerrainObject_ABC& element );
    //virtual void NotifyUpdated( const urban::TerrainObject_ABC& element );
    //@}

private:
    //! @name Types
    //@{
    //class TerrainVisitor : public urban::TerrainObjectVisitor_ABC
    //{
    //public:
    //    virtual void VisitBlock( urban::TerrainObject_ABC& object ) { urbanBlocks_.insert( &object ); }
    //    std::set< urban::TerrainObject_ABC* > urbanBlocks_;
    //};

    //class MotivationsVisitor : public urban::MotivationsVisitor_ABC
    //{
    //public:
    //    virtual void Visit( const std::string& motivation, float proportion ) { proportions_[ motivation ] = proportion; }
    //    std::map< std::string, float > proportions_;
    //};
    //@}

    //! @name Helpers
    //@{
    void InsertLine( const QString& line = "", const QString& value = "" );
    void Update();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    StaticModel&         staticModel_;
    UrbanModel&          model_;
    QLabel*              urbanBlocksCount_;
    QTextEdit*           infoEditText_;
    QStringList          infos_;
    QStringList          values_;
    //@}
};

#endif // __UrbanInfosDockWidget_h_
