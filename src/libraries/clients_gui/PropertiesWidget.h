// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PropertiesWidget_h_
#define __PropertiesWidget_h_

#include "clients_kernel/Displayer_ABC.h"

namespace gui
{
    class PropertiesTable;

// =============================================================================
/** @class  PropertiesWidget
    @brief  PropertiesWidget
*/
// Created: AGE 2006-10-18
// =============================================================================
class PropertiesWidget : public QWidget, public kernel::Displayer_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             PropertiesWidget( QWidget* parent, const QString& name );
    virtual ~PropertiesWidget();
    //@}

    //! @name Operations
    //@{
    void Clear();
    virtual void Hide();
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
    virtual QSizePolicy sizePolicy() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PropertiesWidget( PropertiesWidget* parent, const QString& name );
    PropertiesWidget( const PropertiesWidget& );            //!< Copy constructor
    PropertiesWidget& operator=( const PropertiesWidget& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< PropertiesWidget* >  T_SubWidgets;
    typedef T_SubWidgets::const_iterator    CIT_SubWidgets;

    typedef std::map< QString, unsigned int > T_SubCategories;
    typedef T_SubCategories::const_iterator CIT_SubCategories;
    //@}

    //! @name Helpers
    //@{
    void FillUp( const QString& name );
    kernel::Displayer_ABC& SubItem( const QString& name, const QString& path );
    PropertiesWidget* CreateWidget( const QString& subItem );

    virtual kernel::Displayer_ABC& SubItem( const QString& name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    //@}

private:
    //! @name Member data
    //@{
    QGridLayout* layout_;
    QToolButton* button_;
    T_SubCategories categories_;
    T_SubWidgets subWidgets_;
    PropertiesTable* table_;
    QSpacerItem* spacer_;
    //@}
};

}

#endif // __PropertiesWidget_h_
