#pragma once
#include <cmp/physicsCmp.hpp>
#include <cmp/AICmp.hpp>

struct Pointer_t {

	explicit Pointer_t() = default;

	PhysicsCmp_t phycmp;
	AICmp_t 	 aicmp;
};