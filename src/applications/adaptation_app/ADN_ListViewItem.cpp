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

    ADN_ListViewItem_Connector( ADN_ListViewItem& lvi, ADN_ListViewItem::E_TypeCvt eCvt, const char* szName = 0 )
        : ADN_Connector_ABC( szName )
        , lvi_( lvi )
        , eCvt_( eCvt )
    {
    }

    virtual ~ADN_ListViewItem_Connector()
    {
    }

    void SetDataPrivate( void* data )
    {
        switch( eCvt_ )
        {
            case ADN_ListViewItem::eString:
            {
                lvi_.setText( ( static_cast< std::string* >( data ) )->c_str() );
                break;
            }
            case ADN_ListViewItem::eInt:
            {
                lvi_.setText( QString::number( *static_cast< int* >( data ) ) );
                break;
            }
            case ADN_ListViewItem::eDouble:
            {
                lvi_.setText( QString::number( *static_cast< double* >( data ) ) );
                break;
            }
            default:
                assert( false );
                break;
        }
    }

    void SetDataChanged(const QString& text)
    {
        switch ( eCvt_ )
        {
            case ADN_ListViewItem::eString:
            {
                std::string string = text.toStdString();
                emit DataChanged( &string );
                break;
            }
            case ADN_ListViewItem::eInt:
            {
                int newval = text.toInt();
                emit DataChanged( &newval );
                break;
            }
            case ADN_ListViewItem::eDouble:
            {
                double newval = text.toDouble();
                emit DataChanged( &newval );
                break;
            }
            default:
                break;
        }
    }

private:
    ADN_ListViewItem_Connector& operator=( const ADN_ListViewItem_Connector& );

private:
    ADN_ListViewItem&           lvi_;
    ADN_ListViewItem::E_TypeCvt eCvt_;
};

//-----------------------------------------------------------------------------
// Name: ADN_ListViewItem constructor
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
ADN_ListViewItem::ADN_ListViewItem( void* item )
    : pData_( item )
    , connector_( 0 )
{
    setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
    setData( *new QVariant( gui::StandardModel::showValue_ ), gui::Roles::FilterRole );
    setData( *new QVariant(), gui::Roles::DataRole );
    setData( *new QVariant(), gui::Roles::SafeRole );
    setData( *new QVariant(), gui::Roles::MimeTypeRole );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListViewItem destructor
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
ADN_ListViewItem::~ADN_ListViewItem()
{
    pData_=0;
    delete connector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListViewItem::Connect
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_ListViewItem::Connect( ADN_Connector_ABC* data, E_TypeCvt cvt )
{
    // create new internal connector
    assert ( connector_ == 0 );

    connector_ = new ADN_ListViewItem_Connector( *this, cvt, "ADN_ListViewItem_Connector" );
    assert( connector_ );

    // connect internal connect and external data
    connector_->Connect( data );
}
