/*
 * Copyright 2026 Pie Laboratories
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CORE_STL_DOT_HPP
#define CORE_STL_DOT_HPP

#include <cstdint>
#include <list>
#include <vector>
#include <map>
#include <memory>

#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif

namespace Core
{
    typedef std::list<bool> BoolList;
    typedef BoolList::iterator BoolListIndex;
    typedef BoolList::const_iterator ConstBoolListIndex;
    typedef BoolList::reverse_iterator ReverseBoolListIndex;
    typedef BoolList::const_reverse_iterator ConstReverseBoolListIndex;

    typedef std::vector<bool> BoolVector;
    typedef BoolVector::iterator BoolVectorIndex;
    typedef BoolVector::const_iterator ConstBoolVectorIndex;
    typedef BoolVector::reverse_iterator ReverseBoolVectorIndex;
    typedef BoolVector::const_reverse_iterator ConstReverseBoolVectorIndex;

    typedef std::list<int8_t> Int8List;
    typedef Int8List::iterator Int8ListIndex;
    typedef Int8List::const_iterator ConstInt8ListIndex;
    typedef Int8List::reverse_iterator ReverseInt8ListIndex;
    typedef Int8List::const_reverse_iterator ConstReverseInt8ListIndex;

    typedef std::vector<int8_t> Int8Vector;
    typedef Int8Vector::iterator Int8VectorIndex;
    typedef Int8Vector::const_iterator ConstInt8VectorIndex;
    typedef Int8Vector::reverse_iterator ReverseInt8VectorIndex;
    typedef Int8Vector::const_reverse_iterator ConstReverseInt8VectorIndex;

    typedef std::list<uint8_t> UInt8List;
    typedef UInt8List::iterator UInt8ListIndex;
    typedef UInt8List::const_iterator ConstUInt8ListIndex;
    typedef UInt8List::reverse_iterator ReverseUInt8ListIndex;
    typedef UInt8List::const_reverse_iterator ConstReverseUInt8ListIndex;

    typedef std::vector<uint8_t> UInt8Vector;
    typedef UInt8Vector::iterator UInt8VectorIndex;
    typedef UInt8Vector::const_iterator ConstUInt8VectorIndex;
    typedef UInt8Vector::reverse_iterator ReverseUInt8VectorIndex;
    typedef UInt8Vector::const_reverse_iterator ConstReverseUInt8VectorIndex;

    typedef std::list<uint16_t> Int16List;
    typedef Int16List::iterator Int16ListIndex;
    typedef Int16List::const_iterator ConstInt16ListIndex;
    typedef Int16List::reverse_iterator ReverseInt16ListIndex;
    typedef Int16List::const_reverse_iterator ConstReverseInt16ListIndex;

    typedef std::vector<uint16_t> Int16Vector;
    typedef Int16Vector::iterator Int16VectorIndex;
    typedef Int16Vector::const_iterator ConstInt16VectorIndex;
    typedef Int16Vector::reverse_iterator ReverseInt16VectorIndex;
    typedef Int16Vector::const_reverse_iterator ConstReverseInt16VectorIndex;

    typedef std::list<uint16_t> UInt16List;
    typedef UInt16List::iterator UInt16ListIndex;
    typedef UInt16List::const_iterator ConstUInt16ListIndex;
    typedef UInt16List::reverse_iterator ReverseUInt16ListIndex;
    typedef UInt16List::const_reverse_iterator ConstReverseUInt16ListIndex;

    typedef std::vector<uint16_t> UInt16Vector;
    typedef UInt16Vector::iterator UInt16VectorIndex;
    typedef UInt16Vector::const_iterator ConstUInt16VectorIndex;
    typedef UInt16Vector::reverse_iterator ReverseUInt16VectorIndex;
    typedef UInt16Vector::const_reverse_iterator ConstReverseUInt16VectorIndex;

    typedef std::list<int32_t> Int32List;
    typedef Int32List::iterator Int32ListIndex;
    typedef Int32List::const_iterator ConstInt32ListIndex;
    typedef Int32List::reverse_iterator ReverseInt32ListIndex;
    typedef Int32List::const_reverse_iterator ConstReverseInt32ListIndex;

    typedef std::vector<int32_t> Int32Vector;
    typedef Int32Vector::iterator Int32VectorIndex;
    typedef Int32Vector::const_iterator ConstInt32VectorIndex;
    typedef Int32Vector::reverse_iterator ReverseInt32VectorIndex;
    typedef Int32Vector::const_reverse_iterator ConstReverseInt32VectorIndex;

    typedef std::list<uint32_t> UInt32List;
    typedef UInt32List::iterator UInt32ListIndex;
    typedef UInt32List::const_iterator ConstUInt32ListIndex;
    typedef UInt32List::reverse_iterator ReverseUInt32ListIndex;
    typedef UInt32List::const_reverse_iterator ConstReverseUInt32ListIndex;

    typedef std::vector<uint32_t> UInt32Vector;
    typedef UInt32Vector::iterator UInt32VectorIndex;
    typedef UInt32Vector::const_iterator ConstUInt32VectorIndex;
    typedef UInt32Vector::reverse_iterator ReverseUInt32VectorIndex;
    typedef UInt32Vector::const_reverse_iterator ConstReverseUInt32VectorIndex;

    typedef std::list<int64_t> Int64List;
    typedef Int64List::iterator Int64ListIndex;
    typedef Int64List::const_iterator ConstInt64ListIndex;
    typedef Int64List::reverse_iterator ReverseInt64ListIndex;
    typedef Int64List::const_reverse_iterator ConstReverseInt64ListIndex;

    typedef std::vector<int64_t> Int64Vector;
    typedef Int64Vector::iterator Int64VectorIndex;
    typedef Int64Vector::const_iterator ConstInt64VectorIndex;
    typedef Int64Vector::reverse_iterator ReverseInt64VectorIndex;
    typedef Int64Vector::const_reverse_iterator ConstReverseInt64VectorIndex;

    typedef std::list<uint64_t> UInt64List;
    typedef UInt64List::iterator UInt64ListIndex;
    typedef UInt64List::const_iterator ConstUInt64ListIndex;
    typedef UInt64List::reverse_iterator ReverseUInt64ListIndex;
    typedef UInt64List::const_reverse_iterator ConstReverseUInt64ListIndex;

    typedef std::vector<uint64_t> UInt64Vector;
    typedef UInt64Vector::iterator UInt64VectorIndex;
    typedef UInt64Vector::const_iterator ConstUInt64VectorIndex;
    typedef UInt64Vector::reverse_iterator ReverseUInt64VectorIndex;
    typedef UInt64Vector::const_reverse_iterator ConstReverseUInt64VectorIndex;

    typedef std::list<float> FloatList;
    typedef FloatList::iterator FloatListIndex;
    typedef FloatList::const_iterator ConstFloatListIndex;
    typedef FloatList::reverse_iterator ReverseFloatListIndex;
    typedef FloatList::const_reverse_iterator ConstReverseFloatListIndex;

    typedef std::vector<float> FloatVector;
    typedef FloatVector::iterator FloatVectorIndex;
    typedef FloatVector::const_iterator ConstFloatVectorIndex;
    typedef FloatVector::reverse_iterator ReverseFloatVectorIndex;
    typedef FloatVector::const_reverse_iterator ConstReverseFloatVectorIndex;

    typedef std::list<double> DoubleList;
    typedef DoubleList::iterator DoubleListIndex;
    typedef DoubleList::const_iterator ConstDoubleListIndex;
    typedef DoubleList::reverse_iterator ReverseDoubleListIndex;
    typedef DoubleList::const_reverse_iterator ConstReverseDoubleListIndex;

    typedef std::vector<double> DoubleVector;
    typedef DoubleVector::iterator DoubleVectorIndex;
    typedef DoubleVector::const_iterator ConstDoubleVectorIndex;
    typedef DoubleVector::reverse_iterator ReverseDoubleVectorIndex;
    typedef DoubleVector::const_reverse_iterator ConstReverseDoubleVectorIndex;

    typedef std::list<Core::String> StringList;
    typedef StringList::iterator StringListIndex;
    typedef StringList::const_iterator ConstStringListIndex;
    typedef StringList::reverse_iterator ReverseStringListIndex;
    typedef StringList::const_reverse_iterator ConstReverseStringListIndex;

    typedef std::vector<Core::String> StringVector;
    typedef StringVector::iterator StringVectorIndex;
    typedef StringVector::const_iterator ConstStringVectorIndex;
    typedef StringVector::reverse_iterator ReverseStringVectorIndex;
    typedef StringVector::const_reverse_iterator ConstReverseStringVectorIndex;

    typedef std::map<Core::String,Core::String> StringToStringMap;
    typedef StringToStringMap::iterator StringToStringMapIndex;
    typedef StringToStringMap::const_iterator ConstStringToStringMapIndex;
    typedef StringToStringMap::reverse_iterator ReverseStringToStringMapIndex;
    typedef StringToStringMap::const_reverse_iterator ConstReverseStringToStringMapIndex;

    template<typename T>
    concept Cloneable = requires(const T &t) {
        { t.clone() } -> std::convertible_to<std::unique_ptr<T>>;
    };
}

#endif  // CORE_STL_DOT_HPP
