// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoSummariesWidget_h_
#define __InfoSummariesWidget_h_

#include "tools/SelectionObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/StockAttribute_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Attributes_ABC;
    class Displayer_ABC;
    class Positions;
}

// =============================================================================
/** @class  InfoSummariesWidget
    @brief  InfoSummariesWidget
*/
// Created: SBO 2007-02-28
// =============================================================================
class InfoSummariesWidget : public Q3VBox
                          , public tools::Observer_ABC
                          , public tools::SelectionObserver< kernel::Entity_ABC >
                          , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                          , public tools::ElementObserver_ABC< kernel::Attributes_ABC >
                          , public tools::ElementObserver_ABC< kernel::Positions >
                          , public tools::ElementObserver_ABC< kernel::ConstructionAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::MineAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::ActivityTimeAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::NBCAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::BypassAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::ObstacleAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::LogisticAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::CrossingSiteAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::SupplyRouteAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::ToxicCloudAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::FireAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::BurnAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::BurnSurfaceAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::MedicalTreatmentAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::OccupantAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::DelayAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::FloodAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::StockAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::AltitudeModifierAttribute_ABC >
                          , public tools::ElementObserver_ABC< kernel::UndergroundAttribute_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoSummariesWidget( QWidget* parent, kernel::Controllers& controllers );
    virtual ~InfoSummariesWidget();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoSummariesWidget( const InfoSummariesWidget& );            //!< Copy constructor
    InfoSummariesWidget& operator=( const InfoSummariesWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* );
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    virtual void NotifyUpdated( const kernel::Attributes_ABC& extension );
    virtual void NotifyUpdated( const kernel::Positions& extension );

    virtual void NotifyUpdated( const kernel::ConstructionAttribute_ABC& extension )     { UpdateDisplayIfNeeded< kernel::ConstructionAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::MineAttribute_ABC& extension )             { UpdateDisplayIfNeeded< kernel::MineAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::ActivityTimeAttribute_ABC& extension )     { UpdateDisplayIfNeeded< kernel::ActivityTimeAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::NBCAttribute_ABC& extension )              { UpdateDisplayIfNeeded< kernel::NBCAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::BypassAttribute_ABC& extension )           { UpdateDisplayIfNeeded< kernel::BypassAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::ObstacleAttribute_ABC& extension )         { UpdateDisplayIfNeeded< kernel::ObstacleAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::LogisticAttribute_ABC& extension )         { UpdateDisplayIfNeeded< kernel::LogisticAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::CrossingSiteAttribute_ABC& extension )     { UpdateDisplayIfNeeded< kernel::CrossingSiteAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::SupplyRouteAttribute_ABC& extension )      { UpdateDisplayIfNeeded< kernel::SupplyRouteAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::ToxicCloudAttribute_ABC& extension )       { UpdateDisplayIfNeeded< kernel::ToxicCloudAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::FireAttribute_ABC& extension )             { UpdateDisplayIfNeeded< kernel::FireAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::BurnAttribute_ABC& extension )             { UpdateDisplayIfNeeded< kernel::BurnAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::BurnSurfaceAttribute_ABC& extension )      { UpdateDisplayIfNeeded< kernel::BurnSurfaceAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::MedicalTreatmentAttribute_ABC& extension ) { UpdateDisplayIfNeeded< kernel::MedicalTreatmentAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::OccupantAttribute_ABC& extension )         { UpdateDisplayIfNeeded< kernel::OccupantAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::DelayAttribute_ABC& extension )            { UpdateDisplayIfNeeded< kernel::DelayAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::FloodAttribute_ABC& extension )            { UpdateDisplayIfNeeded< kernel::FloodAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::StockAttribute_ABC& extension )            { UpdateDisplayIfNeeded< kernel::StockAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::AltitudeModifierAttribute_ABC& extension ) { UpdateDisplayIfNeeded< kernel::AltitudeModifierAttribute_ABC >( extension ); }
    virtual void NotifyUpdated( const kernel::UndergroundAttribute_ABC& extension )      { UpdateDisplayIfNeeded< kernel::UndergroundAttribute_ABC >( extension ); }

    template< class T >
    void UpdateDisplayIfNeeded( const T& extension );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    std::auto_ptr< kernel::Displayer_ABC > display_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: InfoSummariesWidget::UpdateDisplayIfNeeded
// Created: ABR 2011-01-21
// -----------------------------------------------------------------------------
template< class T >
void InfoSummariesWidget::UpdateDisplayIfNeeded( const T& extension )
{
    if( !selected_ || ( selected_->Retrieve< T >() != &extension ) )
        return;
    extension.DisplayInSummary( *display_ );
}

#endif // __InfoSummariesWidget_h_
