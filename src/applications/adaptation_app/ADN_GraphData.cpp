// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_GraphData.h"
#include "ADN_GraphValue.h"

namespace
{
    class ADN_GraphDataConnector : public ADN_Connector_Vector_ABC
                                 , private boost::noncopyable
    {
    public:
        explicit ADN_GraphDataConnector( ADN_GraphData& graphData )
            : ADN_Connector_Vector_ABC()
            , graphData_( graphData )
        {
            // NOTHING
        }

        void SetValueCreator( const std::function< void (void*) >& valueCreator )
        {
            valueCreator_ = valueCreator;
        }

        bool AddItemPrivate( void* pItem )
        {
            if( pItem == 0 || !valueCreator_ )
                return false;
            valueCreator_( pItem );
            return true;
        }

        bool RemItemPrivate( void* pItem )
        {
            graphData_.DeleteData( pItem );
            ADN_Workspace::GetWorkspace().SetMainWindowModified( true );
            return true;
        }

        void ConnectPrivateSub( ADN_Connector_Vector_ABC* pDataConnector )
        {
            ADN_Connector_ABC::ConnectPrivateSub( static_cast< ADN_Connector_ABC* >( pDataConnector ) );
            pDataConnector->Initialize( *this );
            connect( pDataConnector, SIGNAL( ItemAdded( void* ) ),      this, SLOT( AddItemNoEmit( void* ) ) );
            connect( pDataConnector, SIGNAL( ItemRemoved( void* ) ),    this, SLOT( RemItemNoEmit( void* ) ) );
        }

        void DisconnectPrivateSub( ADN_Connector_Vector_ABC* pDataConnector )
        {
            ADN_Connector_ABC::DisconnectPrivateSub( static_cast< ADN_Connector_ABC* >( pDataConnector ) );
            disconnect( pDataConnector, SIGNAL( ItemAdded( void* ) ),      this, SLOT( AddItemNoEmit( void* ) ) );
            disconnect( pDataConnector, SIGNAL( ItemRemoved( void* ) ),    this, SLOT( RemItemNoEmit( void* ) ) );
        }

    private:
        ADN_GraphData& graphData_;
        std::function< void (void*) > valueCreator_;
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphData constructor
/** @param  nUserID
*/
// Created: APE 2004-12-21
// -----------------------------------------------------------------------------
ADN_GraphData::ADN_GraphData( uint nUserID, GQ_Plot& plot )
    : GQ_PlotData( nUserID, plot )
    , pConnector_( 0 )
{
    pConnector_ = new ADN_GraphDataConnector( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphData destructor
// Created: APE 2004-12-21
// -----------------------------------------------------------------------------
ADN_GraphData::~ADN_GraphData()
{
    clear_owned_ptrs( graphValueList_ );
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphData::SetValueCreator
// Created: ABR 2014-04-08
// -----------------------------------------------------------------------------
void ADN_GraphData::SetValueCreator( const std::function< void (void*) >& valueCreator )
{
    static_cast< ADN_GraphDataConnector* >( pConnector_ )->SetValueCreator( valueCreator );
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphData::GetConnector
// Created: ABR 2014-04-08
// -----------------------------------------------------------------------------
ADN_Connector_ABC& ADN_GraphData::GetConnector() const
{
    return *pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphData::AddData
// Created: APE 2004-12-21
// -----------------------------------------------------------------------------
void ADN_GraphData::AddData( ADN_GraphValue& value )
{
    graphValueList_.push_back( &value );
    this->AddPoint( value.GetPoint() );
    TouchData();
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphData::DeleteData
/** @param  pRelatedObject
*/
// Created: APE 2004-12-21
// -----------------------------------------------------------------------------
void ADN_GraphData::DeleteData( void* pRelatedObject )
{
    for( auto it = graphValueList_.begin(); it != graphValueList_.end(); )
    {
        ADN_GraphValue& value = **it;
        // Check if the value is related to the object being deleted.
        if( value.GetRelatedObject() == pRelatedObject )
        {
            // It is. Delete the graphData point and the value.
            this->DeletePoint( static_cast< unsigned int >( std::distance( graphValueList_.begin(), it ) ) );
            it = graphValueList_.erase( it );
            delete &value;
            TouchData();
            continue;
        }
        // The value is not related to the object being deleted. Update it in case it has changed
        // as a result of having the other value deleted (for example, it's x coordinate
        // changing because the preceding value was deleted thus avoiding some ugly empty space).
        this->ChangePoint( static_cast< unsigned int >( std::distance( graphValueList_.begin(), it ) ), value.GetPoint() );
        ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphData::OnDataChanged
// Created: APE 2004-12-21
// -----------------------------------------------------------------------------
void ADN_GraphData::OnDataChanged( ADN_GraphValue& value )
{
    auto it = std::find( graphValueList_.begin(), graphValueList_.end(), &value );
    assert( it != graphValueList_.end() );
    T_Point newPoint = value.GetPoint();
    this->ChangePoint( static_cast< unsigned int >( std::distance( graphValueList_.begin(), it ) ), newPoint );
    TouchData();
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphData::SelectRelatedData
/** @param  pObj
*/
// Created: APE 2004-12-22
// -----------------------------------------------------------------------------
void ADN_GraphData::SelectRelatedData( void* pObj )
{
    SetSelected( false );
    for( auto it = graphValueList_.begin(); it != graphValueList_.end(); ++it )
        if( ( *it )->GetRelatedObject() == pObj )
            SetSelected( static_cast< unsigned int >( std::distance( graphValueList_.begin(), it ) ), true );
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphData::GetGraphValue
// Created: APE 2004-12-22
// -----------------------------------------------------------------------------
ADN_GraphValue& ADN_GraphData::GetGraphValue( uint nIndex )
{
    assert( nIndex < graphValueList_.size() );
    return * graphValueList_[nIndex];
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphData::GetDataIndex
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
int ADN_GraphData::GetDataIndex( const ADN_GraphValue& value ) const
{
    auto it = std::find( graphValueList_.begin(), graphValueList_.end(), &value );
    assert( it != graphValueList_.end() );
    return static_cast< int >( std::distance( graphValueList_.begin(), it ) );
}
