// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef gui_PropertyTreeView_h
#define gui_PropertyTreeView_h

namespace gui
{
    class PropertyModel;
// =============================================================================
/** @class  PropertyTreeView
    @brief  Property Tree view
*/
// Created: LGY 2012-08-09
// =============================================================================
class PropertyTreeView : public QTreeView
{
public:
    //! @name Constructors/Destructor
    //@{
             PropertyTreeView();
    virtual ~PropertyTreeView();
    //@}

    //! @name Operations
    //@{
    void SaveState();
    void Display();
    //@}

private:
    //! @name Helpers
    //@{
    void DisplayHeader( QModelIndex root, const QStandardItemModel& model );
    void SaveState( QModelIndex root, const QStandardItemModel& model );
    void RestoreState( const PropertyModel& model );
    //@}

private:
    //! @name Member Data
    //@{
    std::set< std::string > itemsCollapsed_;
    //@}
};

}

#endif // gui_PropertyTreeView_h
