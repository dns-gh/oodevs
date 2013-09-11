//*****************************************************************************
//
// $Created: JDY 03-07-16 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Tools.h $
// $Author: Nld $
// $Modtime: 21/07/05 11:40 $
// $Revision: 10 $
// $Workfile: ADN_Tools.h $
//
//*****************************************************************************

#ifndef __ADN_Gui_Tools_h_
#define __ADN_Gui_Tools_h_

#include "ADN_Connector_Vector_ABC.h"
#include "ADN_VectorEditionDialog.h"
#include "clients_kernel/tools.h"

template< typename T > class ADN_Type_Vector_ABC;

class Q3PopupMenu;

namespace ADN_Gui_Tools
{
    template< typename TargetType, typename ViewType >
    void GenerateStandardContextMenu( ViewType& view, const QPoint& pt )
    {
        Q3PopupMenu menu( &view );
        menu.insertItem( tools::translate( "ADN_Tools", "Add"), 0 );
        if( view.GetData( pt ) )
            menu.insertItem( tools::translate( "ADN_Tools", "Remove"), 1 );

        int result = menu.exec( pt );
        if( result == 1 )
            view.RemoveCurrentElement();
        else if( result == 0 )
            view.CreateNewElement< TargetType >();
    }

    template< typename SourceType, typename TargetType, typename ViewType >
    void GenerateStandardEditionDialog( ViewType& view, const QPoint& pt, const QString& objectName, const QString& dialogTitle, const QString& vectorName, ADN_Type_Vector_ABC< SourceType >& vector )
    {
        Q3PopupMenu menu( &view );
        menu.insertItem( tools::translate( "ADN_Tools", "Edit" ), 0 );
        if( view.GetData( pt ) )
            menu.insertItem( tools::translate( "ADN_Tools", "Remove" ), 1 );

        int menuResult = menu.exec( pt );
        if( menuResult == 1 )
            view.RemoveCurrentElement();
        else if( menuResult == 0 )
        {
            ADN_VectorEditionDialog< SourceType, TargetType >* dialog;
            dialog = new ADN_VectorEditionDialog< SourceType, TargetType >( view.objectName() + "_" + objectName, dialogTitle, &view );
            dialog->AddVector( vectorName, vector, view.GetModel(), static_cast< ADN_Connector_Vector_ABC& >( view.GetConnector() ) );
            dialog->exec();
        }
    }

}

#endif // __ADN_Gui_Tools_h_
