//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_Targets_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:53 $
// $Revision: 12 $
// $Workfile: ADN_Sensors_Targets_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Sensors_Targets_GUI.h"

#include "ADN_App.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Sensors_Data.h"

typedef ADN_Sensors_Data::SensorInfos SensorInfos;
typedef ADN_Sensors_Data::TargetInfos TargetInfos;

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with T_TargetInfos_Vector
//-----------------------------------------------------------------------------
class ADN_CT_Sensors_Targets
:public ADN_Connector_Table_ABC
{
public:

    ADN_CT_Sensors_Targets(ADN_Sensors_Targets_GUI& tab)
    : ADN_Connector_Table_ABC(tab,false)
    {}

    void  AddSubItems(int i,void *obj)
    {
        assert(obj);
        ADN_TableItem_String *pItemString=0;
        ADN_TableItem_Int    *pItemIntDist=0;

        // set new values
        tab_.setItem(i,0,pItemString=new ADN_TableItem_String(&tab_,obj));
        tab_.setItem(i,1,pItemIntDist=new ADN_TableItem_Int(&tab_,obj));

        // set items validators range
        pItemIntDist->GetValidator().setBottom( 0 );

        // disable object name
        pItemString->setEnabled(false);
        pItemString->SetAutoEnabled(false);

        // connect items & datas
        pItemString->GetConnector().Connect(&static_cast<TargetInfos*>(obj)->ptrObject_.GetData()->strName_);
        pItemIntDist->GetConnector().Connect(&static_cast<TargetInfos*>(obj)->rDistanceDetection_);
    }

private:
    ADN_CT_Sensors_Targets& operator=( const ADN_CT_Sensors_Targets& );
};

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Targets_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Sensors_Targets_GUI::ADN_Sensors_Targets_GUI(QWidget * parent )
:   ADN_Table2( parent, "ADN_Sensors_Targets_GUI" )
{
    // peut etre selectionne & trie
    setSorting(true);
    setSelectionMode(Q3Table::Single);
    setShowGrid(false);
    setLeftMargin(0);

    // hide vertical header
    verticalHeader()->hide();

    // tab with 2 columns
    setNumCols(3);
    setNumRows(0);
    setColumnStretchable(0,true);
    setColumnStretchable(1,true);
    setColumnStretchable(2,true);
    setMaximumWidth( 300 );

    horizontalHeader()->setLabel(0, tr( "Objects"));
    horizontalHeader()->setLabel(1, tr( "Detection time (s)"));
    horizontalHeader()->setLabel(2, tr( "Detection distance (m)"));

    // connector creation
    pConnector_=new ADN_CT_Sensors_Targets(*this);
}


//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Targets_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Sensors_Targets_GUI::~ADN_Sensors_Targets_GUI()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Targets_GUI::OnContextMenu
/** @param  row
    @param  col
    @param  pt
*/
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
void ADN_Sensors_Targets_GUI::OnContextMenu(int /*row*/,int /*col*/,const QPoint& pt)
{
    std::auto_ptr< Q3PopupMenu > pTargetMenu( new Q3PopupMenu(this) );


    // Get the list of the possible munitions
    bool bDisplayAdd = false;
    bool bDisplayRem = GetCurrentData()!=0;
    ADN_Objects_Data::T_ObjectsInfos_Vector& vAllMunitions = ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos();

    for( ADN_Objects_Data::T_ObjectsInfos_Vector::iterator it = vAllMunitions.begin(); it != vAllMunitions.end(); ++it )
    {
        ADN_Objects_Data::ObjectInfos* pInfo = *it;
        if( Contains( pInfo->strName_.GetData() ) != 0 )
            continue;

        bDisplayAdd = true;
        pTargetMenu->insertItem( pInfo->strName_.GetData().c_str(), static_cast< int >( 2 + std::distance( vAllMunitions.begin(), it ) ) );
    }

    if( ! bDisplayAdd && !bDisplayRem )
        return;

    Q3PopupMenu * pMenu=new Q3PopupMenu(this);
    if( bDisplayAdd )
        pMenu->insertItem( tr( "Add object" ), pTargetMenu.get() ,0 );
    if( bDisplayRem )
        pMenu->insertItem( tr( "Remove object" ), 1 );

    int nMenu=pMenu->exec(pt);
    if( nMenu == 1 )
    {
        RemoveCurrentElement();
    }
    else if( nMenu > 1 )
    {
        assert( nMenu - 2 < (int)vAllMunitions.size() );

        AddNewElement( nMenu - 2 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Targets_GUI::Contains
/** @param  strMunitionName
    @return
*/
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
bool ADN_Sensors_Targets_GUI::Contains( const std::string& strMunitionName )
{
    int i = 0;
    while( text( i, 0 ) != QString::null )
    {
        if( text( i, 0 ) == strMunitionName.c_str() )
            return true;
        ++i;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Targets_GUI::AddNewElement
/** @param  n
*/
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
void ADN_Sensors_Targets_GUI::AddNewElement( int n )
{
    // Get the list of the defined targets
    TargetInfos* pNewInfo = new TargetInfos();
    pNewInfo->ptrObject_ = ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos()[n];

    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( pNewInfo );
    pCTable->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Targets_GUI::RemoveCurrentElement
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
void ADN_Sensors_Targets_GUI::RemoveCurrentElement()
{
    // delete composante
    TargetInfos* pCurComposante=(TargetInfos*)GetCurrentData();
    if( pCurComposante )
    {
        // remove current data from list
        // take care cause pCurData_ can change!!
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem(pCurComposante);
    }
}
