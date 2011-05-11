// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LoadableField_h_
#define __LoadableField_h_

#ifdef __GNUG__
#   pragma interface
#endif

namespace gui
{
    class ObjectPrototypeLoader_ABC;

// =============================================================================
/** @class  LoadableField
    @brief  Base class for fields that can be filled from a file or manually.
*/
// Created: BCI 2011-05-09
// =============================================================================
class LoadableField : public QHBox
{
    Q_OBJECT
public:

    //! @name Constructors/Destructor
    //@{
             LoadableField( QWidget* parent, const QString& probableName );
    virtual ~LoadableField();
    //@}

    //! @name Operations
    //@{
    void SetDefaultValueWidget( QWidget* );
    void SetLoader( const ObjectPrototypeLoader_ABC* );
    QString GetField() const;
    //@}

private slots:
    void FieldSelected( const QString& fieldName );

private:
    //! @name Copy/Assignment
    //@{
    LoadableField( const LoadableField& );            //!< Copy constructor
    LoadableField& operator=( const LoadableField& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SetPossibleFields( const QStringList& fields );
    void SelectProbableField();
    //@}

protected:
    //! @name Member data
    //@{
    const QString probableName_;
    QWidget* defaultValueWidget_;
    QComboBox* fieldName_;
    const ObjectPrototypeLoader_ABC* currentLoader_;
    //@}
};

template< typename T >
class LoadableFieldTemplate :  public LoadableField
{
public:
    LoadableFieldTemplate( QWidget* parent, const QString& probableName )
        : LoadableField( parent, probableName )
    {
        //NOTHING
    }

    T* GetDefaultValueWidget() const
    {
        return static_cast< T* >( defaultValueWidget_ );
    }
};

} // end namespace gui

#endif // __LoadableField_h_
