// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DictionaryUpdated_h_
#define __DictionaryUpdated_h_

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
// =============================================================================
/** @class  DictionaryUpdated
    @brief  Dictionary update message
*/
// Created: SBO 2006-11-13
// =============================================================================
class DictionaryUpdated
{
public:
    //! @name Constructors/Destructor
    //@{
             DictionaryUpdated( const kernel::Entity_ABC& entity, const QString& entry )
                 : entity_( entity ), entry_( entry ) {}
    virtual ~DictionaryUpdated() {}
    //@}

    //! @name Accessors
    //@{
    const kernel::Entity_ABC& GetEntity() const { return entity_; }
    const QString&      GetEntry () const { return entry_; }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DictionaryUpdated( const DictionaryUpdated& );            //!< Copy constructor
    DictionaryUpdated& operator=( const DictionaryUpdated& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& entity_;
    QString entry_;
    //@}
};

}

#endif // __DictionaryUpdated_h_
