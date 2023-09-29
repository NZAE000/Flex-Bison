#pragma once

struct Pointer_t;
struct PhysicsCmp_t;
struct Point2D_t;

struct AISys_t {

	explicit AISys_t() = default;
	
	void update(Pointer_t&) const;

private:
		
	// MRU (only velocity control) or MRUA (acceleration control) on linear and angular
	struct SteerTarget_t {
		float linear  {0.0};
		float angular {0.0};
	};

	SteerTarget_t
	arrive(PhysicsCmp_t const&, Point2D_t const&, float arrivalTime, float arrivalRadius) const;
};