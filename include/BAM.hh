#ifndef BAM_HH
#define BAM_HH

#include "decl.h"
#include <cinttypes>
#include <array>
#include <vector>

class DECL_SHARE BAM {
public:
	using Coord = int32_t;
	using UV = std::array<Coord, 2>;
	using Polygon = std::vector<UV>;
	struct DECL_SHARE Mesh2D {
		void* mesh;
		BAM* bam;
	public:
		void insert(UV uv) const;
		const Mesh2D& operator|=(const Polygon&) const;
		const Mesh2D& operator-=(const Polygon&) const;
		const Mesh2D& operator&=(const Polygon&) const;
		
	};
public:
	BAM(int argc, char* argv[]);
	~BAM();
public:
	Mesh2D mesh2d();
	void erase(const Mesh2D&);
private:
	struct Private;
	Private* _p;
};

#endif // BAM_HH
