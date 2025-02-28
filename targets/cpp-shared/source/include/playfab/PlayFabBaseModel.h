#ifndef PLAYFAB_BASE_MODEL_H_
#define PLAYFAB_BASE_MODEL_H_

#include <string>
#include <ctime>
#include <list>
#include <map>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/encodings.h"
#include "rapidjson/stringbuffer.h"


namespace PlayFab
{

	typedef rapidjson::Writer< rapidjson::GenericStringBuffer< rapidjson::UTF8<> > > PFStringJsonWriter;

	typedef unsigned short Uint16;
	typedef short Int16;
	typedef unsigned long Uint32;
	typedef long Int32;
	typedef unsigned long long Uint64;
	typedef long long Int64;

    template <typename BoxedType>
    class Boxed
    {
    public:
        BoxedType mValue;

        Boxed() : mValue(), mIsSet(false) {}
        Boxed(BoxedType value) : mValue(value), mIsSet(true) {}

        Boxed& operator=(BoxedType value) { mValue = value; mIsSet = true; return *this; }
        operator BoxedType() { return mValue; }

        void setNull() { mIsSet = false; }
        bool notNull() { return mIsSet; }
        bool isNull() { return !mIsSet; }
    private:
        bool mIsSet;
    };

    typedef Boxed<bool> OptionalBool;
    typedef Boxed<Uint16> OptionalUint16;
    typedef Boxed<Int16> OptionalInt16;
    typedef Boxed<Uint32> OptionalUint32;
    typedef Boxed<Int32> OptionalInt32;
    typedef Boxed<Uint64> OptionalUint64;
    typedef Boxed<Int64> OptionalInt64;
    typedef Boxed<float> OptionalFloat;
    typedef Boxed<double> OptionalDouble;
    typedef Boxed<time_t> OptionalTime;

    struct PlayFabBaseModel
    {
        virtual ~PlayFabBaseModel() {}
        virtual void writeJSON(PFStringJsonWriter& writer) = 0;
        virtual bool readFromValue(const rapidjson::Value& obj) = 0;

        std::string toJSONString();
    };

    enum MultitypeVarTypes
    {
        MultitypeNull,
        MultitypeBool,
        MultitypeNumber,
        MultitypeString
    };

    struct MultitypeVar : public PlayFabBaseModel
    {
    private:
        MultitypeVarTypes mType;
        std::string mString;
        bool mBool;
        double mNumber;

    public:

        MultitypeVar() : mType(MultitypeNull) {}
        MultitypeVar(bool val) : mType(MultitypeBool), mBool(val) {}
        MultitypeVar(Uint16 val) : mType(MultitypeNumber), mNumber(val) {}
        MultitypeVar(Int16 val) : mType(MultitypeNumber), mNumber(val) {}
        MultitypeVar(Uint32 val) : mType(MultitypeNumber), mNumber(val) {}
        MultitypeVar(Int32 val) : mType(MultitypeNumber), mNumber(val) {}
        MultitypeVar(float val) : mType(MultitypeNumber), mNumber(val) {}
        MultitypeVar(double val) : mType(MultitypeNumber), mNumber(val) {}
        MultitypeVar(std::string val) : mType(MultitypeString), mString(val) {}

        MultitypeVar(const rapidjson::Value& obj)
        {
            readFromValue(obj);
        }

        void setNull() { mType = MultitypeNull; }
        bool notNull() { return mType != MultitypeNull; }
        bool isNull() { return mType == MultitypeNull; }
        MultitypeVarTypes getType() { return mType; }

        MultitypeVar& operator=(bool val) { mBool = val; mType = MultitypeBool; return *this; }
        MultitypeVar& operator=(Uint16 val) { mNumber = val; mType = MultitypeNumber; return *this; }
        MultitypeVar& operator=(Int16 val) { mNumber = val; mType = MultitypeNumber; return *this; }
        MultitypeVar& operator=(Uint32 val) { mNumber = val; mType = MultitypeNumber; return *this; }
        MultitypeVar& operator=(Int32 val) { mNumber = val; mType = MultitypeNumber; return *this; }
        MultitypeVar& operator=(float val) { mNumber = val; mType = MultitypeNumber; return *this; }
        MultitypeVar& operator=(double val) { mNumber = val; mType = MultitypeNumber; return *this; }
        MultitypeVar& operator=(std::string val) { mString = val; mType = MultitypeString; return *this; }

        operator bool() { return mBool; }
        operator Uint16() { return (Uint16)mNumber; }
        operator Int16() { return (Int16)mNumber; }
        operator Uint32() { return (Uint32)mNumber; }
        operator Int32() { return (Int32)mNumber; }
        operator float() { return (float)mNumber; }
        operator double() { return mNumber; }
        operator std::string() { return mString; }

        ~MultitypeVar() {}
        void writeJSON(PFStringJsonWriter& writer);
        bool readFromValue(const rapidjson::Value& obj);
    };

	
	void writeDatetime(time_t datetime, PFStringJsonWriter& writer);
    time_t readDatetime(const rapidjson::Value& obj);
}

#endif