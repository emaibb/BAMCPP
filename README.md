# BAMCPP

## Build

Dependencies:

- CMake 3.16 or higher
- libjulia (v1.7 tested)
- libBAM

## Installation

By default, after building on Windows, `BAMCPP` will generate `BAM_cpp.dll` and `BAM_test.exe` under `libBAM/bin`.

## Usage
`BAMCPP/include/BAM.hh`
```cpp
class DECL_SHARE BAM {
public:
	using Coord = int32_t;
	using UV = std::array<Coord, 2>;
	using Polygon = std::vector<UV>;
	class DECL_SHARE Mesh2D {
		struct Private;
		std::shared_ptr<Private> _p;
	public:
		friend class BAM;
		Mesh2D();
		Mesh2D(const Mesh2D&);
		~Mesh2D();
		void insert(UV uv) const;
		const Mesh2D& operator|=(const Polygon&) const;
		const Mesh2D& operator-=(const Polygon&) const;
		const Mesh2D& operator&=(const Polygon&) const;
		std::vector<std::pair<bool, Polygon>> boolean_polygons() const;
	};
public:
	BAM(int argc, char* argv[]);
	BAM(const BAM&);
	~BAM();
public:
	Mesh2D new_mesh2d();
private:
	struct Private;
	std::shared_ptr<Private> _p;
};
```

2D polygon data structure `BAM::Polygon` is defined as `std::vector` of `BAM::UV`, satisfying:
- Coordinate is stored using `Coord(int32_t)`, and takes value between `-2^30 ~ 2^30`.
- Edges induced by `BAM::Polygon uvs` can be represented by `UV` pairs `{{uvs[0], uvs[1]}, {uvs[1], uvs[2]}, ..., {uvs[uvs.size()-1], uvs[0]}}`.
- Edge trajectory is counterclockwise.

`BAMCPP/test/src/test.cc`
```cpp
#include "BAM.hh"
#include "test.hh"
#include <iostream>
#include <chrono>

int main(int argc, char* argv[]) {
    // Start julia agent, and caching BAM.jl package utility.
	BAM bam(argc, argv);
    // Start timing.
	auto start = std::chrono::system_clock::now();
	// A new 2D bounded adaptive mesh instance.
    auto mesh = bam.new_mesh2d();
    // Boolean operations (|= : boolean or, -+ : boolean sub)
	mesh |= polygons_3DIC[0];
	for (auto i = 1; i < polygons_3DIC.size(); ++i) {
		mesh -= polygons_3DIC[i];
	}
    // Extract boolean operation and polygons, returns std::vector<std::pair<bool, BAM::Polygon>>, with bool value represents boolean operation signature (false : |=, true : -=), and element permutation preserves operation order.
	auto ret = mesh.boolean_polygons();
    // Show total cost seconds.
	std::cout 
		<< std::chrono::duration_cast<std::chrono::seconds>(
			std::chrono::system_clock::now() - start
		).count() 
		<< std::endl
	;
    // Count output elements.
	std::cout << ret.size() << std::endl;
}
```
