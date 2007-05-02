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

class ParamVisitor_ABC;

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
             ListParameter( QObject* parent, const QString& name, kernel::ActionController& controller, bool optional );
    virtual ~ListParameter();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity();
    virtual void BuildInterface( QWidget* parent );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

protected:
    //! @name Operations
    //@{
    unsigned int Count() const;
    void Accept( ParamVisitor_ABC& visitor ) const;
    void Select( const Param_ABC& param );
    virtual void DeleteElement( Param_ABC& param );
    void EnableCreation( bool enabled );
    //@}

protected slots:
    //! @name slots
    //@{
    void OnCreate();
    void OnDeleteSelectedItem();
    void OnClear();
    void OnRequestPopup( QListViewItem* item, const QPoint& pos );
    void OnSelectionChanged( QListViewItem* item );
    void TurnHeaderBlack();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ListParameter( const ListParameter& );            //!< Copy constructor
    ListParameter& operator=( const ListParameter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual Param_ABC* CreateElement() = 0;
    void DeleteItem( QListViewItem* item );
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
    QListView* list_;
    QListViewItem* selected_;
    bool optional_;
    T_Widgets widgets_;
    bool createEnabled_;
    //@}
};

#endif // __ListParameter_h_
