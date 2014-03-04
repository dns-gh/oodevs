// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef _gui_LocationEditorToolbar_h_
#define _gui_LocationEditorToolbar_h_

#include "RichToolBar.h"

#include "clients_kernel/ContextMenuObserver_ABC.h"

#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class CoordinateConverter_ABC;
    class Controllers;
    class ModelLoaded;
}

namespace gui
{
    class FeatureNameParser;
    class LocationEditorBox;
    class LocationsLayer;
    template< typename T > class RichWidget;
    class View_ABC;

// =============================================================================
/** @class  LocationEditorToolbar
    @brief  LocationEditorToolbar
*/
// Created: SBO 2007-03-06
// =============================================================================
class LocationEditorToolbar : public RichToolBar
                            , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
                            , public tools::ElementObserver_ABC< kernel::ModelLoaded >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LocationEditorToolbar( QMainWindow* parent, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, View_ABC& view, LocationsLayer& layer );
    virtual ~LocationEditorToolbar();
    //@}

private slots:
    //! @name Slots
    //@{
    void Goto();
    void AddParamPoint();
    void CreateBookmark();
    void GotoBookmark( int index );
    void ClearBookmarks();
    //@}

private:
    //! @name Observers implementation
    //@{
    virtual void NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu );
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    //@}

    //! @name Helpers
    //@{
    bool GetPosition( geometry::Point2f& point ) const;
    void CreateBookmark( const std::string& defaultName );
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
    gui::RichWidget< QToolButton >* paramsButton_;
    kernel::ContextMenu* bookmarksMenu_;
    T_Bookmarks bookmarks_;
    geometry::Point2f menuPoint_;
    LocationEditorBox* locBox_;
    std::shared_ptr< FeatureNameParser > featureNameParser_;
    //@}
};

}

#endif // _gui_LocationEditorToolbar_h_
