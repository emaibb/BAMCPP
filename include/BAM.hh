#ifndef BAM_HH
#define BAM_HH

#include "decl.h"
#include <cinttypes>
#include <array>
#include <vector>
#include <memory>
#include <utility>

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
		std::vector<std::pair<bool, std::vector<UV>>> boolean_polygons() const;
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

#endif // BAM_HH
