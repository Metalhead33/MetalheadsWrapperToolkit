#ifndef STREAMABLE_HPP
#define STREAMABLE_HPP
#include <memory>
namespace Abstract {
namespace GFX {

// Forward declaring structs
struct SharedMappedMemory;
struct ReferencedMappedMemory;
// The class
class Streamable {
public:
	virtual ~Streamable() = default;
	virtual void* map() = 0;
	virtual void unmap() = 0;
	struct SharedMappedMemory {
		std::shared_ptr<Streamable> parent;
		void* ptr;
		~SharedMappedMemory()
		{
			if(ptr) parent->unmap();
		}
		SharedMappedMemory(const std::shared_ptr<Streamable>& nparent)
			: parent(nparent), ptr(nullptr)
		{
			ptr = parent->map();
		}
		SharedMappedMemory(std::shared_ptr<Streamable>&& nparent)
			: parent(std::move(nparent)), ptr(nullptr)
		{
			ptr = parent->map();
		}
		SharedMappedMemory(SharedMappedMemory&& mov)
			: parent(std::move(mov.parent)), ptr(mov.ptr)
		{
			mov.ptr = nullptr;
		}
		SharedMappedMemory(const SharedMappedMemory& cpy) = delete;
	};
	struct ReferencedMappedMemory {
		Streamable& parent;
		void* ptr;
		~ReferencedMappedMemory()
		{
			if(ptr) parent.unmap();
		}
		ReferencedMappedMemory(Streamable& nparent)
			: parent(nparent), ptr(nullptr)
		{
			ptr = parent.map();
		}
		ReferencedMappedMemory(ReferencedMappedMemory&& mov)
			: parent(mov.parent), ptr(mov.ptr)
		{
			mov.ptr = nullptr;
		}
		ReferencedMappedMemory(const ReferencedMappedMemory& cpy) = delete;
	};
	inline ReferencedMappedMemory mapThis() {
		return ReferencedMappedMemory(*this);
	}
};

}
}
#endif // STREAMABLE_HPP
