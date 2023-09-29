#pragma once


struct PhysicsCmp_t;

struct PhysicsSys_t {

	explicit PhysicsSys_t() = default;
	
	void update(PhysicsCmp_t&, const float dt) const;
};