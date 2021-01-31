//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/common/types/vector_buffer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/common.hpp"
#include "duckdb/common/types/string_heap.hpp"
#include "duckdb/common/types/string_type.hpp"
#include "duckdb/common/types/selection_vector.hpp"

namespace duckdb {

class BufferHandle;
class VectorBuffer;
class Vector;
class ChunkCollection;

enum class VectorBufferType : uint8_t {
	STANDARD_BUFFER,     // standard buffer, holds a single array of data
	DICTIONARY_BUFFER,   // dictionary buffer, holds a selection vector
	VECTOR_CHILD_BUFFER, // vector child buffer: holds another vector
	STRING_BUFFER,       // string buffer, holds a string heap
	STRUCT_BUFFER,       // struct buffer, holds a ordered mapping from name to child vector
	LIST_BUFFER,         // list buffer, holds a single flatvector child
	MANAGED_BUFFER       // managed buffer, holds a buffer managed by the buffermanager
};

//! The VectorBuffer is a class used by the vector to hold its data
class VectorBuffer {
public:
	VectorBuffer(VectorBufferType type) : vectorBufferType(type) {
	}
	VectorBuffer(idx_t data_size) : vectorBufferType(VectorBufferType::STANDARD_BUFFER) {
        if (data_size > 0) {
            data = unique_ptr<data_t[]>(new data_t[data_size]);
        }
    }
	virtual ~VectorBuffer() {
	}
	VectorBuffer(){
	}
	VectorBufferType vectorBufferType;

    VectorBuffer(VectorType vectorType, const LogicalType &type, idx_t data_size)
    : vector_type(vectorType), type(type), vectorBufferType(VectorBufferType::STANDARD_BUFFER)  {
        if (data_size > 0) {
            data = unique_ptr<data_t[]>(new data_t[data_size]);
        }
    }
    VectorBuffer(VectorType vectorType, const LogicalType &type)
        : vector_type(vectorType), type(type)  {
    }

    VectorBuffer(VectorType vectorType, idx_t data_size)
    : vector_type(vectorType) ,  vectorBufferType(VectorBufferType::STANDARD_BUFFER) {
        if (data_size > 0) {
            data = unique_ptr<data_t[]>(new data_t[data_size]);
        }
    }

    VectorBuffer(VectorType vectorType)
        : vector_type(vectorType) {
    }


public:
	data_ptr_t GetData() {
		return data.get();
	}

	static buffer_ptr<VectorBuffer> CreateStandardVector(PhysicalType type);
	static buffer_ptr<VectorBuffer> CreateConstantVector(PhysicalType type);
    static buffer_ptr<VectorBuffer> CreateConstantVector(VectorType vectorType, LogicalType logicalType, PhysicalType type);
    static buffer_ptr<VectorBuffer> CreateStandardVector(VectorType vectorType, LogicalType logicalType, PhysicalType type);
    static buffer_ptr<VectorBuffer> CreateStandardVector(VectorType vectorType, PhysicalType type);

public:
	unique_ptr<data_t[]> data;
    //! The vector type specifies how the data of the vector is physically stored (i.e. if it is a single repeated
    //! constant, if it is compressed)
    VectorType vector_type;
    //! The type of the elements stored in the vector (e.g. integer, float)
    LogicalType type;
};


//! The DictionaryBuffer holds a selection vector
class DictionaryBuffer : public VectorBuffer {
public:
	DictionaryBuffer(const SelectionVector &sel) : VectorBuffer(VectorBufferType::DICTIONARY_BUFFER), sel_vector(sel) {
	}
	DictionaryBuffer(buffer_ptr<SelectionData> data)
	    : VectorBuffer(VectorBufferType::DICTIONARY_BUFFER), sel_vector(move(data)) {
	}
	DictionaryBuffer(idx_t count = STANDARD_VECTOR_SIZE)
	    : VectorBuffer(VectorBufferType::DICTIONARY_BUFFER), sel_vector(count) {
	}

public:
	SelectionVector &GetSelVector() {
		return sel_vector;
	}

private:
	SelectionVector sel_vector;
};

class VectorStringBuffer : public VectorBuffer {
public:
	VectorStringBuffer();

public:
	string_t AddString(const char *data, idx_t len) {
		return heap.AddString(data, len);
	}
	string_t AddString(string_t data) {
		return heap.AddString(data);
	}
	string_t AddBlob(string_t data) {
		return heap.AddBlob(data.GetDataUnsafe(), data.GetSize());
	}
	string_t EmptyString(idx_t len) {
		return heap.EmptyString(len);
	}

	void AddHeapReference(buffer_ptr<VectorBuffer> heap) {
		references.push_back(move(heap));
	}

private:
	//! The string heap of this buffer
	StringHeap heap;
	// References to additional vector buffers referenced by this string buffer
	vector<buffer_ptr<VectorBuffer>> references;
};

class VectorStructBuffer : public VectorBuffer {
public:
	VectorStructBuffer();
	~VectorStructBuffer();

public:
	child_list_t<unique_ptr<Vector>> &GetChildren() {
		return children;
	}
	void AddChild(string name, unique_ptr<Vector> vector) {
		children.push_back(std::make_pair(name, move(vector)));
	}

private:
	//! child vectors used for nested data
	child_list_t<unique_ptr<Vector>> children;
};

class VectorListBuffer : public VectorBuffer {
public:
	VectorListBuffer();

	~VectorListBuffer();

public:
	ChunkCollection &GetChild() {
		return *child;
	}
	void SetChild(unique_ptr<ChunkCollection> new_child);

private:
	//! child vectors used for nested data
	unique_ptr<ChunkCollection> child;
};

//! The ManagedVectorBuffer holds a buffer handle
class ManagedVectorBuffer : public VectorBuffer {
public:
	ManagedVectorBuffer(unique_ptr<BufferHandle> handle);
	~ManagedVectorBuffer();

private:
	unique_ptr<BufferHandle> handle;
};

} // namespace duckdb
