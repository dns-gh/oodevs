// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Resources_AttritionGraph.h"
#include "moc_ADN_Resources_AttritionGraph.cpp"
#include "ADN_Connector_Vector_ABC.h"
#include "ADN_Resources_Data.h"
#include "ADN_Logistic_Data.h"
#include "ADN_Health_Data.h"
#include "ADN_Resources_GUI.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"
#include <boost/noncopyable.hpp>

//-----------------------------------------------------------------------------
// Internal Canvas connector
//-----------------------------------------------------------------------------
class ADN_Connector_AttritionGraph : public ADN_Connector_Vector_ABC
                                   , private boost::noncopyable
{

public:
    explicit ADN_Connector_AttritionGraph( ADN_Resources_AttritionGraph* graph )
        : ADN_Connector_Vector_ABC()
        , graph_( graph )
    {}

    virtual ~ADN_Connector_AttritionGraph()
    {}

    bool AddItemPrivate( void* pItem )
    {
        if( pItem == 0 )
            return false;

        graph_->AddAttrition( *( helpers::AttritionInfos* ) pItem );
        return true;
    }

    void ClearPrivate( bool /*bInConnection*/ )
    {
        graph_->ClearAttritions();
    }

    bool RemItemPrivate( void* pItem )
    {
        graph_->RemoveAttrition( *( helpers::AttritionInfos* ) pItem );
        ADN_Workspace::GetWorkspace().SetMainWindowModified( true );
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
    ADN_Resources_AttritionGraph* graph_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Resources_AttritionGraph constructor
// Created: JSR 2010-04-29
// -----------------------------------------------------------------------------
ADN_Resources_AttritionGraph::ADN_Resources_AttritionGraph( QWidget* pParent /* = 0*/ )
    : QWidget( pParent )
{
    setMinimumWidth( 500 );
    setMinimumHeight( 300 );

    pConnector_.reset( new ADN_Connector_AttritionGraph( this ) );

    QColor color;
    color.setHsv( 0, 120, 255 );
    effectColors_.push_back( color );
    color.setHsv( 20, 120, 255 );
    effectColors_.push_back( color );
    color.setHsv( 35, 120, 255 );
    effectColors_.push_back( color );
    color.setHsv( 50, 120, 255 );
    effectColors_.push_back( color );
    color.setHsv( 60, 120, 255 );
    effectColors_.push_back( color );
    color.setHsv( 120, 120, 255 );
    effectColors_.push_back( color );
    effectStrings_.push_back( tr( "Dead" ) );
    effectStrings_.push_back( tr( "Wounded (%1)" ).arg( ADN_Tr::ConvertFromDoctorSkills( eDoctorSkills_UE ).c_str() ) );
    effectStrings_.push_back( tr( "Wounded (%1)" ).arg( ADN_Tr::ConvertFromDoctorSkills( eDoctorSkills_U1 ).c_str() ) );
    effectStrings_.push_back( tr( "Wounded (%1)" ).arg( ADN_Tr::ConvertFromDoctorSkills( eDoctorSkills_U2 ).c_str() ) );
    effectStrings_.push_back( tr( "Wounded (%1)" ).arg( ADN_Tr::ConvertFromDoctorSkills( eDoctorSkills_U3 ).c_str() ) );
    effectStrings_.push_back( tr( "Unwounded" ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_AttritionGraph destructor
// Created: JSR 2010-04-29
// -----------------------------------------------------------------------------
ADN_Resources_AttritionGraph::~ADN_Resources_AttritionGraph()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_AttritionGraph::AddAttrition
// Created: JSR 2010-04-29
// -----------------------------------------------------------------------------
void ADN_Resources_AttritionGraph::AddAttrition( helpers::AttritionInfos& info )
{
    attritions_.push_back( &info );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_AttritionGraph::RemoveAttrition
// Created: JSR 2010-04-30
// -----------------------------------------------------------------------------
void ADN_Resources_AttritionGraph::RemoveAttrition( helpers::AttritionInfos& info )
{
    for( auto it = attritions_.begin(); it != attritions_.end(); ++it )
        if( *it == &info )
        {
            attritions_.erase( it );
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_AttritionGraph::ClearAttritions
// Created: JSR 2010-04-29
// -----------------------------------------------------------------------------
void ADN_Resources_AttritionGraph::ClearAttritions()
{
    attritions_.clear();
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_AttritionGraph::Update
// Created: JSR 2010-04-30
// -----------------------------------------------------------------------------
void ADN_Resources_AttritionGraph::Update()
{
    ADN_Armors_Data::ArmorInfos* info = ADN_Workspace::GetWorkspace().GetResources().GetGui().GetSelectedArmor();
    if( info == 0 )
        return;

    if( info->nType_ == eProtectionType_Crowd )
        return;

    helpers::ADN_UrbanAttritionInfos* material = ADN_Workspace::GetWorkspace().GetResources().GetGui().GetSelectedMaterial();
    const double urbanProtection = material ? material->rCoeff_.GetData() : 0.;

    ADN_Health_Data::WoundInfo* wounds = ADN_Workspace::GetWorkspace().GetLogistic().GetData().GetElement< ADN_Health_Data >( eHealth ).wounds;

    for( auto it = attritions_.begin(); it != attritions_.end(); ++it )
        if( (*it)->GetCrossedElement() && ( *it )->GetCrossedElement()->strName_.GetData() == info->strName_.GetData() )
        {
            columns_.clear();

            helpers::AttritionInfos& attrition = **it;
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

            GraphData visuUndamaged( 6 );
            visuUndamaged.strName_ = tr( "Undamaged" );
            GraphData visuNoEvac( 6 );
            visuNoEvac.strName_ = ADN_Tr::ConvertFromEquipmentState_ADN( eEquipmentState_ADN_FixableInPlace, ENT_Tr::eToTr ).c_str();
            GraphData visuWithEvac( 6 );
            visuWithEvac.strName_ = ADN_Tr::ConvertFromEquipmentState_ADN( eEquipmentState_ADN_FixableWithEvac, ENT_Tr::eToTr ).c_str();
            GraphData visuDestroyed( 6 );
            visuDestroyed.strName_ = ADN_Tr::ConvertFromEquipmentState_ADN( eEquipmentState_ADN_Destroyed, ENT_Tr::eToTr ).c_str();

            visuUndamaged.value_ = ( int )( 100.0 - rDestroyed - rReparableWithoutEvacuation - rReparableWithEvacuation );
            visuNoEvac.value_ = ( int )rReparableWithoutEvacuation;
            visuWithEvac.value_ = ( int )rReparableWithEvacuation;
            visuDestroyed.value_ = ( int )rDestroyed;

            for( auto effect = info->vAttritionEffects_.begin(); effect != info->vAttritionEffects_.end(); ++effect )
            {
                int injured = ( *effect )->nInjuredPercentage_.GetData();
                int dead = ( *effect )->nDeadPercentage_.GetData();
                GraphData* localData = 0;
                switch( ( *effect )->nEquipmentState_.GetData() )
                {
                case eEquipmentState_ADN_Destroyed:
                    localData = &visuDestroyed;
                    break;
                case eEquipmentState_ADN_FixableWithEvac:
                    localData = &visuWithEvac;
                    break;
                case eEquipmentState_ADN_FixableInPlace:
                    localData = &visuNoEvac;
                    break;
                default:
                    throw MASA_EXCEPTION( "Bad Equipment State" );
                    break;
                }

                localData->values_[ 0 ] = dead;
                localData->values_[ 1 ] = injured * wounds[ 0 ].rPercentage_.GetData() / 100.;
                localData->values_[ 2 ] = injured * wounds[ 1 ].rPercentage_.GetData() / 100.;
                localData->values_[ 3 ] = injured * wounds[ 2 ].rPercentage_.GetData() / 100.;
                localData->values_[ 4 ] = injured * wounds[ 3 ].rPercentage_.GetData() / 100.;
                localData->values_[ 5 ] = 100;
                for( int i = 0; i < 5; ++i )
                    localData->values_[ 5 ] -= localData->values_[ i ];
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
// Name: ADN_Resources_AttritionGraph::GetTextTooltip
// Created: JSR 2010-06-01
// -----------------------------------------------------------------------------
QString ADN_Resources_AttritionGraph::GetTextTooltip( const QPoint& point, QRect& rc ) const
{
    for( auto column = columns_.begin(); column != columns_.end(); ++column )
    {
        auto value = column->values_.begin();
        auto string = effectStrings_.begin();

        for( auto rect = column->rectangles_.begin(); rect != column->rectangles_.end(); ++rect, ++value, ++string )
        {
            if( rect->contains( point ) )
            {
                rc = *rect;
                double val = *value * column->value_ * 0.01;
                QString str = QString( "%1:\n %2 % \n%3:\n %4 %" ).arg( column->strName_ )
                    .arg( column->value_ ).arg( *string ).arg( val, 0, 'g', 3 );
                return str;
            }
        }
    }

    return QString();
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_AttritionGraph::event
// Created: JSR 2010-04-29
// -----------------------------------------------------------------------------
bool ADN_Resources_AttritionGraph::event( QEvent* e )
{
    if( e->type() == QEvent::ToolTip )
    {
        QHelpEvent* help = dynamic_cast< QHelpEvent* >( e );
        QRect rc;
        const QString s = this->GetTextTooltip( help->pos(), rc );
        if( !s.isEmpty() )
        {
            QToolTip::showText( help->globalPos(), s, this, rc );
            return true;
        }
    }
    return QWidget::event( e );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_AttritionGraph::paintEvent
// Created: JSR 2010-04-29
// -----------------------------------------------------------------------------
void ADN_Resources_AttritionGraph::paintEvent( QPaintEvent* )
{
    QPainter painter( this );

    painter.setPen( Qt::NoPen );
    painter.setBrush( Qt::white );
    painter.drawRect( 0, 0, width(), height() );

    if( !isEnabled() || columns_.empty() )
        return;

    painter.setFont( QFont( "helvetica", 8, QFont::DemiBold ) );

    static const int fh = fontMetrics().height();
    static const int IntervalWidth = 5;
    static const int LeftMargin = 120;
    static const int Margin = 5;

    const int graphHeight = height() - fh - 2 * Margin;
    const double ratio = (double) graphHeight / 100.;

    if( effectStrings_.size() != effectColors_.size() )
        throw MASA_EXCEPTION( "Attrition graph: Strings and Colors not coherent" );

    // Legends with color
    int index = 0;
    std::vector< QColor >::iterator itColor = effectColors_.begin();
    for( auto itString = effectStrings_.begin(); itString != effectStrings_.end(); ++itString, ++itColor )
    {
        int y = Margin + ( fh + Margin ) * index++;
        painter.setPen( QColor( 64, 64, 64 ) );
        painter.setBrush( *itColor );
        painter.drawRect( Margin, y, fh, fh );

        QRect rc( Margin + fh + Margin, y, LeftMargin - 2 * Margin - fh, fh );
        painter.drawText( rc, Qt::AlignLeft, *itString );
    }

    unsigned int columnWidth = static_cast< unsigned int >( ( width() - LeftMargin - Margin - ( columns_.size() - 1 ) * IntervalWidth ) / columns_.size() );
    unsigned int columnIndex = 0;
    for( auto column = columns_.begin(); column != columns_.end(); ++column )
    {
        int columnX = LeftMargin + ( columnWidth + IntervalWidth ) * columnIndex++;

        // Legends
        QRect rc( columnX, Margin + graphHeight, columnWidth, height() - graphHeight - 2 * Margin );
        painter.setPen( QColor( 64, 64, 64 ) );
        painter.setBrush( Qt::NoBrush );
        painter.setFont( QFont( "helvetica", 6, QFont::DemiBold ) );
        painter.drawText( rc, Qt::AlignCenter, column->strName_ );

        unsigned int columnHeight = ( unsigned int )( column->value_ * ratio );
        if( columnHeight < 4 )
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

            column->rectangles_.clear();

            for( auto values = column->values_.begin(); values != column->values_.end(); ++values )
            {
                colorIndex = ( colorIndex + 1 ) % effectColors_.size();

                double value = *values * vRatio;
                if( value < 0.5 )
                {
                    column->rectangles_.push_back( QRect() );
                    continue;
                }

                unsigned int lineHeight = unsigned int( value ) + 1;
                lineHeight = std::min( lineHeight, columnHeight - cumulatedHeight - 3 );

                painter.setPen( Qt::NoPen );
                painter.setBrush( effectColors_[ colorIndex ] );

                QRect rc( columnX + 2
                                , graphHeight - columnHeight + cumulatedHeight + Margin + 2
                                , columnWidth - 3
                                , lineHeight );
                column->rectangles_.push_back( rc );

                painter.drawRect( rc );

                cumulatedHeight += lineHeight;
            }
        }
    }
}
