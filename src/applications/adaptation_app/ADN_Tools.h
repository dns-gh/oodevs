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
enum E_UnitPosture;
enum E_TempsBordee;

class QPopupMenu;


// =============================================================================
/** @class  ADN_Tools
    @brief  ADN_Tools
*/
// Created: APE 2005-03-09
// =============================================================================
class ADN_Tools
{
public:
    template< typename T >
    class NameCmp : public std::unary_function< T* , bool >
    {
    public:
        NameCmp( const std::string& strName ) : strName_( strName ) {}
        ~NameCmp() {}

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


public:
    static std::string Replace( const std::string& str, char cSrc, char cDst );
    static bool CaselessCompare( const std::string& str1, const std::string& str2 );

    static void CreatePathToFile( const std::string& strFilePath );
    static bool CopyFileToFile( const std::string& strSrc, const std::string& strDest );

    static std::string Scriptify( const std::string& strFieldName );
    static std::string ComputePostureScriptName( E_UnitPosture nPosture );

    static void CheckConnectorVector( const T_ConnectorVector& vConnectors, uint nExpectedSize );

    static std::string SecondToString( double rSecond );

    static void SortMenu( QPopupMenu& menu );
};


#endif // __ADN_Tools_h_