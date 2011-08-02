//*****************************************************************************
//
// $Created: JDY 03-07-02 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListViewItem.cpp $
// $Author: Ape $
// $Modtime: 28/02/05 11:13 $
// $Revision: 5 $
// $Workfile: ADN_ListViewItem.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ListViewItem.h"

#include "ADN_Connector_ABC.h"
#include "ADN_ListView.h"


class ADN_ListViewItem_Connector
: public ADN_Connector_ABC
{
public:

    ADN_ListViewItem_Connector(ADN_ListViewItem& lvi,int pos,ADN_ListViewItem::E_TypeCvt eCvt, const char* szName = 0 )
    : ADN_Connector_ABC( szName )
    , lvi_(lvi)
    , nPos_(pos)
    , eCvt_(eCvt)
    {
    }

    virtual ~ADN_ListViewItem_Connector()
    {
    }

    void SetDataPrivate(void *data)
    {

        switch ( eCvt_ )
        {
            case ADN_ListViewItem::eString:
            {
                lvi_.setText(nPos_,((std::string*)data)->c_str());
                break;
            }
            case ADN_ListViewItem::eInt:
            {
                char   istring[256];
                sprintf_s(istring,"%d",*(int*)data);
                lvi_.setText(nPos_,istring);
                break;
            }
            case ADN_ListViewItem::eDouble:
            {
                char   istring[256];
                sprintf_s(istring,"%f",*(double*)data);
                lvi_.setText(nPos_,istring);
                break;
            }
            default:
                break;
        }
    }

    void  SetDataChanged(const QString& text)
    {
        switch ( eCvt_ )
        {
            case ADN_ListViewItem::eString:
            {
                std::string string=text.ascii();
                emit DataChanged(&string);
                break;
            }
            case ADN_ListViewItem::eInt:
            {
                int newval=atoi(text.ascii());
                emit DataChanged(&newval);
                break;
            }
            case ADN_ListViewItem::eDouble:
            {
                double newval=atof(text.ascii());
                emit DataChanged(&newval);
                break;
            }
            default:
                break;
        }
    }

private:
    ADN_ListViewItem_Connector& operator=( const ADN_ListViewItem_Connector& );

private:

    int                         nPos_;
    ADN_ListViewItem&           lvi_;
    ADN_ListViewItem::E_TypeCvt eCvt_;
    T_ConnectorVector        vConnectors_;
};




//-----------------------------------------------------------------------------
// Name: ADN_ListViewItem constructor
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
ADN_ListViewItem::ADN_ListViewItem(ADN_ListView *parent,void* item,int ncol)
:   Q3ListViewItem(parent, parent?parent->lastItem ():0 )
,   pData_(item)
,   vConnectors_(ncol,(ADN_Connector_ABC*)0 )
{
    int size=parent->columns();
    if (parent && size<ncol )
        for (int i=0;i<ncol-size;++i)
        {
            parent->addColumn("");
            parent->setSorting(size+i,true);
        }

    if (parent)
    for (int j=0;j<parent->columns();++j)
        parent->setColumnWidthMode(j,Q3ListView::Maximum);
}


//-----------------------------------------------------------------------------
// Name: ADN_ListViewItem destructor
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
ADN_ListViewItem::~ADN_ListViewItem()
{
    pData_=0;
    clear_owned_ptrs(vConnectors_);
    vConnectors_.clear();
}


//-----------------------------------------------------------------------------
// Name: ADN_ListViewItem::Connect
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_ListViewItem::Connect(int ndx,ADN_Connector_ABC *data,E_TypeCvt cvt)
{
    // create new internal connector
    vConnectors_[ndx]=new ADN_ListViewItem_Connector(*this,ndx,cvt, "ADN_ListViewItem_Connector" );
    assert(vConnectors_[ndx]);


    // connect internal connect and external data
    vConnectors_[ndx]->Connect(data);
}



//-----------------------------------------------------------------------------
// Name: ADN_ListViewItem::okRename
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_ListViewItem::okRename ( int col )
{
    assert(vConnectors_[col]);
    Q3ListViewItem::okRename(col);
    static_cast<ADN_ListViewItem_Connector*>(vConnectors_[col])->SetDataChanged(text(col));
}