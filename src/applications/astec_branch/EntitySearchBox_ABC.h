// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntitySearchBox_ABC_h_
#define __EntitySearchBox_ABC_h_

// =============================================================================
/** @class  EntitySearchBox_ABC
    @brief  EntitySearchBox_ABC
*/
// Created: SBO 2006-04-20
// =============================================================================
class EntitySearchBox_ABC : public QHBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit EntitySearchBox_ABC( QWidget* parent );
    virtual ~EntitySearchBox_ABC();
    //@}

    //! @name Types
    //@{
    class SearchableItem_ABC
    {
    public:
        virtual ~SearchableItem_ABC() {};
        virtual bool Matches( const QString& input ) const = 0;
        virtual void Activate() = 0;
    };
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void Search( const QString& input );
    virtual void FindNext();
    //@}

protected:
    //! @name Types
    //@{
    typedef std::vector< SearchableItem_ABC* >    T_Items;
    typedef T_Items::iterator                    IT_Items;
    typedef T_Items::const_iterator             CIT_Items;
    //@}

    //! @name Helpers
    //@{
    void AddItem( SearchableItem_ABC& item );
    void RemoveItem( unsigned long id );
    
    void Find();
    bool Find( CIT_Items begin, CIT_Items end );
    //@}

private:
    //! @name Member data
    //@{
    T_Items items_;
    QString currentSearch_;
    CIT_Items lastItem_;
    //@}

};

#endif // __EntitySearchBox_ABC_h_
