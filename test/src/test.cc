#include "BAM.hh"
#include "test.hh"
#include <iostream>
#include <chrono>

int main(int argc, char* argv[]) {
	BAM bam(argc, argv);
	auto mesh = bam.new_mesh2d();
	mesh |= polygons_3DIC[0];
	for (auto i = 1; i < polygons_3DIC.size(); ++i) {
		mesh -= polygons_3DIC[i];
	}
	auto ret = mesh.boolean_polygons();
	std::cout << ret.size() << std::endl;
}