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

namespace gui
{
    class ObjectPrototypeLoader_ABC;
    template< typename T > class RichWidget;

// =============================================================================
/** @class  LoadableField
    @brief  Base class for fields that can be filled from a file or manually.
*/
// Created: BCI 2011-05-09
// =============================================================================
class LoadableField : public Q3HBox
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
    void SetLoader( const ObjectPrototypeLoader_ABC* );
    QString GetField() const;
    //@}

private slots:
    void FieldSelected( const QString& fieldName );

private:
    //! @name Helpers
    //@{
    void SetPossibleFields( const QStringList& fields );
    void SelectProbableField();
    //@}

protected:
    const ObjectPrototypeLoader_ABC* currentLoader_;
    QWidget* defaultValueWidget_;

private:
    //! @name Member data
    //@{
    const QString probableName_;
    RichWidget< QComboBox >* fieldName_;
    //@}
};

template< typename T >
class LoadableFieldTemplate :  public LoadableField
{
public:
    LoadableFieldTemplate( QWidget* parent, const QString& objectName )
        : LoadableField( parent, objectName )
    {
        //NOTHING
    }

    void SetDefaultValueWidget( T* w )
    {
        defaultValueWidget_ = w;
    }

    T* GetDefaultValueWidget() const
    {
        return static_cast< T* >( defaultValueWidget_ );
    }
};

} // end namespace gui

#endif // __LoadableField_h_
