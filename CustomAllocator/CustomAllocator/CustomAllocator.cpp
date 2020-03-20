#include <iostream>
#include <list>
#include <Windows.h>

using namespace std;

class CustomAllocator {
	//block type
	struct mem_block
	{
		// memory location pointer
		unsigned char* ptr;
		//size of the block which we will allocate
		unsigned int size;
		// pointer to the next memoru block
		mem_block* next;
		//pointer to the previous memory block
		mem_block* prev;
	};

public:
	//constructor
	CustomAllocator()
	{
		used_blocks = new std::list<mem_block*>();
		free_blocks = new std::list<mem_block*>();
		first_block = nullptr;
		last_block = nullptr;
	}
	//func shows status of memory block
	void mem_dump() {
		cout << "Current state:" << endl;

		if (first_block == nullptr) {
			cout << "Memory is empty" << endl;
			return;
		}

		auto curr_block = first_block;
		while (true) {

			cout << "- Size of the block: " << endl << curr_block->size << " - Status: " << endl;
			if (contains_block(free_blocks, curr_block))
				cout << "Is free" << endl;
			else if (contains_block(used_blocks, curr_block))
				cout << "Is used" << endl;
			else
				cout << "Something terrible has happened" << endl;

			if (curr_block->next == nullptr)
				break;
			//go to next block
			curr_block = curr_block->next;
		}
	}
	// allocates a block of memory
	void* mem_alloc(unsigned int size) {
		//makes a block a word size //word=4bytes
		size = align_size(size);
		//try to find free memory block
		mem_block* block = find_free_block(size);
		//if we cant find one => create a new one
		if (block == nullptr)
		{
			block = alloc_block(size);

			if (block == nullptr)
			{
				return nullptr;
			}
		}
		//block free => used
		//cut the block if possible
		split_block(block, size);
		//return pointer
		return block->ptr;
	}

	//resize the block
	void* mem_realloc(void* addr, unsigned int size) {
		//make our momory free
		mem_free(addr);
		//mem_alloc with new size
		return mem_alloc(size);
	}
	//makes block of memory free
	void mem_free(void* addr) {
		//we try to find a block
		//make it free
		//connect 2 bkocks into 1
		const auto ptr = reinterpret_cast<uint8_t*>(addr);
		for (auto block : *used_blocks)
		{
			if (block->ptr != ptr)
				continue;

			used_blocks->remove(block);
			free_blocks->push_back(block);
			join_block(block);
			return;
		}
	}

private:
	//lists
	list<mem_block*>* free_blocks;
	list<mem_block*>* used_blocks;
	//pointersto the first and the last block
	mem_block* first_block;
	mem_block* last_block;

	//makes a block a word size //word=4bytes
	unsigned int align_size(unsigned int size)
	{
		return (size + sizeof(int) - 1) & ~(sizeof(int) - 1);
	}

	mem_block* find_free_block(unsigned int size) {
		//we try to find the first free block
		for (mem_block* block : *free_blocks)
		{
			//we check if the block fits
			if (block->size < size)
				continue;
			return block;
		}
		//if there is no block which first return 
		return nullptr;
	}

	mem_block* alloc_block(unsigned int size) {
		// we allocate memory like meminit
		const auto mem = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
		//check
		if (mem == nullptr)
		{
			return nullptr;
		}
		//block creation
		const auto block = new mem_block();
		block->next = nullptr;
		block->prev = nullptr;
		block->ptr = static_cast<uint8_t*>(mem);
		block->size = size;
		//if this block is the first mark it
		if ((free_blocks->size() == 0) && (used_blocks->size() == 0))
			first_block = block;
		//put the block into free blocks list
		free_blocks->push_back(block);
		//make this block the last
		if (last_block != nullptr)
		{
			last_block->next = block;
			block->prev = last_block;
		}

		last_block = block;

		return block;
	}
	void split_block(mem_block* block, unsigned int size) {
		//remove the block from free blocks list
		free_blocks->remove(block);

		// put it into used blocks list
		used_blocks->push_back(block);

		//return if the block size isequal to given size
		if (block->size == size) return;

		//count the block size difference
		const auto size_diff = block->size - size;
		//create a new block
		const auto splitted_block = new mem_block();
		splitted_block->ptr = block->ptr + size;
		splitted_block->size = size_diff;
		splitted_block->prev = block;
		splitted_block->next = block->next;
		// connext 2 blocks
		block->next = splitted_block;
		block->size = size;
		free_blocks->push_back(splitted_block);
	}
	void join_block(mem_block* block) {
		// check if the next block is free
		if (block->prev != nullptr && (block->prev + block->prev->size == block) && contains_block(free_blocks, block->prev))
		{
			block = join_blocks(block->prev, block);
		}
		// check if the previous block is free
		if (block->next != nullptr && (block + block->size == block->next) && contains_block(free_blocks, block->next))
		{
			block = join_blocks(block, block->next);
		}
	}
	//joins size of 2 blocks
	mem_block* join_blocks(mem_block* left, mem_block* right) {
		left->size += right->size;
		left->next = right->next;
		return left;
	}
	//checks the block in the block list
	bool contains_block(list<mem_block*>* list, mem_block* block) {
		if (block == nullptr)
			return false;
		for (auto list_block : *list)
		{
			if (list_block->ptr == block->ptr)
				return true;
		}
		return false;
	}
};


int main() {

	auto allocator = new CustomAllocator();

	cout << "Create blocks " << endl;
	auto example1 = allocator->mem_alloc(sizeof(int));
	auto example2 = allocator->mem_alloc(sizeof(int));
	auto var1 = new (example1) int(1);
	auto var2 = new (example2) int(2);

	cout << "Memory status" << endl;
	allocator->mem_dump();
	//realloc
	cout << "Realloc and check memory status" << endl;
	auto example3 = allocator->mem_realloc(example2, 7);
	allocator->mem_dump();

	cout << endl << "mem_free and check status" << endl;
	allocator->mem_free(example1);
	allocator->mem_dump();

	system("pause");
};