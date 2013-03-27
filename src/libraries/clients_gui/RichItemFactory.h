// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RichItemFactory_h_
#define __RichItemFactory_h_

#include "ItemFactory_ABC.h"

namespace gui
{

// =============================================================================
/** @class  RichItemFactory
    @brief  RichItemFactory
*/
// Created: AGE 2006-05-10
// =============================================================================
class RichItemFactory : public QObject, public ItemFactory_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichItemFactory( QObject* parent = 0 );
    virtual ~RichItemFactory();
    //@}

    //! @name Operations
    //@{
    virtual RichLabel*      CreateLabel( const QString& objectName, QWidget* parent = 0 );
    virtual RichLabel*      CreateLabel( const QString& objectName, const QString& text, QWidget* parent = 0 );
    virtual RichLabel*      CreateLabel( const QString& objectName, const QString& text, bool required, QWidget* parent = 0 );
    //@}

signals:
    //! @name Signals
    //@{
    void LinkClicked( const QString& anchor );
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void DealWithLink( const QString& anchor );
    //@}

private:
    //! @name Helpers
    //@{
    RichLabel*      Connect( RichLabel* item );
    //@}

    //! @name Types
    //@{
    typedef std::set< QObject* > T_Connections;
    //@}

private:
    //! @name Member data
    //@{
    T_Connections connections_;
    //@}
};

}

#endif // __RichItemFactory_h_
