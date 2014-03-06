// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "QuotasEditor.h"
#include "moc_QuotasEditor.cpp"
#include "QuotasResourcesTable.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/LogisticHierarchiesBase.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/LogisticSupplyClass.h"
#include "clients_kernel/ObjectTypes.h"
#include "preparation/Dotation.h"
#include "preparation/LogisticBaseStates.h"
#include "preparation/StaticModel.h"

Q_DECLARE_METATYPE( const kernel::Entity_ABC* )

// -----------------------------------------------------------------------------
// Name: QuotasEditor constructor
// Created: JSR 2014-03-05
// -----------------------------------------------------------------------------
QuotasEditor::QuotasEditor( QWidget* parent, const StaticModel& staticModel )
    : QWidget( parent )
    , supplyClasses_( staticModel.objectTypes_ )
{
    subordinateCombo_ = new QComboBox( this );
    quotasTableView_ = new QuotasResourcesTable( "quotasTable", this, staticModel.objectTypes_ );

    QHBoxLayout* subLayout = new QHBoxLayout();
    subLayout->addWidget( new QLabel( tr( "Logistics subordinate:" ) ) );
    subLayout->addWidget( subordinateCombo_ );
    subLayout->setStretch( 0, 1 );
    subLayout->setStretch( 1, 3 );

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout( subLayout );
    mainLayout->addWidget( quotasTableView_ );
    setLayout( mainLayout );

    connect( subordinateCombo_, SIGNAL( activated( int ) ), SLOT( OnSubordinateChanged( int ) ) );
    connect( quotasTableView_, SIGNAL( ResourceValueChanged() ), SLOT( NotifyQuotasUserChange() ) );
}

// -----------------------------------------------------------------------------
// Name: QuotasEditor destructor
// Created: JSR 2014-03-05
// -----------------------------------------------------------------------------
QuotasEditor::~QuotasEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: QuotasEditor::Initialize
// Created: JSR 2014-03-05
// -----------------------------------------------------------------------------
void QuotasEditor::Initialize( const kernel::Entity_ABC& entity )
{
    quotasByEntity_.clear();
    subordinateCombo_->clear();
    quotasTableView_->OnClearItems();

    const gui::LogisticHierarchiesBase* pLogHierarchy = entity.Retrieve< gui::LogisticHierarchiesBase >();
    if( !pLogHierarchy )
        return;
    tools::Iterator< const kernel::Entity_ABC& > itChildren = pLogHierarchy->CreateSubordinateIterator();
    while( itChildren.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = itChildren.NextElement();
        if( logistic_helpers::IsLogisticBase( child ) )
        {
            const LogisticBaseStates* bl = static_cast< const LogisticBaseStates* >( child.Retrieve< gui::LogisticHierarchiesBase >() );
            if( !bl )
                continue;
            subordinateCombo_->addItem( child.GetName(), QVariant::fromValue( &child ) );
            auto dotationIt = static_cast< const tools::Resolver< Dotation >* >( bl )->CreateIterator();
            while( dotationIt.HasMoreElements() )
            {
                const auto& curDotation = dotationIt.NextElement();
                quotasByEntity_[ &child ][ &curDotation.type_ ] += curDotation.quantity_;
            }
        }
    }
    OnSubordinateChanged( subordinateCombo_->currentIndex() );
}

// -----------------------------------------------------------------------------
// Name: QuotasEditor::ApplyQuotas
// Created: JSR 2014-03-05
// -----------------------------------------------------------------------------
void QuotasEditor::ApplyQuotas( const gui::LogisticHierarchiesBase& logHierarchy, const std::map< const kernel::DotationType*, unsigned int >& generatedQuotas ) const
{
    const LogisticBaseStates* pBaseStates = dynamic_cast< const LogisticBaseStates* >( &logHierarchy );
    if( !pBaseStates )
        return;
    LogisticBaseStates& baseStates = *const_cast< LogisticBaseStates* >( pBaseStates );
    baseStates.ClearDotations();
    for( auto itRequired = generatedQuotas.begin(); itRequired != generatedQuotas.end(); ++itRequired )
    {
        if( itRequired->second <= 0 )
            continue;
        const kernel::DotationType& dotationType = *itRequired->first;
        if( supplyClasses_.Find( dotationType.GetLogisticSupplyClass().GetName() ) )
            baseStates.SetDotation( dotationType, itRequired->second );
    }
}

// -----------------------------------------------------------------------------
// Name: QuotasEditor::ApplyQuotas
// Created: JSR 2014-03-05
// -----------------------------------------------------------------------------
void QuotasEditor::ApplyQuotas() const
{
    for( auto it = quotasByEntity_.begin(); it != quotasByEntity_.end(); ++it )
        if( const gui::LogisticHierarchiesBase* pLogChildrenHierarchy = it->first->Retrieve< gui::LogisticHierarchiesBase >() )
            ApplyQuotas( *pLogChildrenHierarchy, it->second );
}

// -----------------------------------------------------------------------------
// Name: QuotasEditor::OnSubordinateChanged
// Created: JSR 2014-03-05
// -----------------------------------------------------------------------------
void QuotasEditor::OnSubordinateChanged( int index )
{
    if( index < 0 || index >= subordinateCombo_->count() )
        return;
    QVariant v = subordinateCombo_->itemData( index, Qt::UserRole );
    if( v.isValid() )
        quotasTableView_->SetQuotas( quotasByEntity_[ v.value< const kernel::Entity_ABC* >() ] );
}

// -----------------------------------------------------------------------------
// Name: QuotasEditor::NotifyQuotasUserChange
// Created: JSR 2014-03-05
// -----------------------------------------------------------------------------
void QuotasEditor::NotifyQuotasUserChange()
{
    if( subordinateCombo_->count() == 0 )
        return;
    int currentIndex = subordinateCombo_->currentIndex();
    if( currentIndex == -1 )
        return;
    auto entity = subordinateCombo_->itemData( currentIndex, Qt::UserRole ).value< const kernel::Entity_ABC* >();
    if( entity )
        quotasTableView_->ComputeValueByDotation( quotasByEntity_[ entity ] );
}

// -----------------------------------------------------------------------------
// Name: QuotasEditor::NotifyAutomaticQuotas
// Created: JSR 2014-03-05
// -----------------------------------------------------------------------------
void QuotasEditor::NotifyAutomaticQuotas( const std::map< const kernel::Entity_ABC*, std::map< const kernel::DotationType*, unsigned int > >& quotas )
{
    quotasByEntity_.clear();
    for( auto it = quotas.begin(); it != quotas.end(); ++it )
        for( auto itD = it->second.begin(); itD != it->second.end(); ++itD )
            quotasByEntity_[ it->first ][ itD->first ] = itD->second;
    OnSubordinateChanged( subordinateCombo_->currentIndex() );
}
