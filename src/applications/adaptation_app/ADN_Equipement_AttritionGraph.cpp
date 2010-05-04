// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipement_AttritionGraph.h"
#include "ADN_Connector_Vector_ABC.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Equipement_GUI.h"
#include "ADN_Tr.h"
#include "moc_ADN_Equipement_AttritionGraph.cpp"

//-----------------------------------------------------------------------------
// Internal Canvas connector 
//-----------------------------------------------------------------------------
class ADN_Connector_AttritionGraph
    : public ADN_Connector_Vector_ABC
{
    MT_COPYNOTALLOWED( ADN_Connector_AttritionGraph )

public:
    ADN_Connector_AttritionGraph( ADN_Equipement_AttritionGraph* graph )
        : ADN_Connector_Vector_ABC()
        , graph_( graph )
    {}

    virtual ~ADN_Connector_AttritionGraph()
    {}

    bool AddItemPrivate( void* pItem, bool /*bCreateCommand*/ = false )
    {
        if( pItem == 0 )
            return false;

        graph_->AddAttrition( *( ADN_Equipement_Data::AttritionInfos* ) pItem );
        return true;
    }

    void ClearPrivate( bool /*bInConnection*/ )
    {
        graph_->ClearAttritions();
    }

    bool RemItemPrivate( void* pItem, bool /*bCreateCommand*/ = false )
    {
        graph_->RemoveAttrition( *( ADN_Equipement_Data::AttritionInfos* ) pItem );
        return true;
    }

    void ConnectPrivateSub( ADN_Connector_Vector_ABC* pDataConnector )
    {
        ADN_Connector_ABC::ConnectPrivateSub( ( ADN_Connector_ABC* )pDataConnector );

        pDataConnector->Initialize( *this );
        connect( pDataConnector, SIGNAL( ItemAdded( void* ) ), this, SLOT( AddItemNoEmit( void* ) ) );
        connect( pDataConnector, SIGNAL( ItemRemoved( void* ) ), this, SLOT( RemItemNoEmit( void* ) ) );
    }

    void DisconnectPrivateSub( ADN_Connector_Vector_ABC* pDataConnector )
    {
        ADN_Connector_ABC::DisconnectPrivateSub( ( ADN_Connector_ABC* )pDataConnector );

        disconnect( pDataConnector, SIGNAL( ItemAdded( void* ) ), this, SLOT( AddItemNoEmit( void* ) ) );
        disconnect( pDataConnector, SIGNAL( ItemRemoved( void* ) ), this, SLOT( RemItemNoEmit( void* ) ) );
    }

private:
    ADN_Equipement_AttritionGraph* graph_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AttritionGraph constructor
// Created: JSR 2010-04-29
// -----------------------------------------------------------------------------
ADN_Equipement_AttritionGraph::ADN_Equipement_AttritionGraph( QWidget* pParent /*= 0*/ )
    : QWidget( pParent )
{
    setMinimumWidth( 500 );
    setMinimumHeight( 300 );

    pConnector_ = new ADN_Connector_AttritionGraph( this );

    QColor color;
    color.setHsv( 0, 30, 255 );
    effectColors_.push_back( color );
    color.setHsv( 60, 30, 255 );
    effectColors_.push_back( color );
    color.setHsv( 120, 30, 255 );
    effectColors_.push_back( color );
    effectStrings_.push_back( tr( "Dead" ) );
    effectStrings_.push_back( tr( "Wounded" ) );
    effectStrings_.push_back( tr( "Unwounded" ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AttritionGraph destructor
// Created: JSR 2010-04-29
// -----------------------------------------------------------------------------
ADN_Equipement_AttritionGraph::~ADN_Equipement_AttritionGraph()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AttritionGraph::AddAttrition
// Created: JSR 2010-04-29
// -----------------------------------------------------------------------------
void ADN_Equipement_AttritionGraph::AddAttrition( ADN_Equipement_Data::AttritionInfos& info )
{
    attritions_.push_back( &info );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AttritionGraph::RemoveAttrition
// Created: JSR 2010-04-30
// -----------------------------------------------------------------------------
void ADN_Equipement_AttritionGraph::RemoveAttrition( ADN_Equipement_Data::AttritionInfos& info )
{
    for( IT_Attritions it = attritions_.begin(); it != attritions_.end(); ++it )
        if( *it == &info )
        {
            attritions_.erase( it );
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AttritionGraph::ClearAttritions
// Created: JSR 2010-04-29
// -----------------------------------------------------------------------------
void ADN_Equipement_AttritionGraph::ClearAttritions()
{
    attritions_.clear();
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AttritionGraph::Update
// Created: JSR 2010-04-30
// -----------------------------------------------------------------------------
void ADN_Equipement_AttritionGraph::Update()
{
    ADN_Categories_Data::ArmorInfos* info = ADN_Workspace::GetWorkspace().GetEquipements().GetGui().GetSelectedArmor();
    if( info == 0 )
        return;

    double urbanProtection = 0.;
    ADN_Equipement_Data::UrbanAttritionInfos* material = ADN_Workspace::GetWorkspace().GetEquipements().GetGui().GetSelectedMaterial();
    if( material )
        urbanProtection = material->rCoeff_.GetData();

    for( IT_Attritions it = attritions_.begin(); it != attritions_.end(); ++it )
        if( (*it)->ptrArmor_.GetData()->strName_.GetData() == info->strName_.GetData() )
        {
            columns_.clear();

            ADN_Equipement_Data::AttritionInfos& attrition = **it;
            double rReparableWithoutEvacuation = attrition.rRepairNoEvac_.GetData();
            double rReparableWithEvacuation = attrition.rRepairWithEvac_.GetData();
            double rDestroyed = attrition.rDestroy_.GetData();

            // adapted from PHY_AttritionData::ComputeComposanteState
            double rReductionDestroyed = rDestroyed * urbanProtection;
            rDestroyed -= rReductionDestroyed;

            double rReductionReparableWithEvacuation = ( rReparableWithEvacuation + rReductionDestroyed ) * urbanProtection;
            rReparableWithEvacuation -= rReductionReparableWithEvacuation - rReductionDestroyed;
   
            double rReductionReparableWithoutEvacuation = ( rReparableWithoutEvacuation + rReductionReparableWithEvacuation ) * urbanProtection;
            rReparableWithoutEvacuation -= rReductionReparableWithoutEvacuation - rReductionReparableWithEvacuation;

            GraphData visuUndamaged( 3 );
            visuUndamaged.strName_ = tr( "Undamaged" );
            GraphData visuNoEvac( 3 );
            visuNoEvac.strName_ = ADN_Tr::ConvertFromEquipmentState( eEquipmentState_FixableInPlace ).c_str();
            GraphData visuWithEvac( 3 );
            visuWithEvac.strName_ = ADN_Tr::ConvertFromEquipmentState( eEquipmentState_FixableWithEvac ).c_str();
            GraphData visuDestroyed( 3 );
            visuDestroyed.strName_ = ADN_Tr::ConvertFromEquipmentState( eEquipmentState_Destroyed ).c_str();

            visuUndamaged.value_ = ( int )( 100.0 - rDestroyed - rReparableWithoutEvacuation - rReparableWithEvacuation );
            visuNoEvac.value_ = ( int )rReparableWithoutEvacuation;
            visuWithEvac.value_ = ( int )rReparableWithEvacuation;
            visuDestroyed.value_ = ( int )rDestroyed;

            for( ADN_Type_Vector_ABC< ADN_Categories_Data::AttritionEffectOnHuman >::CIT_PtrVector effect = info->vAttritionEffects_.begin(); effect != info->vAttritionEffects_.end(); ++effect )
            {
                int injured = ( *effect )->nInjuredPercentage_.GetData();
                int dead = ( *effect )->nDeadPercentage_.GetData();
                int safe = 100 - injured - dead;
                GraphData* localData = 0;
                switch( ( *effect )->nEquipmentState_.GetData() )
                {
                case eEquipmentState_Destroyed:
                    localData = &visuDestroyed;
                    break;
                case eEquipmentState_FixableWithEvac:
                    localData = &visuWithEvac;
                    break;
                case eEquipmentState_FixableInPlace:
                    localData = &visuNoEvac;
                    break;
                default:
                    throw std::exception( "Bad Equipment State" );
                    break;
                }
                localData->values_[ 0 ] = dead;
                localData->values_[ 1 ] = injured;
                localData->values_[ 2 ] = safe;
            }

            columns_.push_back( visuUndamaged );
            columns_.push_back( visuNoEvac );
            columns_.push_back( visuWithEvac );
            columns_.push_back( visuDestroyed );
            
            update();
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_AttritionGraph::paintEvent
// Created: JSR 2010-04-29
// -----------------------------------------------------------------------------
void ADN_Equipement_AttritionGraph::paintEvent( QPaintEvent* )
{
    QPainter painter( this );

    painter.setPen( Qt::NoPen );
    painter.setBrush( Qt::white );
    painter.drawRect( 0, 0, width(), height() );

    if( !isEnabled() )
        return;

    painter.setFont( QFont( "helvetica", 8, QFont::DemiBold ) );

    static const int fh = fontMetrics().height();
    static const int IntervalWidth = 5;
    static const int LeftMargin = 100;
    static const int Margin = 5;

    const int graphHeight = height() - fh - 2 * Margin;
    const double ratio = (double) graphHeight / 100.;

    if( effectStrings_.size() != effectColors_.size() )
        throw std::exception( "Attrition graph: Strings and Colors not coherent" );

    // Legends with color
    int index = 0;
    std::vector< QColor >::iterator itColor = effectColors_.begin();
    for( std::vector< QString >::iterator itString = effectStrings_.begin(); itString != effectStrings_.end(); ++itString, ++itColor )
    {
        int y = Margin + ( fh + Margin ) * index++;
        painter.setPen( QColor( 64, 64, 64 ) );
        painter.setBrush( *itColor );
        painter.drawRect( Margin, y, fh, fh );

        QRect rc( Margin + fh + Margin, y, LeftMargin - 2 * Margin - fh, fh );
        painter.drawText( rc, Qt::AlignLeft, *itString );
    }

    unsigned int columnWidth = ( width() - LeftMargin - Margin - ( columns_.size() - 1 ) * IntervalWidth ) / columns_.size();
    unsigned int columnIndex = 0;
    for( IT_Columns column = columns_.begin(); column != columns_.end(); ++column )
    {
        int columnX = LeftMargin + ( columnWidth + IntervalWidth ) * columnIndex++;

        // Legends
        QRect rc( columnX, Margin + graphHeight, columnWidth, height() - graphHeight - 2 * Margin );
        painter.setPen( QColor( 64, 64, 64 ) );
        painter.setBrush( Qt::NoBrush );
        painter.setFont( QFont( "helvetica", 6, QFont::DemiBold ) );
        painter.drawText( rc, Qt::AlignCenter, column->strName_ );

        unsigned int columnHeight = ( unsigned int )( column->value_ * ratio );
        if( columnHeight == 0 )
        {
            painter.setPen( Qt::darkGray );
            painter.drawLine( columnX, graphHeight + Margin
                            , columnX + columnWidth, graphHeight + Margin );
        }
        else
        {
            // Frame
            painter.setPen( Qt::lightGray );
            painter.drawLine( columnX, graphHeight - columnHeight + Margin, columnX + columnWidth, graphHeight - columnHeight + Margin );
            painter.drawLine( columnX, graphHeight - columnHeight + Margin, columnX, graphHeight + Margin );
            painter.setPen( Qt::gray );
            painter.drawLine( columnX + 1, graphHeight - columnHeight + Margin + 1, columnX + columnWidth - 1, graphHeight - columnHeight + Margin + 1);
            painter.drawLine( columnX + 1, graphHeight - columnHeight + Margin + 1, columnX + 1, graphHeight + Margin -1 );
            painter.setPen( Qt::darkGray );
            painter.drawLine( columnX, graphHeight + Margin, columnX + columnWidth, graphHeight + Margin );
            painter.drawLine( columnX + columnWidth, graphHeight - columnHeight + Margin, columnX + columnWidth, graphHeight + Margin );
            painter.drawLine( columnX + 1, graphHeight + Margin - 1, columnX + columnWidth - 1, graphHeight + Margin - 1 );
            painter.drawLine( columnX + columnWidth - 1, graphHeight - columnHeight + Margin + 1, columnX + columnWidth - 1, graphHeight + Margin - 1 );

            unsigned int cumulatedHeight = 0;
            int colorIndex = -1;

            const double vRatio = ( double ) ( columnHeight - 4 ) / 100.;

            for( GraphData::IT_Values values = column->values_.begin(); values != column->values_.end(); ++values )
            {
                colorIndex = ( colorIndex + 1 ) % effectColors_.size();
                if( *values == 0 )
                    continue;

                unsigned int lineHeight = ( unsigned int )( *values * vRatio ) + 1;
                lineHeight = std::min( lineHeight, columnHeight - cumulatedHeight - 3 );

                painter.setPen( Qt::NoPen );
                painter.setBrush( effectColors_[ colorIndex ] );

                painter.drawRect( columnX + 2
                                , graphHeight - columnHeight + cumulatedHeight + Margin + 2
                                , columnWidth - 3
                                , lineHeight );

                cumulatedHeight += lineHeight;            
            }
        }
    }
}
