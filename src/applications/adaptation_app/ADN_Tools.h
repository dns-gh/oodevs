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
#include "ADN_Enums.h"

template< typename T > class ADN_Type_ABC;
typedef ADN_Type_ABC< std::string > ADN_Type_Time;

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
    bool FieldCompare( const T& element, boost::function< const typename ADN_Type_ABC< FieldType >& ( const T& ) > fieldExtractor, const FieldType& value )
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
    void SortMenu( Q3PopupMenu& menu );
    std::string MakePluralFromEntityType( E_EntityType type );

    // -----------------------------------------------------------------------------
    E_EntityType ConvertMissionToEntityType( E_MissionType type );
    E_MissionType ConvertEntityTypeToMissionType( E_EntityType type );

    // -----------------------------------------------------------------------------
    // Delay & time conversions
    // -----------------------------------------------------------------------------
    QString GetLocalFormatWithoutSeconds();
    QString ConvertLocalTimeToXmlTime( const QString& localTime );
    QString ConvertXmlTimeToLocalTime( const QString& xmlTime );

    QString BuildLongString( const std::string& hours, const std::string& minutes,
                             const std::string& seconds, const std::string& microseconds,
                             const std::string& decimalPoint );

    QString ConvertDelayToLongString( const QString& delay );           // from 12m / 12s / 12.3s to HH:MM:SS.MS
    QString ConvertLongStringToDelay( const QString& text );          // from HH:MM:SS.MS to 720s / 12s / 12.3s

    int ConvertDelayToCentiseconds( const QString& delay );             // from 12m / 12s / 12.3s to 720. / 12. / 12.3
    QString ConvertCentisecondsToDelay( int value );                  // from 720. / 12. / 12.3 to 720s / 12s / 12.3s
    bool IsNullDelay( const ADN_Type_Time& delay );

    QString ConvertCentisecondsToLongString( int );                     //
    int ConvertLongStringToCentiseconds( const QString& text );         //

    // -----------------------------------------------------------------------------
    // connector helpers
    // -----------------------------------------------------------------------------
    void CheckConnectorVector( const T_ConnectorVector& vConnectors, uint nExpectedSize );
    void SetAutoClear( T_ConnectorVector& v, bool b );

}

#endif // __ADN_Tools_h_
