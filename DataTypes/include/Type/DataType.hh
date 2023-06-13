#pragma once

#include <string>
#include <vector>

#include "Value/Value.hh"
#include "Algorithm/DataTypeVisitor.hh"
#include "Utils/VariableNameStack.hh"

namespace TypeClass {
    enum e {
        PRIMITIVE   = 0,
        ENUMERATION = 1,
        POINTER     = 2,
        ARRAY       = 3,
        COMPOSITE   = 4
    };
};

/** @brief Abstract base class for DataTypes. */
class DataType {

    public:

    virtual ~DataType() {}

    /**
    Verify that all of the types that are named by this DataType or
    subordinate DataTypes are resolvable to actual DataTypes in
    the TypeDictionary.
    */
    virtual bool validate() = 0;

    /**
    @return The size (in bytes) of an instance of the DataType.
    */
    virtual size_t getSize() const = 0;

    /**
    @return An enumeration value indicating which of the several specific subclasses of DataType this class is.
    */
    virtual TypeClass::e getTypeClass() const = 0;

    /**
     */
    virtual std::string getTypeSpecName() const = 0;

    /**
    @return The number of arrayed Type-specifier elements of the DataType.
    FIXME: THIS LOOKS HINKY and IS ARRAY SPECIFIC
    */
    // virtual unsigned int getTotalElementCount() const { return 1; }

    /**
    @return does the DataType or any member of the DataType represent a pointer?
    */
    virtual bool containsPointers() const { return false; }

    /**
    @return a duplicate DataType.
    */
    virtual DataType * clone () const = 0;

    /**
    */
    virtual void* createInstance(unsigned int n = 1 ) const = 0;

    /**
    */
    virtual void deleteInstance(void* address) const = 0;

    /**
    Clear the variable at the given address.
    @param address Address of the variable.
    */
    virtual void clearValue(void * address) const = 0;

    /**
    Assign a value to the variable at the given address.
    @param address Address of the variable.
    @param value Value to be assigned to the element.
    */
    virtual void assignValue(void * address, Value * value) const = 0;

    /**
    Print the value of the variable at the given address to the given stream.
    @param s The stream to print to.
    @param address Address of the variable.
    */
    virtual void printValue(std::ostream &s, void *address) const = 0;

    /**
    Creates a Value object for the variable at the given address.
    @param address Address of the variable.
    */
    virtual Value * getValue(void *address) const;


    // virtual bool lookupVariableNameByOffset(VariableNameStack& nameStack, unsigned int offset, const DataType * expectedType) const;


    /**
    Checkpoint the variable at the given address to the given stream.
    @param s The stream to print to.
    @param address Address of the variable.
    */
    // virtual void checkpointValue(std::ostream &s, std::string var_name, void *address) const;

    // /**
    // Print the value of the variable at the given address to the given stream.
    // @param s The stream to print to.
    // @param address Address of the variable.
    // */
    // virtual void restoreValue(std::ostream &s, std::string var_name, void *address) const = 0;

    /**
    Generate a declaration of this DataType.
    @param declarator A declarator, which can be a
        1) a variable name or an array declarator (both are direct-declarators) or
        2) a pointer declarator ( not a direct-declarator ).
    */
    virtual std::string makeDeclaration(std::string declarator) const;

    /**
    Generate a definition of this DataType.
    */
    virtual std::string toString() const;

    /**
    @return true it this DataType represents void or false otherwise.
    */
    virtual bool isVoid() const { return false; }

    /* 
    Implement a Visitor Pattern
    */
    virtual bool accept (DataTypeVisitor* visitor) const;


    private:
};