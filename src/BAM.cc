#include "BAM.hh"
#include "BAMJL.h"
extern "C" {
#include "julia_init.h"
}
#include <string>

struct BAM::Private {
	jl_module_t* BAM;
	jl_value_t* refs;
	jl_function_t* setindex;
	jl_function_t* erase;
	jl_function_t* Mesh2D;
	jl_datatype_t* UV;
	jl_value_t* uv_vec_type;
	jl_function_t* insert;
	jl_function_t* or_polygon;
	jl_function_t* sub_polygon;
	jl_function_t* and_polygon;
	jl_function_t* boolean_polygons;
	Private(int argc, char* argv[]) {
		init_julia(argc, argv);
	}
	~Private() {
		shutdown_julia(0);
	}
};

BAM::BAM(int argc, char* argv[])
	: _p(new Private(argc, argv))
{
	_p->BAM = (jl_module_t*)jl_eval_string("BAM");
	_p->refs = jl_eval_string("refs = IdDict()");
	_p->setindex = jl_get_function(jl_base_module, "setindex!");
	_p->erase = jl_get_function(jl_base_module, "delete!");
	_p->Mesh2D = jl_get_function(_p->BAM, "Mesh2D");
	_p->UV = (jl_datatype_t*)(jl_eval_string("BAM.UV"));
	_p->uv_vec_type = jl_apply_array_type((jl_value_t*)_p->UV, 1);
	_p->insert = jl_get_function(_p->BAM, "insert!");
	_p->or_polygon = jl_get_function(_p->BAM, "or_polygon!");
	_p->sub_polygon = jl_get_function(_p->BAM, "sub_polygon!");
	_p->and_polygon = jl_get_function(_p->BAM, "and_polygon!");
	_p->boolean_polygons = jl_get_function(_p->BAM, "boolean_polygons");
	if (jl_exception_occurred())
		printf("%s \n", jl_typeof_str(jl_exception_occurred()));
}

BAM::BAM(const BAM& other)
	: _p(other._p)
{}

BAM::~BAM() {}

struct BAM::Mesh2D::Private {
	std::shared_ptr<BAM::Private> pBAM;
	jl_value_t* mesh;
	~Private() {
		jl_call2(pBAM->erase, pBAM->refs, mesh);
	}
};

BAM::Mesh2D::Mesh2D()
	: _p(new Private)
{}

BAM::Mesh2D::Mesh2D(const Mesh2D& other)
	: _p(other._p)
{}

BAM::Mesh2D::~Mesh2D()
{}

BAM::Mesh2D BAM::new_mesh2d()
{
	jl_value_t* mesh = jl_call0(_p->Mesh2D);
	JL_GC_PUSH1(&mesh);
	jl_call3(_p->setindex, _p->refs, mesh, mesh);
	JL_GC_POP();
	Mesh2D ret;
	ret._p->pBAM = _p;
	ret._p->mesh = mesh;
	return ret;
}

void BAM::Mesh2D::insert(UV uv) const
{
	jl_value_t* uv_ = jl_new_struct(_p->pBAM->UV, jl_box_int32(uv[0]), jl_box_int32(uv[1]));
	JL_GC_PUSH1(&uv_);
	jl_call2(_p->pBAM->insert, _p->mesh, uv_);
	JL_GC_POP();
}

const BAM::Mesh2D& BAM::Mesh2D::operator|=(const BAM::Polygon& polygon) const
{
	jl_array_t* uvs = jl_alloc_array_1d(_p->pBAM->uv_vec_type, polygon.size());
	for (size_t i = 0; i < polygon.size(); ++ i)
		((UV*)jl_array_data(uvs))[i] = polygon[i];
	JL_GC_PUSH1(&uvs);
	jl_call2(_p->pBAM->or_polygon, _p->mesh, (jl_value_t*)uvs);
	JL_GC_POP();
	return *this;
}

const BAM::Mesh2D& BAM::Mesh2D::operator-=(const BAM::Polygon& polygon) const
{
	jl_array_t* uvs = jl_alloc_array_1d(_p->pBAM->uv_vec_type, polygon.size());
	for (size_t i = 0; i < polygon.size(); ++i)
		((UV*)jl_array_data(uvs))[i] = polygon[i];
	JL_GC_PUSH1(&uvs);
	jl_call2(_p->pBAM->sub_polygon, _p->mesh, (jl_value_t*)uvs);
	JL_GC_POP();
	return *this;
}

const BAM::Mesh2D& BAM::Mesh2D::operator&=(const BAM::Polygon& polygon) const
{
	jl_array_t* uvs = jl_alloc_array_1d(_p->pBAM->uv_vec_type, polygon.size());
	for (size_t i = 0; i < polygon.size(); ++i)
		((UV*)jl_array_data(uvs))[i] = polygon[i];
	JL_GC_PUSH1(&uvs);
	jl_call2(_p->pBAM->and_polygon, _p->mesh, (jl_value_t*)uvs);
	JL_GC_POP();
	return *this;
}

struct buvs_t{
	bool b;
	jl_array_t* uvs;
};
static_assert(sizeof(buvs_t) == 16, "sizeof(buvs_t) != 16");

std::vector<std::pair<bool, std::vector<BAM::UV>>> BAM::Mesh2D::boolean_polygons() const
{
	jl_array_t* jl_ret = (jl_array_t*)jl_call1(_p->pBAM->boolean_polygons, _p->mesh);
	JL_GC_PUSH1(&jl_ret);
	std::vector<std::pair<bool, std::vector<UV>>> ret;
	for (size_t i = 0; i < jl_ret->length; ++i) {
		auto buvs = ((buvs_t*)jl_array_data(jl_ret))[i];
		auto uvs_begin = (UV*)jl_array_data(buvs.uvs);
		std::vector<UV> uvs( uvs_begin, uvs_begin + buvs.uvs->length );
		ret.push_back({buvs.b, uvs});
	}
	JL_GC_POP();
	return ret;
}
