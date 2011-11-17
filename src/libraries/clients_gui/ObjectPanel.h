// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Gui_ObjectPanel_h_
#define __Gui_ObjectPanel_h_

#include "InfoPanel_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Object_ABC;
    class Controllers;
    class MineAttribute_ABC;
    class ConstructionAttribute_ABC;
    class TimeLimitedAttribute_ABC;
    class ObstacleAttribute_ABC;
    class BypassAttribute_ABC;
    class NBCAttribute_ABC;
    class FireAttribute_ABC;
    class MedicalTreatmentAttribute_ABC;
    class LodgingAttribute_ABC;
    class LogisticAttribute_ABC;
    class CrossingSiteAttribute_ABC;
    class SupplyRouteAttribute_ABC;
    class StockAttribute_ABC;
}

namespace gui
{
    class DisplayBuilder;
    class ItemFactory_ABC;

// =============================================================================
/** @class  ObjectPanel
    @brief  ObjectPanel
*/
// Created: APE 2004-06-11
// =============================================================================
class ObjectPanel : public InfoPanel_ABC
                  , public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< kernel::Object_ABC >
                  , public tools::ElementObserver_ABC< kernel::ConstructionAttribute_ABC >
                  , public tools::ElementObserver_ABC< kernel::MineAttribute_ABC >
                  , public tools::ElementObserver_ABC< kernel::ObstacleAttribute_ABC >
                  , public tools::ElementObserver_ABC< kernel::TimeLimitedAttribute_ABC >
                  , public tools::ElementObserver_ABC< kernel::BypassAttribute_ABC >
                  , public tools::ElementObserver_ABC< kernel::LogisticAttribute_ABC >
                  , public tools::ElementObserver_ABC< kernel::LodgingAttribute_ABC >
                  , public tools::ElementObserver_ABC< kernel::SupplyRouteAttribute_ABC >
                  , public tools::ElementObserver_ABC< kernel::CrossingSiteAttribute_ABC >
                  , public tools::ElementObserver_ABC< kernel::NBCAttribute_ABC >
                  , public tools::ElementObserver_ABC< kernel::FireAttribute_ABC >
                  , public tools::ElementObserver_ABC< kernel::MedicalTreatmentAttribute_ABC >
                  , public tools::ElementObserver_ABC< kernel::StockAttribute_ABC >
                  , public tools::SelectionObserver< kernel::Object_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectPanel( QWidget* parent, PanelStack_ABC& panel, kernel::Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~ObjectPanel();
    //@}

    //! @name Operations
    //@{
    virtual void NotifySelected( const kernel::Object_ABC* object );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    ObjectPanel( const ObjectPanel& );
    ObjectPanel& operator=( const ObjectPanel& );
    //@}

protected:
    //! @name Helpers
    //@{
    const kernel::Object_ABC* GetSelected();
    DisplayBuilder& GetBuilder();
    void Update();
    void Clear();
    //@}

private:
    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );

    virtual void NotifyUpdated( const kernel::Object_ABC& );
    virtual void NotifyDeleted( const kernel::Object_ABC& );
    virtual void NotifyUpdated( const kernel::MineAttribute_ABC& attributes );
    virtual void NotifyUpdated( const kernel::ConstructionAttribute_ABC& attributes );
    virtual void NotifyUpdated( const kernel::BypassAttribute_ABC& attributes );
    virtual void NotifyUpdated( const kernel::ObstacleAttribute_ABC& attributes );
    virtual void NotifyUpdated( const kernel::LogisticAttribute_ABC& attributes );
    virtual void NotifyUpdated( const kernel::LodgingAttribute_ABC& attributes );
    virtual void NotifyUpdated( const kernel::CrossingSiteAttribute_ABC& attributes );
    virtual void NotifyUpdated( const kernel::SupplyRouteAttribute_ABC& attributes );
    virtual void NotifyUpdated( const kernel::NBCAttribute_ABC& attributes );
    virtual void NotifyUpdated( const kernel::FireAttribute_ABC& attributes );
    virtual void NotifyUpdated( const kernel::MedicalTreatmentAttribute_ABC& attributes );
    virtual void NotifyUpdated( const kernel::TimeLimitedAttribute_ABC& attributes );
    virtual void NotifyUpdated( const kernel::StockAttribute_ABC& attributes );

    template< typename Extension >
    bool ShouldUpdate( const Extension& extension );
    template< typename Extension >
    void DisplayIfNeeded( const Extension& extension );
    template< typename T >
    void UpdateExtension( const kernel::Object_ABC& object );
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QBoxLayout* pLayout_;
    //@}

private:
    //! @name Member data
    //@{
    kernel::SafePointer< kernel::Object_ABC > selected_;
    DisplayBuilder* display_;
    //@}
};

}

#endif // __Gui_ObjectPanel_h_
