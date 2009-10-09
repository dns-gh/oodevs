// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPreviewIcon_h_
#define __ObjectPreviewIcon_h_

#include "tools/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{    
    class ObjectType; 
}

namespace gui
{
    class SymbolIcons;
    class ColorStrategy_ABC;

// =============================================================================
/** @class  ObjectPreviewIcon
    @brief  ObjectPreviewIcon
*/
// Created: SBO 2007-10-16
// =============================================================================
class ObjectPreviewIcon : public QHBox    
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectPreviewIcon( QWidget* parent, kernel::Controllers& controllers, SymbolIcons& icons );
    virtual ~ObjectPreviewIcon();
    //@}

    //! @name Operations
    //@{
    void NotifySelected( const kernel::ObjectType& type );    
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectPreviewIcon( const ObjectPreviewIcon& );            //!< Copy constructor
    ObjectPreviewIcon& operator=( const ObjectPreviewIcon& ); //!< Assignment operator
    //@}

    //! @name Slots
    //@{
    void UpdateSymbol();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    SymbolIcons& icons_;    
    QLabel* icon_;
    std::string symbol_;
    //@}
};

}

#endif // __ObjectPreviewIcon_h_
