// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListParameter_h_
#define __ListParameter_h_

#include "Param_ABC.h"

namespace kernel
{
    class OrderParameter;
}

namespace actions
{
    class Parameter_ABC;

    namespace gui
    {

// =============================================================================
/** @class  ListParameter
    @brief  ListParameter
*/
// Created: SBO 2007-04-26
// =============================================================================
class ListParameter : public QObject
                    , public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ListParameter( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& controller );
    virtual ~ListParameter();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity();
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

protected:
    //! @name Operations
    //@{
    unsigned int Count() const;
    void CommitChildrenTo( actions::ParameterContainer_ABC& parent ) const;
    void Select( const Param_ABC& param );
    virtual void DeleteElement( Param_ABC& param );
    void EnableCreation( bool enabled );
    void SetLabel( const QString& label );
    //@}

protected slots:
    //! @name slots
    //@{
    void OnCreate();
    void OnDeleteSelectedItem();
    void OnClear();
    void OnRequestPopup( Q3ListViewItem* item, const QPoint& pos );
    void OnSelectionChanged( Q3ListViewItem* item );
    void TurnHeaderBlack();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool IsOptional() const;
    virtual Param_ABC* CreateElement() = 0;
    void DeleteItem( Q3ListViewItem* item );
    void Clear();
    bool Invalid();
    //@}

    //! @name Types
    //@{
    typedef std::map< const Param_ABC*, QWidget* > T_Widgets;
    //@}

private:
    //! @name Member data
    //@{
    kernel::ActionController& controller_;
    Q3ListView* list_;
    Q3ListViewItem* selected_;
    T_Widgets widgets_;
    unsigned int min_;
    unsigned int max_;
    bool optional_;
    bool createEnabled_;
    //@}
};

    }
}

#endif // __ListParameter_h_
