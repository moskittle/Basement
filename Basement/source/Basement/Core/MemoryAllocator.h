#pragma once

#include "Basement/Core/Core.h"

 namespace Basement {

	 static u8 DefaultAlignment = 8;

	class Allocator
	{
	public:
		Allocator(size_t memSize, void* memStart) :
			m_MemorySize(memSize),
			m_MemoryStart(memStart),
			m_UsedMemory(0),
			m_AllocationCount(0) {}
		virtual ~Allocator() = 0;

		virtual void* Allocate(size_t size, u8 alignment = DefaultAlignment) = 0;
		virtual void Deallocate(void* ptr) = 0;
		virtual void Clear() = 0;

		size_t GetMemorySize() const { return m_MemorySize; }
		void* GetMemoryStart() const { return m_MemoryStart; }
		size_t GetUsedMemory() const { return m_UsedMemory; }
		u32 GetAllocationcCount() const { return m_AllocationCount; }

	 protected:
		size_t m_MemorySize;
		void* m_MemoryStart;
		size_t m_UsedMemory;
		u32 m_AllocationCount;
	 };


	namespace MemAlloc {

		// Calculate Header Size
		size_t CalcHeaderSize(size_t classSize)
		{
			size_t headerSize = (sizeof(size_t) / classSize);
			headerSize = (sizeof(size_t) % classSize > 0) ? headerSize + 1 : headerSize;
			return headerSize;
		}

		// Allocate
		template <class TClassType>
		TClassType* AllocateNew(Allocator& allocator)
		{
			return new (allocator.Allocate(sizeof(TClassType), alignof(TClassType))) TClassType;
		}

		// Allocate and Copy
		template <class TClassType>
		TClassType* AllocateNew(Allocator& allocator, const TClassType& other)
		{
			return new (allocator.Allocate(sizeof(TClassType), alignof(TClassType))) TClassType(other);
		}

		// Deallocate
		template <class TClassType>
		void DeallocateDelete(Allocator& allocator, TClassType& object)
		{
			object.~TClassType();
			allocator.Deallocate(&object);
		}

		// Allocate an Array
		template <class TClassType>
		TClassType* AllocateArray(Allocator& allocator, size_t length)
		{
			BM_ASSERT(length > 0);

			size_t headerSize = CalcHeaderSize(sizeof(TClassType));

			TClassType* ptr = (TClassType*) allocator.Allocate(sizeof(TClassType) * (length + headerSize), alignof(TClassType)) + headerSize;
			*( ((size_t*) ptr) - 1) = length;

			for (size_t i = 0; i < length; ++i)
			{
				new (&p[i]) TClassType;
			}

		}

		// Deallcate an Array
		template <class TClassType>
		void DeallocateArray(Allocator& allocator, TClassType* objectArray)
		{
			BM_ASSERT(allocator != nullptr);

			size_t length = *( ((size_t*)objectArray) - 1);
			for (size_t i = 0; i < length; ++i)
			{
				objectArray[i].~TClassType();
			}

			size_t headerSize = CalcHeaderSize(sizeof(TClassType));

			allocator.Deallocate(objectArray - headerSize);
		}

	}	// namespace Basement::MemAlloc

 }