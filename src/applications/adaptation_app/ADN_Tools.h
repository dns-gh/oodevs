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

#ifndef __ADN_Tools_h_
#define __ADN_Tools_h_

#include "ADN_Connector_ABC.h"
#include "ADN_VectorEditionDialog.h"
#include "ADN_Types.h"

namespace xml
{
    class xostream;
}

template< typename T > class ADN_Type_Vector_ABC;

enum E_UnitPosture;
enum E_TempsBordee;

class Q3PopupMenu;

// =============================================================================
/** @class  ADN_Tools
    @brief  ADN_Tools
*/
// Created: APE 2005-03-09
// =============================================================================
namespace ADN_Tools
{
    // -----------------------------------------------------------------------------
    template< typename T, typename FieldType >
    bool FieldCompare( const T& element, boost::function< const ADN_Type_ABC< FieldType >& ( const T& ) > fieldExtractor, const FieldType& value )
    {
        return fieldExtractor( element ) == value;
    }

    // -----------------------------------------------------------------------------
    template< typename Data >
    ADN_Type_ABC< std::string >& NameExtractor( Data& data )
    {
        return data.strName_;
    }

    template< typename Data >
    const ADN_Type_ABC< std::string >& NameExtractor( const Data& data )
    {
        return data.strName_;
    }

    template< typename Data >
    const ADN_Type_ABC< std::string >& CrossedRefNameExtractor( const Data& data )
    {
        assert( data.GetCrossedElement() != 0 );
        return data.GetCrossedElement()->strName_;
    }

    template< typename Data >
    const ADN_Type_ABC< int >& IdExtractor( const Data& data )
    {
        return data.id_;
    }

    template< typename Data >
    const ADN_Type_ABC< int >& NIdExtractor( const Data& data )
    {
        return data.nId_;
    }

    // -----------------------------------------------------------------------------
    template< typename T >
    bool NameCompare( const T* element, const std::string& name )
    {
        assert( element != 0 );
        return FieldCompare< T, std::string >( *element, boost::bind( &ADN_Tools::NameExtractor< T >, boost::cref( *element ) ), boost::cref( name ) );
    }

    template< typename T >
    bool CrossedRefNameCompare( const T* element, const std::string& name )
    {
        assert( element != 0 );
        return FieldCompare< T, std::string >( *element, boost::bind( &ADN_Tools::CrossedRefNameExtractor< T >, boost::cref( *element ) ), boost::cref( name ) );
    }

    template< typename T >
    bool IdCompare( const T* element, const int& id )
    {
        assert( element != 0 );
        return FieldCompare< T, int >( *element, boost::bind( &ADN_Tools::IdExtractor< T >, boost::cref( *element ) ), boost::cref( id ) );
    }

    template< typename T >
    bool NIdCompare( const T* element, const int& id )
    {
        assert( element != 0 );
        return FieldCompare< T, int >( *element, boost::bind( &ADN_Tools::NIdExtractor< T >, boost::cref( *element ) ), boost::cref( id ) );
    }

    // -----------------------------------------------------------------------------
    class NameCmp
    {
    public:
                 NameCmp( const std::string& strName ) : strName_( strName ) {}
        virtual ~NameCmp() {}

        template < typename T >
        bool operator()( T* infos ) const
        {
            return ( infos != 0 ) ? infos->strName_ == strName_ : false;
        }

    private:
        std::string strName_;
    };

    template< class DataClass >
    struct NameSort
    {
        bool operator()( DataClass* pLHS, DataClass* pRHS )
        {
            return pLHS->strName_.GetData().compare( pRHS->strName_.GetData() ) < 0;
        }
    };

    // -----------------------------------------------------------------------------
    std::string Scriptify( const std::string& strFieldName );
    std::string ComputePostureScriptName( E_UnitPosture nPosture );

    void CheckConnectorVector( const T_ConnectorVector& vConnectors, uint nExpectedSize );
    void SortMenu( Q3PopupMenu& menu );

    // -----------------------------------------------------------------------------
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

#endif // __ADN_Tools_h_
