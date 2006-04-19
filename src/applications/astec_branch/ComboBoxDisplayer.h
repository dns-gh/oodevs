// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ComboBoxDisplayer_h_
#define __ComboBoxDisplayer_h_

#include "Displayer_ABC.h"
#include "ValuedComboBox.h"
#include "Iterator.h"

// =============================================================================
/** @class  ComboBoxDisplayer
    @brief  ComboBox Displayer
*/
// Created: SBO 2006-04-18
// =============================================================================
template< typename ConcreteList, typename Element >
class ComboBoxDisplayer : public Displayer_ABC
                        , public ValuedComboBox< const Element* >
{
public:
    //! @name Constructors/Destructor
    //@{
             ComboBoxDisplayer( QWidget* parent, const char* name, const ConcreteList& list );
    virtual ~ComboBoxDisplayer() {};
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ComboBoxDisplayer( const ComboBoxDisplayer& );            //!< Copy constructor
    ComboBoxDisplayer& operator=( const ComboBoxDisplayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Hide() {}
    virtual Displayer_ABC& SubItem( const char* name ) { throw std::runtime_error( __FUNCTION__ ); }
    virtual void StartDisplay() {}
    virtual void DisplayFormatted( const QString& formatted ) {}
    virtual void EndDisplay() {}
    //@}
};

// -----------------------------------------------------------------------------
// Name: ComboBoxDisplayer constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
template< typename ConcreteList, typename Element >
ComboBoxDisplayer<ConcreteList, Element>::ComboBoxDisplayer( QWidget* parent, const char* name, const ConcreteList& list )
    : ValuedComboBox< const Element* >( parent, name )
{
    Iterator< const Element& > it( list.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const Element& element = it.NextElement();
        AddItem( element.GetName().c_str(), &element );
    }
}

#endif // __ComboBoxDisplayer_h_
