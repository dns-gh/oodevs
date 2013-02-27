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

namespace xml
{
    class xostream;
}

namespace boost
{
    namespace filesystem
    {
        class path;
    }
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
    template< typename Data >
    ADN_Type_ABC< std::string >& NameExtractor( Data& data )
    {
        return data.strName_;
    }

    template< typename T >
    class NameCmp : public std::unary_function< T* , bool >
    {
    public:
        NameCmp( const std::string& strName ) : strName_( strName ) {}
        virtual ~NameCmp() {}

        template < typename T >
        bool operator()( T* tgtnfos ) const
        { return CaselessCompare( tgtnfos->strName_.GetData(), strName_ ); }

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

    bool CaselessCompare( const std::string& str1, const std::string& str2 );

    void CreatePathToFile( const std::string& strFilePath );
    bool CopyFileToFile( const std::string& strSrc, const std::string& strDest );
    void CopyDirToDir( const boost::filesystem::path& pathFrom, const boost::filesystem::path& pathTo, bool recursive, bool forceCopy );
    void CleanDirectoryContent( const boost::filesystem::path& dir, bool recursive );

    std::string Scriptify( const std::string& strFieldName );
    std::string ComputePostureScriptName( E_UnitPosture nPosture );

    void CheckConnectorVector( const T_ConnectorVector& vConnectors, uint nExpectedSize );

    std::string SecondToString( double rSecond );

    void SortMenu( Q3PopupMenu& menu );

    void AddSchema( xml::xostream& xos, const std::string& name );

    void AddVersion( xml::xostream& xos, const std::string& name );

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
