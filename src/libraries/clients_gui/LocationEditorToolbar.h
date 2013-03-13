// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef _gui_LocationEditorToolbar_h_
#define _gui_LocationEditorToolbar_h_

#include "RichToolBar.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Controllers;
}

namespace gui
{
    class LocationEditorBox;
    class LocationsLayer;
    class View_ABC;

// =============================================================================
/** @class  LocationEditorToolbar
    @brief  LocationEditorToolbar
*/
// Created: SBO 2007-03-06
// =============================================================================
class LocationEditorToolbar : public RichToolBar
                            , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LocationEditorToolbar( QMainWindow* parent, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, View_ABC& view, LocationsLayer& layer );
    virtual ~LocationEditorToolbar();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void Goto();
    void AddParamPoint();
    void CreateBookmark();
    void GotoBookmark( size_t index );
    void ClearBookmarks();
    //@}

private:
    //! @name Helpers
    //@{
    bool GetPosition( geometry::Point2f& point );
    void SetAspect( bool oneValue, bool red );
    //@}

    //! @name Types
    //@{
    struct Bookmark
    {
        Bookmark() {}
        Bookmark( const std::string& name, const std::string& position )
            : name_( name ), position_( position )
        {}
        std::string name_;
        std::string position_;
    };

    typedef std::vector< Bookmark > T_Bookmarks;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::CoordinateConverter_ABC& converter_;
    View_ABC& view_;
    LocationsLayer& layer_;
    QToolButton* paramsButton_;
    kernel::ContextMenu* bookmarksMenu_;
    T_Bookmarks bookmarks_;
    geometry::Point2f menuPoint_;
    LocationEditorBox* locBox_;
    //@}
};

}

#endif // _gui_LocationEditorToolbar_h_
